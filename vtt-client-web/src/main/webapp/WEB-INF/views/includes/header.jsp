<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>
<c:set var="servlet_path" scope="request"><c:out value="${ requestScope['javax.servlet.forward.servlet_path'] }"/></c:set>
<sec:authentication var="user" property="principal" />
<!DOCTYPE html>
<html lang="en">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <!-- Meta, title, CSS, favicons, etc. -->
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="<c:url value="/resources/custom/img/favicon.ico" />" type="image/ico" />

    <title> VTT </title>

    <!-- Bootstrap -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/bootstrap/3.3.6/css/bootstrap.min.css"/>">
    <!-- Font Awesome -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/font-awesome/4.7.0/css/font-awesome.min.css"/>">
    <!-- NProgress -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/nprogress/0.2.0/nprogress.css"/>">
    <!-- iCheck -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/iCheck/1.0.2/skins/flat/green.css"/>">
    <!-- bootstrap-progressbar -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/bootstrap-progressbar/0.9.0/css/bootstrap-progressbar-3.3.4.min.css"/>">
    <!-- bootstrap-daterangepicker -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/bootstrap-daterangepicker/2.1.24/css/bootstrap-daterangepicker.css"/>">
    <!-- Custom Theme Style -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/iCheck/1.0.2/skins/flat/green.css"/>">

    <!-- Custom Theme Style -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/css/custom.min.css"/>" rel="stylesheet">

    <!-- Custom Theme Style -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/custom/css/j-css.css?_d=${pageContext.session.id}"/>" rel="stylesheet">

    <!-- Toast -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/custom/css/jquery.toast.css"/>">
<%-- selectarea --%>
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/css/jquery.selectareas.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/custom/css/style.css?_d=${pageContext.session.id}"/>">
    <!-- Videojs -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/custom/css/video-js.css"/>">
    <!-- dropzone style -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/custom/js/dropzone/dropzone.min.css"/>">


</head>

<body class="nav-md">
<div class="container body">
    <div class="main_container">

        <header class="topbar">
            <nav class="navbar top-navbar navbar-toggleable-sm navbar-light">
                <div class="navbar-header">
                    <a class="navbar-brand" href="<c:url value="/"/>">
                        <b>
                            <img src="<c:url value="/resources/custom/img/konan_ci.png"/>" alt="homepage" class="light-logo" />
                           <%-- <img src="<c:url value="/resources/custom/img/vtt-logo.png"/>" alt="homepage" class="light-logo" />--%>
                        </b>
                    </a>
                </div>
                <div class="navbar-collapse">
                    <ul class="nav navbar-nav navbar-right">
                        <li class="">
                            <a href="javascript:;" class="user-profile dropdown-toggle" data-toggle="dropdown" aria-expanded="false">
                                ${user.username}
                                <span class=" fa fa-angle-down"></span>
                            </a>
                            <ul class="dropdown-menu dropdown-usermenu pull-right">
                                <li>
                                    <a class="nav-link" data-toggle="modal" data-target="#addUserModal">
                                        사용자 추가
                                    </a>
                                </li>
                                <li>
                                    <a class="nav-link" data-toggle="modal" data-target="#exampleModal">
                                        <i class="fa fa-sign-out pull-right"></i> Log Out
                                    </a>
                                </li>
                            </ul>
                        </li>
                    </ul>
                </div>

            </nav>
        </header>



        <div class="col-md-3 left_col">
            <div class="left_col scroll-view">
                <div class="navbar nav_title"></div>
                <div class="clearfix"></div>

                <!-- sidebar menu -->
                <div id="sidebar-menu" class="main_menu_side hidden-print main_menu">
                    <div class="menu_section">
                        <h3>CONTENT</h3>
                        <ul class="nav side-menu">
                            <li>
                                <a>
                                    <i class="fa fa-clone"></i>
                                    콘텐츠
                                    <span class="fa fa-chevron-down">
                            </span>
                                </a>
                                <ul class="nav child_menu">
                                    <li><a href="<c:url value="/content"/>"> 콘텐츠 리스트 </a></li>
                                    <li><a href="<c:url value="/storyboard?idx=${param.idx}"/>"> 스토리 보드 </a></li>
                                    <li><a href="<c:url value="/storyboard/play?idx=${param.idx}"/>"> 객체인식 재생 </a></li>
                                </ul>
                            </li>
                        </ul>
                    </div>

                </div>
                <!-- /sidebar menu -->
            </div>
        </div>