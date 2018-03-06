<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@include file="../includes/taglib.jsp" %>

<!DOCTYPE html>
<html lang="en-us" id="extr-page">
<head>
    <meta charset="utf-8">
    <title>DEMO</title>
    <meta name="description" content="">
    <meta name="author" content="">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">

    <!-- #CSS Links -->
    <!-- Basic Styles -->
    <link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/css/bootstrap.min.css" />"/>
    <link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/css/font-awesome.min.css" />"/>

    <!-- SmartAdmin Styles : Caution! DO NOT change the order -->
    <link rel="stylesheet" type="text/css" media="screen"
          href="<c:url value="/resources/css/smartadmin-production-plugins.min.css" />"/>
    <link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/css/smartadmin-production.min.css" />"/>
    <link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/css/smartadmin-skins.min.css" />"/>

    <!-- SmartAdmin RTL Support -->
    <link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/css/smartadmin-rtl.min.css" />"/>

    <!-- We recommend you use "your_style.css" to override SmartAdmin
         specific styles this will also ensure you retrain your customization with each SmartAdmin update.
    <link rel="stylesheet" type="text/css" media="screen" href="css/your_style.css"> -->

    <!-- Demo purpose only: goes with demo.js, you can delete this css when designing your own WebApp -->
    <link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/css/demo.min.css" />"/>

    <!-- CUSTOM CSS -->
    <link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/custom/css/style.css" />"/>

    <!-- #FAVICONS -->
    <link rel="shortcut icon" type="image/ico" href="<c:url value="/resources/custom/img/favicon.ico" />"/>
    <link rel="icon" href="<c:url value="/resources/custom/img/favicon.ico" />" type="image/x-icon">

</head>

<body class="animated fadeInDown">
<header id="header">
    <div id="logo-group" style="display: block; vertical-align: middle; float: none;">
        <span id="logo" style="text-align: center; width: 100%; font-weight: bold; font-size: 17px;">DEMO</span>
    </div>
</header>

<div id="main" role="main">
    <!-- MAIN CONTENT -->
    <div id="content" class="container">
        <div class="row">
            <div class="well no-padding login">
                <form id="login-form" class="smart-form client-form" method="post">
                    <input type="hidden" name="${_csrf.parameterName}" value="${_csrf.token}"/>
                    <header>
                        DEMO
                    </header>

                    <fieldset>
                        <section>
                            <label class="label"><spring:message code="ID" text="ID"/></label>
                            <label class="input"> <i class="icon-append fa fa-user"></i>
                                <input type="text" name="username" value="konan">
                                <b class="tooltip tooltip-top-right"><i class="fa fa-user txt-color-teal"></i>
                                    <spring:message code="ID" text="ID"/></b>
                            </label>
                        </section>

                        <section>
                            <label class="label"><spring:message code="password" text="패스워드"/></label>
                            <label class="input"> <i class="icon-append fa fa-lock"></i>
                                <input type="password" name="password" value="konan1">
                                <b class="tooltip tooltip-top-right"><i class="fa fa-lock txt-color-teal"></i>
                                    <spring:message code="password" text="패스워드"/></b>
                            </label>
                        </section>

                        <section>
                            <label class="checkbox">
                                <input type="checkbox" name="remember" checked="">
                                <i></i><spring:message code="login.save.id" text="아이디 저장"/></label>
                        </section>

                        <section>
                            <div class="error" id="err_common" style="display:none;color:red;">
                                <p>
                                    <spring:message code="login.check" text="ID 또는 비밀번호를 다시 확인하세요."></spring:message><br>
                                    <spring:message code="login.wrong" text="등록되지 않은 ID이거나, 아이디 또는 비밀번호를 잘못 입력하셨습니다."/></p>
                            </div>

                            <div class="error" id="logout_common" style="display:none;color:red;">
                                <p><spring:message code="logout.completed" text="로그아웃 되었습니다."/></p>
                            </div>
                        </section>
                    </fieldset>

                    <footer>
                        <button class="btn btn-primary" style="text-align: center; float: none; font-family: 'Custom1 Gothic'; min-width: 140px;">
                            <spring:message code="login" text="로그인"/>
                        </button>
                    </footer>
                </form>
            </div>
        </div>
    </div>
</div>

<!-- PACE LOADER - turn this on if you want ajax loading to show (caution: uses lots of memory on iDevices)-->
<script src="<c:url value="/resources/js/plugin/pace/pace.min.js" />"></script>
<script src="<c:url value="/resources/js/libs/jquery-3.2.1.min.js"/>"></script>

<!-- IMPORTANT: APP CONFIG -->
<script src="<c:url value="/resources/js/app.config.js" />"></script>

<!-- JS TOUCH : include this plugin for mobile drag / drop touch events
<script src="js/plugin/jquery-touch/jquery.ui.touch-punch.min.js"></script> -->

<!-- BOOTSTRAP JS -->
<script src="<c:url value="/resources/js/bootstrap/bootstrap.min.js"/>"></script>

<!-- JQUERY VALIDATE -->
<script src="<c:url value="/resources/js/plugin/jquery-validate/jquery.validate.min.js"/>"></script>

<!-- JQUERY MASKED INPUT -->
<script src="<c:url value="/resources/js/plugin/masked-input/jquery.maskedinput.min.js"/>"></script>

<!--[if IE 8]>
<h1>Your browser is out of date, please update your browser by going to www.microsoft.com/download</h1>
<![endif]-->

<!-- MAIN APP JS FILE -->
<script src="<c:url value="/resources/js/app.min.js" />"></script>

<script type="text/javascript">
    runAllForms();

    $(function () {

        <c:if test="${param.error != null}">
        showErrorMsg();
        </c:if>

        <c:if test="${ param.logout != null}">
        showLogoutMsg();
        </c:if>

        // Validation
        $("#login-form").validate({
            // Rules for form validation
            rules: {
                email: {
                    required: true,
                    email: true
                },
                password: {
                    required: true,
                    minlength: 3,
                    maxlength: 20
                }
            },

            // Messages for form validation
            messages: {
                email: {
                    required: '<spring:message code="login.input.id" text="ID를 입력해주세요."/>'
                },
                password: {
                    required: '<spring:message code="login.input.password" text="비밀번호를 입력해주세요."/>'
                }
            },

            // Do not change code below
            errorPlacement: function (error, element) {
                error.insertAfter(element.parent());
            }
        });

        $("#login-form").submit(function () {
            if ($("#login-form").valid() == false) {
                return false;
            }
            return true;
        });

        // Login Error
        function showErrorMsg() {
            $("#err_common").show();

            $("input[name=username]").val("");
            $("input[name=password]").focus();

            setTimeout(function () {
                $("#err_common").hide();
            }, 2000);
        }

        // Logout
        function showLogoutMsg() {
            $("#logout_common").show();
            setTimeout(function () {
                $("#logout_common").hide();
            }, 2000)
        }
    });
</script>
</body>
</html>