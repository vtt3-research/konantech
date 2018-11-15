<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ include file="/include/common/taglibs.jsp" %>

<html>
<head>
	<title>KONAN CRAWLER</title>
	<link rel="stylesheet" type="text/css" href="../../resources/css/jquery-ui.min.css">
	<link rel="stylesheet" type="text/css" href="../../resources/css/jquery-ui.theme.min.css">	
	<link rel="stylesheet" type="text/css" href="../../resources/css/common.css">
	<link rel="stylesheet" type="text/css" href="../../resources/css/layout.css">
	
	<script type="text/javascript" src="../../resources/js/jquery-3.3.1.min.js"></script>	
	<script type="text/javascript" src="../../resources/js/jquery.form-3.51.0.js"></script>
	<script type="text/javascript" src="../../resources/js/jquery-ui.min.js"></script>
	<script type="text/javascript" src="../../resources/js/common.js"></script>
	
	<script type="text/javascript" src="../../resources/js/konan_main_common.js"></script>
	
	<script type="text/javascript">
	var crawlerObj = {
			columns: function fnGetColumns() {
				return [
				    {key: 'ROWNUM'}
				,   {key: 'TITLE', binder: 'crawlerView'} /*, formatter: 'dateFormat'*/
				,   {key: 'CATEGORY'}
		        ,	{key: 'LICENSE'}
		        ,	{key: 'CONTENTS', styleClass: 'left'}
		        ,	{key: 'ISSUCCESS'}
		        ,	{key: 'FILENAME', binder: 'crawlerDownload'}
		        ,   {key: 'SERVER_IP' , noneClass:'display:none;' }
		        ];	
			}		
		,	tableViewStart: function fnSetTableView() {
			this.tableServive = $('#crawlerList').tableSetData({
				ajaxUrl: '/user/collectionView'
			,   ajaxPars:{SID : <c:out value="${collectionInfo.SID}"/>
				} 						
			,	columns: crawlerObj.columns()
			,	binder: {
					crawlerDownload: function(item) {
						//여기서 페이지이동 
/* 	  					console.log('move to noticeMainList selected Item page');
						console.log('selected Item : ', item); 
						console.log('selected Item : ', item.SID); */
						location.href='ftp://' + item.SERVER_IP + '/download/' + ${collectionInfo.SID} + '/' + item.FILENAME; 
					},
					crawlerView: function(item) {
						//여기서 페이지이동 
	/*  					console.log('move to noticeMainList selected Item page');
						console.log('selected Item : ', item); 
						console.log('selected Item : ', item.SID);*/
						location.href='http://' + item.SERVER_IP + '/download/' + ${collectionInfo.SID} + '/' + item.FILENAME; 
					}					
				}
			,	onLoad: function(target) {
				}
					
			,	onError: function(target) {	
			
				}
				});
			}
		
		};
	

	function download_files(files) {
		  function download_next(i) {
		    if (i >= files.length) {
		      return;
		    }
		    var a = document.createElement('a');
		    a.href = files[i].download;
		    //a.target = '_parent';
		    a.target = '_blank';
		    if ('download' in a) {
		      a.download = files[i].filename;
		    }
		    (document.body || document.documentElement).appendChild(a);
 		    if (a.click) {
		      a.click(); // The click method is supported by most browsers.
		    } else {
		      $(a).click(); // Backup using jquery
		    }
		    a.parentNode.removeChild(a); 
		    setTimeout(function() {
		      download_next(i + 1);
		    }, 500);
		  }
		  download_next(0);
		}
	
	
	
	   function fnDownload(){
		   var aJsonArray = new Array();
		   var sJson = JSON.stringify();
	   
 		   $('#crawlerList tr').each(function() {	 
 			  var aJson = new Object(); 
			  var fileName = $(this).find("td").eq(6).text();
			  var serverIp = $(this).find("td").eq(7).text(); 	
 			  aJson.download  = 'ftp://' + serverIp + '/download/' + ${collectionInfo.SID} + '/' + fileName;
 			  aJson.filename = fileName;
              aJsonArray.push(aJson);
 		   });
 		   aJsonArray.splice(0,1)

		   download_files(aJsonArray); 	 

	   }
		
	   $(document).ready(function(){
	       crawlerObj.tableViewStart();
	   });
	   
		function fnCancel(){
			var SC_ID = '${collectionInfo.SID}';
			var check = confirm("스케줄을 취소하겠습니다?\n이미 크롤링을 시작했다면 수집한 동영상까지만 크롤링 합니다.");
			
			if(check) {
				$.ajax({
						url : '/user/collectionCancel',
						type : 'POST',
						data : {SID : SC_ID},
						beforeSend : function(){},
						success : function(response) {
							//console.log(response.resultCode);
							if(response.resultCode != RESULT_CODE.ERROR){
								alert('취소 되었습니다.');
								location.href='/user/collectionList';						
							}else{
								alert(response.resultMessage);
							}
						}
					});	  
			}
		}	
		
		function fnDelete(){
			var SC_ID = '${collectionInfo.SID}';
			var check = confirm("스케줄을 삭제하겠습니다?");
			
			if(check) {
				$.ajax({
						url : '/user/collectionDelete',
						type : 'POST',
						data : {SID : SC_ID},
						beforeSend : function(){},
						success : function(response) {
							//console.log(response.resultCode);
							if(response.resultCode != RESULT_CODE.ERROR){
								alert('삭제 되었습니다.');
								location.href='/user/collectionList';						
							}else{
								alert(response.resultMessage);
							}
						}
					});	  
			}
		}	
		
		function downloadURI(uri, name) {
			  var blob=new Blob([uri], {type: 'video/mp4'});	
			  var link = document.createElement("a");
			  link.download = name;
			  link.href = window.URL.createObjectURL(blob);
			  document.body.appendChild(link);
			  link.click();
			  document.body.removeChild(link);
			  delete link;
			}		
   </script> 
</head>
<body>
		<%		
		String userId =(String)session.getAttribute("USER_ID");	
				
		 %>		
<a href="#container" class="skip-navi">본문 바로가기</a>
<!-- WRAP -->
<div id="wrap" class="depth-sub">
<!-- HEADER -->
<div id="header" class="active"> <!-- // 활성화 시 active 클래스 추가 -->
	<!-- header-wrap -->
	<div class="header-wrap active"> <!-- // 활성화 시 active 클래스 추가 -->
		<div class="header-inner">		    
			<h1></h1>
			<!-- global-menu -->
			<div class="global-menu">
				<div class="greeting">
					<span class="position"><%=userId %></span>					
				</div>
				<a href="/logout" class="logout">LOGOUT</a>				
			</div>
			<!-- // global-menu -->
			<!-- gnb -->
			<div class="gnb">
				<ul class="nav">                    
					<li class="depth01 menu01 active"><a href="/user/collectionList">스케줄 조회</a></li>					
					<li class="depth01 menu02"><a href="/user/collectionWrite">스케줄 등록</a></li>
					<li class="depth01 menu03"><a href="/user/serverState">서버 모니터링</a></li>
				</ul>
			</div>
			<!-- // gnb -->
		</div>
		<!--// header-inner -->
	</div>
	<!--// header-wrap -->
	<span class="bg"></span>
</div>
<!-- // HEADER -->

<!-- CONTENT -->
<div id="container" class="sub">
	<div id="content">
		<div class="title-wrap">
			<h1 class="content-title">스케줄 조회</h1>			    
		</div>
		<div class="notice-write">
			<form name="frm" id="frm" onsubmit="return false;">
				<div class="view-header">
					<div class="insert">
						<div class="label">스케줄명</div>					
						<div class="input-text"><input type="text" id="SCHEDULE_NAME_VALUE" name="SCHEDULE_NAME" <c:if test="${not empty collectionInfo.SC_NAME}"> value="${collectionInfo.SC_NAME}"</c:if> readonly="readonly"></div>
					</div>
					<div class="insert">
						<div class="label">수집 사이트</div>
						<div>
							<table>
								<tr>
									<td style="width:200px">
										<select name="COLLECTION_SITE" id="COLLECTION_SITE_VALUE"  style="width: 200px; padding-left: 10px;">
											<option value="https://www.youtube.com" <c:if test="${collectionInfo.SC_SITE == 'https://www.youtube.com'}"> selected="selected"</c:if>>YOUTUBE</option>
										</select>									
									</td>
									<td style="width:150px"><div class="label1" style="width:110px">수집 위치</div></td>
									<td style="width:700px"><div class="input-text"><input type="text" id="SCHEDULE_URL_VALUE" name="SCHEDULE_URL" <c:if test="${not empty collectionInfo.SC_URL}"> value="${collectionInfo.SC_URL}"</c:if>  readonly="readonly"></div></td>									
								</tr>
							</table>
						</div>		
					</div>
					<div class="insert">
						<div class="label">수집 형태</div>
						<div>
							<table>
								<tr>
									<td style="width:200px">
										<select name="COLLECTION_TYPE" id="COLLECTION_TYPE_VALUE"  style="width: 200px; padding-left: 10px;">
											<option value=1 <c:if test="${collectionInfo.SC_TYPE == 1}"> selected="selected"</c:if>>동영상</option>
										</select>										
									</td>
									<td style="width:150px"><div class="label1" style="width:110px">수집 확장자</div></td>
									<td style="width:200px">
										<div>
											<select name="COLLECTION_EXT" id="COLLECTION_EXT_VALUE"  style="width: 200px; padding-left: 10px;">
												<option value="*" <c:if test="${collectionInfo.SC_EXT == '*'}"> selected="selected"</c:if> >전체</option>
											</select>
										</div>
									</td>
									<td style="width:150px"><div class="label1" style="width:110px">최대 크기</div></td>
									<td style="width:200px"><div class="input-text"><input type="text" id="FILE_SIZE_VALUE" name="FILE_SIZE" <c:if test="${not empty collectionInfo.SC_MAXSIZE}"> value="${collectionInfo.SC_MAXSIZE}"</c:if> readonly="readonly"></div></td>
									<td style="width:150px"><div class="label1">(MB)</div></td>										
								</tr>
							</table>
						</div>								
					</div>
				</div>
			</form>
			<div class="button-area medium">
				<a href="/user/collectionList" class="btn type04 medium">목록으로</a>
				<c:if test="${collectionInfo.SC_STATE == '대기' or collectionInfo.SC_STATE == '진행'}"><a href="javascript:fnCancel();" class="btn type03 medium">취소</a></c:if>				
				<a href="javascript:fnDownload();" class="btn type01 medium">전체 다운로드</a>
			</div>
		</div>
		
	<div class="button-area small">		
		<span style="font-size: 16px; font-weight: 700;">수집내용</span>
	</div>
	<div class="table-wrap">
		<table class="data-table highlight" id="crawlerList" >
			<caption>수집정보</caption>
			<colgroup>
				<col width="5%">
				<col width="20%">
				<col width="10%">
				<col width="10%">
				<col width="30%">
				<col width="10%">
				<col width="15%">										
			</colgroup>
			<thead>
			   <tr>
					<th scope="col">순번</th>
					<th scope="col">타이틀</th>
					<th scope="col">카테고리</th>							
					<th scope="col">라이센스</th>
					<th scope="col">내용</th>
					<th scope="col">성공/실패</th>
					<th scope="col">다운로드</th>					
				</tr>
			</thead>
			<tbody>
				<!--데이터  -->
			</tbody>
		</table>
	</div>
			
	</div>
</div>	
<!-- // CONTENT -->
</div>
<!-- // WRAP -->
</body>
</html>
