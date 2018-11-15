<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ include file="/include/common/taglibs.jsp" %>

<html>
<head>
	<title>KONAN CRAWLER</title>
	<link rel="stylesheet" type="text/css" href="../../resources/css/layout.css">
	<link rel="stylesheet" type="text/css" href="../../resources/css/common.css">
	<link rel="stylesheet" type="text/css" href="../../resources/css/base.css">
	
	<script type="text/javascript" src="../../resources/js/jquery-3.3.1.min.js"></script>	
	<script type="text/javascript" src="../../resources/js/jquery.form-3.51.0.js"></script>
	<script type="text/javascript" src="../../resources/js/jquery-ui.min.js"></script>	
	<script type="text/javascript" src="../../resources/js/common.js"></script>
	<script type="text/javascript">
		function fnSave(){
					
			if(fnIsEmpty($('#DOMAIN_NAME_VALUE').val())){
				$('#DOMAIN_NAME_VALUE').focus();
				return;
			}
			
			var formOptions = {
				url : '/admin/domainSave',
				type : 'POST',
				beforeSend : function(){},
				success : function(response) {
					if(response.resultCode != RESULT_CODE.ERROR){
						alert('저장 되었습니다.');
						location.href='/admin/domainList';
					}else{
						alert(response.resultMessage);
					}
				}
			};
			$("#frm").ajaxForm(formOptions).submit();
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
			<h1 class="content-title">도메인 계정 등록</h1>			    
		</div>
		
		<div class="notice-write">
			<form name="frm" id="frm" onsubmit="return false;">				
				<input type="hidden" name="INPUT_ID" value="<%=userId %>" />
				<div class="view-header">
					<div class="insert">
						<div class="label">도메인명</div>					
						<div class="input-text"><input type="text" id="DOMAIN_NAME_VALUE" name="DOMAIN"></div>
					</div>	
					<div class="insert">
						<div class="label">아이디</div>					
						<div>
							<table>
								<tr>
									<td style="width:450px">
										<div class="input-text"><input type="text" id="DOMAIN_ID_VALUE" name="USER_ID"></div>										
									</td>
									<td style="width:150px"><div class="label1" style="width:110px">패스워드</div></td>
									<td style="width:450px">
										<div class="input-text"><input type="text" id="DOMAIN_PASS_VALUE" name="USER_PASS"></div>										
									</td>																			
								</tr>
							</table>
						</div>
					</div>
				</div>												
			</form>
			<div class="button-area medium">
				<a href="/admin/domainList" class="btn type04 medium">취소</a>
				<a href="javascript:fnSave();" class="btn type01 medium">등록</a>
			</div>
		</div>				
	</div>
</div>
<!-- // CONTENT -->
</div>
<!-- // WRAP -->
</body>
</html>
