<%@ page import="org.springframework.security.core.context.SecurityContextHolder" %>
<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>

<c:set var="servlet_path" scope="request"><c:out value="${ requestScope['javax.servlet.forward.servlet_path'] }"/></c:set>
<sec:authentication var="user" property="principal" />

<!DOCTYPE html>
<html lang="ko-kr">
<head>
    <meta charset="utf-8">
    <title><spring:message code="common.title.crowd" text="VTT-Crowd" /></title>
    <meta name="description" content="">
    <meta name="author" content="">

    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/jquery-ui/1.12.1/jquery-ui.min.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/bootstrap/3.3.7-1/css/bootstrap.min.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/font-awesome/5.1.0/css/all.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/font-awesome/4.7.0/css/font-awesome.min.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/tooltipster/4.1.4-1/css/tooltipster.bundle.min.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/bootstrap-duallistbox/3.0.6/dist/bootstrap-duallistbox.min.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/bootstrap-daterangepicker/2.1.27/daterangepicker.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/font-awesome-animation/0.1.0/dist/font-awesome-animation.min.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/jstree/3.3.5/dist/themes/default/style.min.css"/>">

    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/ion.rangeSlider/2.2.0/css/ion.rangeSlider.skinFlat.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/nouislider/11.0.0/distribute/nouislider.min.css"/>">

    <!-- custom style -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/custom/css/vtt-crowd.css?_d="/>${pageContext.session.id}" />


    <!-- #FAVICONS -->
    <link rel="shortcut icon" href="<c:url value="/resources/custom/img/favicon.ico"/>" type="image/x-icon">
    <link rel="icon" href="<c:url value="/resources/custom/img/favicon.ico"/>" type="image/x-icon">

</head>

<body class="menu-on-top fixed-page-footer ">
    <!-- #HEADER -->
    <header id="header">
        <div id="logo-group">
            <!-- PLACE YOUR LOGO HERE -->
            <span id="logo"><img src="<c:url value="/resources/custom/img/konan_ci.png" />"></span>
            <!-- END LOGO PLACEHOLDER -->
        </div>
        <div class="pull-right"></div>
    </header>
    <!-- END HEADER -->
<div id="content">
    <section id="widget-grid" class="">
        <div class="content row">
            <article class="col-xs-12 col-sm-12 col-md-4 col-lg-2 col-margin-small__">
                <div id="content-left">
                    <!-- sidebar menu -->
                    <div id="sidebar-menu" class="jarviswidget jarviswidget-color-darken">
                        <header>Vtt-Crowd</header>
                        <div>
                            <ul class="nav side-menu">
                                <li>
                                    <a>
                                        <i class="fa fa-clone"></i>
                                        문제 iframe
                                        <span class="fa fa-chevron-down"></span>
                                    </a>
                                    <ul class="nav child_menu">
                                        <li><a href="<c:url value="/main?type=object"/>"> Object</a></li>
                                        <li><a href="<c:url value="/main?type=background"/>"> Background</a></li>
                                        <li><a href="<c:url value="/main?type=drag"/>"> Drag</a></li>
                                    </ul>
                                </li>
                            </ul>
                        </div>
                    </div>
                    <!-- /sidebar menu -->
                </div>
            </article>