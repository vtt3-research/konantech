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
	var serverObj = {
			columns: function fnGetColumns() {
				return [
				    {key: 'ROWNUM'}
				,   {key: 'SERVER_NAME'} /*, formatter: 'dateFormat'*/
				,   {key: 'SERVER_IP'}
		        ,	{key: 'LOCAL_IP'}
		        ,	{key: 'SERVER_CHECK'}
		        ,	{key: 'USE_PROXY', binder: 'useproxy'}
		        ,	{key: 'SERVER_ACTIVE', binder: 'activeserver'}	
		        ];	
			}		
		,	tableViewStart: function fnSetTableView() {
			this.tableServive = $('#serverList').tableSetData({
				ajaxUrl: '/admin/serverState'
			,	columns: serverObj.columns()
			,	binder: {
					activeserver: function(item) {
						//여기서 페이지이동 
	/*  					console.log('move to noticeMainList selected Item page');
						console.log('selected Item : ', item); 
						console.log('selected Item : ', item.SID);*/
						location.href='/admin/serverActive?SID=' + item.SID; 
					},
					useproxy: function(item) {
						if (item.USE_PROXY == 'N'){
							location.href='/admin/serverProxy?SID=' + item.SID + '&USE_PROXY=Y';	
						}
						else {
							location.href='/admin/serverProxy?SID=' + item.SID + '&USE_PROXY=N';
						}
						 
					}					
			
				}
			,	onLoad: function(target) {
				}
					
			,	onError: function(target) {	
			
				}
				});
			}
		
		};
		
		function fnInquiry(){
			$('#serverbody').empty();			
			serverObj.tableViewStart();			
		}			
   	
	   $(document).ready(function(){
		   serverObj.tableViewStart();
	   });
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
					<span class="position">[관리자] <%=userId %></span>					
				</div>
				<a href="/logout" class="logout">LOGOUT</a>				
			</div>
			<!-- // global-menu -->
			<!-- gnb -->
			<div class="gnb">
				<ul class="nav">                    
					<li class="depth01 menu01"><a href="/admin/collectionList">스케줄 조회</a></li>					
					<li class="depth01 menu02"><a href="/admin/collectionWrite">스케줄 등록</a></li>
					<li class="depth01 menu03 active"><a href="/admin/serverState">서버 모니터링</a></li>
					<li class="depth01 menu04"><a href="/admin/domainList">도메인 계정 조회</a></li>						
					<li class="depth01 menu06"><a href="/admin/memberList">수집원 계정 조회</a></li>
					<li class="depth01 menu07"><a href="/admin/proxyList">프록시 서버 조회</a></li>																								
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
			<h1 class="content-title">서버 모티터링</h1>			    
		</div>	
	<form name="frm" id="frm" action="/admin/serverState">	
	<div class="table-wrap">
		<table class="data-table highlight" id="serverList" >
			<caption>스캐줄 조회</caption>
			<colgroup>
				<col width="5%">
				<col width="25%">
				<col width="20%">				
				<col width="20%">
				<col width="10%">
				<col width="10%">
				<col width="10%">											
			</colgroup>
			<thead>
			   <tr>
					<th scope="col">순번</th>
					<th scope="col">서버명</th>							
					<th scope="col">외부아이피</th>
					<th scope="col">내부아이피</th>
					<th scope="col">접속가능상태</th>
					<th scope="col">프록시사용</th>
					<th scope="col">활성화</th>					
				</tr>
			</thead>
			<tbody id="serverbody">
				<!--데이터  -->
			</tbody>
		</table>
	</div>
	</form> 			
	</div>
</div>	
<!-- // CONTENT -->
</div>
<!-- // WRAP -->
</body>
</html>
