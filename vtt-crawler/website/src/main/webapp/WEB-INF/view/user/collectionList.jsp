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
	var collectionObj = {
			columns: function fnGetColumns() {
				return [
				    {key: 'INPUT_DATE'} /*, formatter: 'dateFormat'*/
				,   {key: 'USER_ID'}
		        ,	{key: 'SC_NAME', styleClass: 'left', binder: 'collectionSubject'}
		        ,	{key: 'SC_URL', styleClass: 'left'}
		        ,	{key: 'SC_STATE'}
		        ,	{key: 'SC_COUNT_W'}
		        ,   {key: 'END_DATE'}
		        ];	
			}		
		,	tableViewStart: function fnSetTableView() {
			this.tableServive = $('#collectionList').tableSetData({
				ajaxUrl: '/user/collectionList'
			,   ajaxPars:{PageNo          : $('#pageNo').val(),
				      	  searchField1    : "SC_STATE",	
				          searchValue1    : $('#CRAWLER_STATE_VALUE').val(),	
				      	  searchField2    : "SC_NAME",	
				          searchValue2    : $('#SC_NAME').val(),
				      	  searchField3    : "USER_ID",	
				          searchValue3    : $('#USER_ID_VALUE').val(),				          
				          searchStartDateValue1:$('#searchStartDateValue1').val(),
				          searchEndDateValue1  :$('#searchEndDateValue1').val()
						} 				
			,	columns: collectionObj.columns()
			,	binder: {
					collectionSubject: function(item) {
						//여기서 페이지이동 
	/*  					console.log('move to noticeMainList selected Item page');
						console.log('selected Item : ', item); 
						console.log('selected Item : ', item.SID);*/
						location.href='/user/collectionView?SID='+item.SID; 
					}
				}
			,	onLoad: function(target) {
				}
					
			,	onError: function(target) {	
			
				}
				});
			}
		
		};
		
		function fnPaging(page){
			fnSearch(page);
		}
		
		function fnSearch(page){
			if(page) $('#frm input[name=pageNo]').val(page);
			else $('#frm input[name=pageNo]').val(0);

			fnInquiry();
		}
		
		function fnInquiry(){
			$('#collectionbody').empty();	
			
			collectionObj.tableViewStart();			
		}	
		
	   $(document).ready(function(){
	       var dDay = new Date();
	       var yDay = new Date(dDay.getFullYear() - 1, (dDay.getMonth() - 1), dDay.getDate());
	
	       $("#searchStartDateValue1").datepicker().datepicker("setDate", yDay);
	       $("#searchEndDateValue1").datepicker().datepicker("setDate", dDay);
	       
	       collectionObj.tableViewStart();
	   });
   </script> 
</head>
<body>
		<%		
		String userId =(String)session.getAttribute("USER_ID");	
				
		 %>		
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
	<fieldset class="fieldset large">
		<input type="hidden" name="pageNo" id="pageNo" value="0">
		<legend></legend>
		<div class="field">
			<div class="insert">
				<label class="label" for="input01">상태</label>
				<select name="CRAWLER_STATE" id="CRAWLER_STATE_VALUE"  style="width: 90px; padding-left: 10px;">
					<option value="-1" >전체</option>
					<option value="0" >대기</option>
					<option value="1" >수집중</option>
					<option value="2" >완료</option>   												
				</select>
			</div>
			<div class="insert">
				<label class="label" for="input02">등록일</label>
				<div class="input-date">
					<input type="text" name="searchStartDateValue1" id="searchStartDateValue1" class="datepicker">
				</div>
				<span class="hypen"></span>
				<div class="input-date">
					<input type="text" name="searchEndDateValue1" id="searchEndDateValue1" class="datepicker">
				</div>
			</div>
			<div class="insert">
				<label class="label" for="input03">스케줄명</label>
				<div class="input-text" style="width: 195px;"><input type="text" id="SC_NAME" placeholder="스케줄명"></div>
			</div>		
			<input type="hidden" id="USER_ID_VALUE" value="<%=userId %>">				
			<button type="button" class="btn type01 medium" onclick="fnSearch();">조회<span class="ico-search"></span></button>
		</div>
	</fieldset>
	
	<form name="frm" id="frm" action="/user/collectionList">	
	<input type="hidden" name="pageNo" value="${pageInfo.pageNo}" />			
	<div class="table-wrap">
		<table class="data-table highlight" id="collectionList" >
			<caption>스캐줄 조회</caption>
			<colgroup>
				<col width="9%">
				<col width="9%">
				<col width="20%">
				<col width="33%">
				<col width="9%">
				<col width="11%">
				<col width="9%">										
			</colgroup>
			<thead>
			   <tr>
					<th scope="col">등록일</th>							
					<th scope="col">수집원</th>
					<th scope="col">스케줄명</th>
					<th scope="col">수집위치</th>
					<th scope="col">수집상태</th>
					<th scope="col">수집수</th>
					<th scope="col">완료일</th>
				</tr>
			</thead>
			<tbody id="collectionbody">
				<!--데이터  -->
			</tbody>
		</table>
	</div>
	<%@ include file="/include/common/paging.jsp" %> 
	</form> 	
</div>	
</div>			
<!-- // CONTENT -->
</div>
<!-- // WRAP -->
</body>
</html>
