<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ include file="/include/common/taglibs.jsp" %>

<html>
<head>
	<title>KONAN CRAWLER</title>
	<base href="/" />
	<link rel="stylesheet" type="text/css" href="../../resources/css/layout.css">
	<link rel="stylesheet" type="text/css" href="../../resources/css/common.css">
	<link rel="stylesheet" type="text/css" href="../../resources/css/base.css">
		
	<script type="text/javascript" src="../../resources/js/jquery-3.3.1.min.js"></script>	
	<script type="text/javascript" src="../../resources/js/jquery.form-3.51.0.js"></script>
	<script type="text/javascript" src="../../resources/js/jquery-ui.min.js"></script>
	<script type="text/javascript">
	function fnLogin() {
		$('#loginFrm').submit();
	}
	
	function enter() {
		if(window.event.keyCode == 13) {
			fnLogin();
		}
	}
	</script>
	<c:if test='${not empty SPRING_SECURITY_LAST_EXCEPTION}'>
	<script type="text/javascript">
	$(document).ready(function(){
		alert('${sessionScope["SPRING_SECURITY_LAST_EXCEPTION"].message}');
	});
	</script>
	<c:remove scope="session" var="SPRING_SECURITY_LAST_EXCEPTION"/>
	</c:if>	
</head>
<body>
<div id="wrap" class="login-wrap">
<!-- CONTENT -->
<div id="container" class="login" align="center">
	<h1 class="login-header"></h1>
	<div id="content" class="content-primary">
		<form name="loginFrm" id="loginFrm" action="/home/loginProcess" method="POST">
			<fieldset class="fieldset">
				<legend>LOGIN</legend>
				<div class="field">
					<div class="insert id">
						<label for="userId" class="blind">ID</label>
						<div class="input-text"><input type="text" name="userId" placeholder="ID" value=""></div>
					</div>
					<div class="insert password">
						<label for="userPw" class="blind">Password</label>
						<div class="input-text"><input type="password" name="userPassword" placeholder="Password" value="" onkeyup="enter();"></div>
					</div>
					<div class="text-wrap">
					</div>
				</div>
				<input type="button" value="LOGIN" class="btn large type01" onclick="fnLogin()" style="">
			</fieldset>
		</form>
	</div>
</div>
<!-- // CONTENT -->
</div>
</body>
			
</body>
</html>
