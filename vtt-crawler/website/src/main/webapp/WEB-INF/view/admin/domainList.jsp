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
	var domainObj = {
			columns: function fnGetColumns() {
				return [
				    {key: 'ROWNUM'}
				,   {key: 'INPUT_DATE'} /*, formatter: 'dateFormat'*/				
		        ,	{key: 'DOMAIN', styleClass: 'left', binder: 'domainSubject'}
		        ,   {key: 'USER_ID'}
		        ,   {key: 'USER_PASS'}		        
		        ];	
			}		
		,	tableViewStart: function fnSetTableView() {
			this.tableServive = $('#domainList').tableSetData({
				ajaxUrl: '/admin/domainList'
			,   ajaxPars:{PageNo          : $('#pageNo').val(),
				      	  searchField2    : "DOMAIN",	
				          searchValue2    : $('#DOMAIN_VALUE').val()	
						} 				
			,	columns: domainObj.columns()
			,	binder: {
					domainSubject: function(item) {
						//여기서 페이지이동 
	/*  					console.log('move to noticeMainList selected Item page');
						console.log('selected Item : ', item); 
						console.log('selected Item : ', item.SID);*/
						location.href='/admin/domainView?DOMAIN='+item.DOMAIN; 
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
			$('#domainbody').empty();	
			
			domainObj.tableViewStart();			
		}			
	   $(document).ready(function(){
	       domainObj.tableViewStart();
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
					<li class="depth01 menu03"><a href="/admin/serverState">서버 모니터링</a></li>
					<li class="depth01 menu04 active"><a href="/admin/domainList">도메인 계정 조회</a></li>
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
			<h1 class="content-title">도메인 계정 조회</h1>			    
		</div>	
		
	<fieldset class="fieldset large">
		<input type="hidden" name="pageNo" id="pageNo" value="0">
		<legend></legend>
		<div class="field">
			<div class="insert">
				<label class="label" for="input01">도메인명</label>
				<div class="input-text" style="width: 300px;"><input type="text" id="DOMAIN_VALUE" placeholder="도메인명"></div>
			</div>								
			<button type="button" class="btn type01 medium" onclick="fnSearch();">조회<span class="ico-search"></span></button>
		</div>
	</fieldset>
			
	<form name="frm" id="frm" action="/admin/domainList">	
	<input type="hidden" name="pageNo" value="${pageInfo.pageNo}" />			
	<div class="table-wrap">
		<table class="data-table highlight" id="domainList" >
			<caption>스캐줄 조회</caption>
			<colgroup>
				<col width="5%">
				<col width="10%">
				<col width="55%">				
				<col width="15%">
				<col width="15%">											
			</colgroup>
			<thead>
			   <tr>
					<th scope="col">순번</th>
					<th scope="col">등록일</th>							
					<th scope="col">도메인</th>
					<th scope="col">아이디</th>
					<th scope="col">패스워드</th>					
				</tr>
			</thead>
			<tbody id="domainbody">
				<!--데이터  -->
			</tbody>
		</table>
	</div>
	<%@ include file="/include/common/paging.jsp" %> 
	</form> 
	<div class="button-area medium">		
		<a href="/admin/domainWrite" class="btn type01 medium">등록</a>
	</div>
	</div>
</div>	
<!-- // CONTENT -->
</div>
<!-- // WRAP -->
</body>
</html>
