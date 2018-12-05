/**
 * 테이블 공통 공통
 */
$.fn.tableSetData = function(options) {
	var _targetTable = $(this);
	var _targetTBody = $(this).find('tbody');
	var _targetTBody1 = $('#pageDiv');
	var _targetTBody2 = $('#pagination');
	var _ajaxUrl = options.ajaxUrl;
	var _ajaxPars = options.ajaxPars || {};
	var _limit = options.limit;
	var _columns = options.columns || [];
	var _binder = options.binder || {};
	var _onLoad = options.onLoad || function() {};
	var _onError = options.onError || function() {};	
	var _check = options.check;
	//var _mask = new maskService(); 
	return new tableService(_targetTable, _targetTBody,_targetTBody1,_targetTBody2, _ajaxUrl, _ajaxPars, _limit, _columns, _binder,  _onLoad, _onError, _check);
};

var tableService = function(_targetTable, _targetTBody,_targetTBody1,_targetTBody2, _ajaxUrl, _ajaxPars, _limit, _columns, _binder,  _onLoad, _onError, _check) {
	var self = this;
	self.setTable = function() {
	
		$.ajax({
			type: 'POST'
			,	url: _ajaxUrl
			,	data: _ajaxPars
			,	beforeSend : function(){
			//	_mask.openMask();
			}
			,	success: function(response) {

					if(response.resultCode != RESULT_CODE.ERROR) {
						var list = response.resultObject.list || [];
						var pageInfo = response.resultObject.pageInfo;
						var totalCount = response.resultObject.totalCount;

						insertTableData(true, _targetTBody, list, _limit, _columns, _check);
						_targetTBody2.remove();
						insertPageData(true, _targetTBody1, pageInfo);
					
						$.each(_targetTBody.find('a'), function(idx) {
							$(this).bind('click', function() {
	
								_binder[$(this).attr('binder')](list[$(this).attr('index')]);
							});
						});
						_onLoad(_targetTable, totalCount);
					} else {
						insertTableData(false, _targetTBody, list, _limit, _columns);
						_onError(_targetTable, list, response);
					}
					//_mask.closeMask(800);
				}
		});
	};
	
	self.setTable();
	return self;
};


/**
 * 공통 함수
 */
function insertTableData(isSuccess, targetTBody, list, limit, columns, check) {
	var dataHtml = new Array();	
	if(!isSuccess) {
		dataHtml.push('<tr><td colspan='+columns.length+'>데이터 조회에 실패하였습니다.</td></tr>');
	} else if(fnIsEmpty(list)) {
		dataHtml.push('<tr><td colspan='+columns.length+'>조회된 데이터가 없습니다.</td></tr>');
	} else {
		$.each(list, function(i) {
			if(i >= limit) return false;
			var rowData = this;

			dataHtml.push('<tr index="'+i+'">');
				
			$.each(columns, function(j) {
                if(fnIsEmpty(this.noneClass)){  
                	dataHtml.push( (fnIsEmpty(this.styleClass)) ? '<td>' : '<td class="'+this.styleClass+'">');
                }else{
                	dataHtml.push( (fnIsEmpty(this.noneClass)) ? '<td>' : '<td style="display:none;">');}
                
            	var data = fnIsEmpty(this.formatter) ? rowData[this.key] : mainFormatter(this.formatter, this.key, rowData[this.key]);
				if(this.key=='SC_URL'){
					data = decodeURIComponent(data);
                }                
				
				if(!fnIsEmpty(this.binder)) {
					dataHtml.push('<a href="javascript:void(0);" binder="'+this.binder+'" index="'+i+'">' + data + '</a>');
				}else dataHtml.push(data);	

				dataHtml.push('</td>');
			});
			dataHtml.push('</tr>');
		});
	}

	targetTBody.append(dataHtml.join(''));
}

function insertPageData(isSuccess, targetTBody, pageInfo) {
	var dataHtml = new Array();	

	if(!isSuccess) {
	} else if(fnIsEmpty(pageInfo)) {
	} else {
		    dataHtml.push('<div class="pagination" id="pagination">');
		    if(!pageInfo.firstWindow){
		    	dataHtml.push('<a href="javascript:fnPaging('+pageInfo.prevWindow+')" class="first">처음</a>');
		    }
		    if(!pageInfo.firstPage){
		    	dataHtml.push('<a href="javascript:fnPaging('+pageInfo.prevPage+')" class="prev">이전</a>');
		    }
		    dataHtml.push('<span class="page">');
		    var pagenum = pageInfo.pages;
		    $.each(pagenum, function(i) {
		    	if((pagenum[i]-1)==(pageInfo.pageNo)){
		    		dataHtml.push('<strong title="현위치">'+pagenum[i]+'</strong>');
		    	}else{
 		
		    		dataHtml.push('<a href="javascript:fnPaging('+(pagenum[i]-1)+')">'+pagenum[i]+'</a>');
		    	}
		    });	
		    dataHtml.push('</span>');
		    
		    if(!pageInfo.lastPage){
		    	dataHtml.push('<a href="javascript:fnPaging('+pageInfo.nextPage+')" class="next">다음</a>');
		    }
		    if(!pageInfo.lastWindow){
		    	dataHtml.push('<a href="javascript:fnPaging('+pageInfo.nextWindow+')" class="last">끝</a>');
		    }
		    dataHtml.push('</div>');
		    targetTBody.append(dataHtml.join(''));
	}
	

	
}



function mainFormatter(formatter, key, value) {
	
	if(formatter == 'commaFormat') {
		if(key.indexOf('_KM') > -1) return fnIsEmpty(value) ? '' : numeral(Number(value)).format('0,0')+' km';
		else return fnIsEmpty(value) ? '' : numeral(Number(value)).format('0,0');
	} else if(formatter == 'dateFormat') {
		return fnIsEmpty(value) ? '' : moment(fnTypeof(value) == 'object' ? value.time : value).format('YYYY.MM.DD');
	} else if(formatter == 'datetimeFormat') {
		return fnIsEmpty(value) ? '' : moment(fnTypeof(value) == 'object' ? value.time : value).format('YYYY.MM.DD HH:mm:ss');
	} else if(formatter == 'timespanFormat') {
		return fnIsEmpty(value) ? '0' : fnMinsToHHmm(value);
	} else if(formatter == 'approvalBtn') {
		if(value == 'Y'){return '<button class="btn_permi">승인</button';}
		else if(value == 'N'){return '<button class="btn_unpermi">미승인</button>';}
		else return value;
	} else if(formatter == 'useynBtn') {
		if(value == 'Y'){return '<button class="btn_stop on">승인</button';}
		else if(value == 'N'){return '<button class="btn_stop">권한중지</button>';}
		else return value;
	} else {
		return value; 
	}
}



/**
 * 마스크 공통
 */
var maskService = function() {
	self = this;
	var mask = new ax5.ui.mask();
	self.openMask = function(target, message, theme) {
		var gridMaskDefaultMsg = "데이터   조회중입니다 .";
		if(!fnIsEmpty(mask) && mask.status == 'on') return;
		var targetElement = target || "";
		var maskConfig = {};
		if(!fnIsEmpty(targetElement) && typeof targetElement == 'object') {
			
			if(targetElement.length > 0)  {
				maskConfig.target = targetElement;
				open();
			} else {
				var isTimeout = false;
				var findMainElementInterval = setInterval(
					function() {
						if(targetElement.length > 0) {
							maskConfig.target = targetElement;
							open(findMainElementInterval);
						}
						if(isTimeout) clearInterval(findMainElementInterval);
					}
				,	100
				);
				setTimeout(function() {isTimeout=true;}, 3000);
			}
		} else {
			open();
		}
		
		function open(interval) {
			if(!fnIsEmpty(interval)) clearInterval(interval);
			theme = theme || null;
			maskConfig.content = '<h1 class="modal_loading" style="font-size:15px; color:#5D5D5D;"><i class="fa fa-spinner fa-spin"></i> &nbsp;&nbsp;'+(message || gridMaskDefaultMsg)+'</h1>';
			 
			maskConfig.zIndex = ZINDEX++;
			if(theme) maskConfig.theme = theme;
			mask.open(maskConfig);
		}
	};
	
	self.closeMask = function(deleyTime) {
		if(!fnIsEmpty(mask) && mask.status == 'on') {
			deleyTime = deleyTime || 500;
			setTimeout(function() {mask.close();}, deleyTime);
		}
	};
	
	return self;
};
