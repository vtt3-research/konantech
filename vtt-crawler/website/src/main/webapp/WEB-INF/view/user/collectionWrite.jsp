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
					
			if(fnIsEmpty($('#SCHEDULE_NAME_VALUE').val())){
				$('#SCHEDULE_NAME_VALUE').focus();
				return;
			}
			
			var formOptions = {
				url : '/user/collectionSave',
				type : 'POST',
				beforeSend : function(){},
				success : function(response) {
					if(response.resultCode != RESULT_CODE.ERROR){
						alert('저장 되었습니다.');
						location.href='/user/collectionList';
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
					<span class="position"><%=userId %></span>					
				</div>
				<a href="/logout" class="logout">LOGOUT</a>				
			</div>
			<!-- // global-menu -->
			<!-- gnb -->
			<div class="gnb">
				<ul class="nav">                    
					<li class="depth01 menu01"><a href="/user/collectionList">스케줄 조회</a></li>					
					<li class="depth01 menu02 active"><a href="/user/collectionWrite">스케줄 등록</a></li>
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
			<h1 class="content-title">스케줄 등록</h1>			    
		</div>
		<div class="notice-write">
			<form name="frm" id="frm" onsubmit="return false;">
				<input type="hidden" name="USER_ID" value="<%=userId %>" />
				<div class="view-header">
					<div class="insert">
						<div class="label">스케줄명</div>					
						<div class="input-text"><input type="text" id="SCHEDULE_NAME_VALUE" name="SC_NAME"></div>
					</div>
					<div class="insert">
						<div class="label">수집 사이트</div>
						<div>
							<table>
								<tr>
									<td style="width:200px">
										<select name="SC_SITE" id="COLLECTION_SITE_VALUE"  style="width: 200px; padding-left: 10px;">
											<option value="https://www.youtube.com" >YOUTUBE</option>
										</select>									
									</td>
									<td style="width:150px"><div class="label1" style="width:110px">수집 URL</div></td>
									<td style="width:700px"><div class="input-text"><input type="text" id="SCHEDULE_URL_VALUE" name="SC_URL"></div></td>									
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
										<select name="SC_TYPE" id="COLLECTION_TYPE_VALUE"  style="width: 200px; padding-left: 10px;">
											<option value=1 >동영상</option>
										</select>										
									</td>
									<td style="width:150px"><div class="label1" style="width:110px">수집 확장자</div></td>
									<td style="width:200px">
										<div>
											<select name="SC_EXT" id="COLLECTION_EXT_VALUE"  style="width: 200px; padding-left: 10px;">
												<option value="*" >전체</option>
											</select>
										</div>
									</td>
									<td style="width:150px"><div class="label1" style="width:110px">최대 크기</div></td>
									<td style="width:200px"><div class="input-text"><input type="text" id="FILE_SIZE_VALUE" name="SC_MAXSIZE" value="50"></div></td>
									<td style="width:150px"><div class="label1">(MB)</div></td>										
								</tr>
							</table>
						</div>								
					</div>
				</div>
			</form>
			<div class="button-area medium">
				<a href="/user/collectionList" class="btn type04 medium">취소</a>
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
