<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>
<c:set var="servlet_path" scope="request"><c:out value="${ requestScope['javax.servlet.forward.servlet_path'] }"/></c:set>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="">
    <meta name="author" content="">
    <title>VTT</title>
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/jquery-ui/1.12.1/jquery-ui.min.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/bootstrap/4.0.0-2/css/bootstrap.min.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/font-awesome/4.7.0/css/font-awesome.min.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/tooltipster/4.1.4-1/css/tooltipster.bundle.min.css"/>">

    <!-- Custom styles for this template-->
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/css/sb-admin.css"/>">
    <!-- Toast -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/custom/css/jquery.toast.css"/>">
    <!-- Videojs -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/custom/css/video-js.css"/>">

    <!-- Custom -->
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/custom/css/top_left_style.css?_d=${pageContext.session.id}"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/custom/css/style.css?_d=${pageContext.session.id}"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/resources/custom/css/layout-custom.css?_d=${pageContext.session.id}"/>">

    <!-- #FAVICONS -->
    <link rel="shortcut icon" href="<c:url value="/resources/custom/img/favicon.ico"/>" type="image/x-icon">
    <link rel="icon" href="<c:url value="/resources/custom/img/favicon.ico"/>" type="image/x-icon">
</head>

<body class="fixed-nav sticky-footer bg-dark" id="page-top">

<!-- Navigation-->
<nav class="navbar navbar-expand-lg navbar-dark bg-dark fixed-top" id="mainNav">
    <button class="navbar-toggler navbar-toggler-right" type="button" data-toggle="collapse" data-target="#navbarResponsive" aria-controls="navbarResponsive" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
    </button>
    <div class="collapse navbar-collapse" id="navbarResponsive">
        <ul class="navbar-nav navbar-sidenav" id="menuAccordion">
            <li class="nav-item <c:if test="${fn:startsWith(servlet_path,'/content/write')}"> active </c:if>" data-toggle="tooltip" data-placement="right" title="콘텐츠등록">
                <a class="nav-link" href="<c:url value="/content/write"/>">
                    <i class="fa fa-fw fa-plus-square-o"></i>
                    <span class="nav-link-text">콘텐츠 등록</span>
                </a>
            </li>
            <li class="nav-item " data-toggle="tooltip" data-placement="right" title="Components">
                <a class="nav-link nav-link-collapse collapsed" data-toggle="collapse" href="#collapseComponents" data-parent="#menuAccordion">
                    <i class="fa fa-fw fa-folder-o"></i>
                    <span class="nav-link-text">콘텐츠</span>
                </a>
                <ul class="sidenav-second-level collapse in" id="collapseComponents">
                    <li class="<c:if test="${(fn:startsWith(servlet_path,'/content') && !fn:startsWith(servlet_path,'/content/write'))}"> active </c:if>" >
                        <a href="<c:url value="/content"/>">콘텐츠 리스트</a>
                    </li>
                    <li class="<c:if test="${(fn:startsWith(servlet_path,'/storyboard') && !fn:startsWith(servlet_path,'/storyboard/play')) }"> active </c:if>">
                        <a href="<c:url value="/storyboard?idx=${param.idx}"/>">스토리보드</a>
                    </li>
                    <li class="<c:if test="${(fn:startsWith(servlet_path,'/storyboard/play')) }"> active </c:if>">
                        <a href="<c:url value="/storyboard/play?idx=${param.idx}"/>">객체인식재생</a>
                    </li>
                </ul>
            </li>
        </ul>

        <ul class="navbar-nav sidenav-toggler">
            <li class="nav-item">
                <a class="nav-link text-center" id="sidenavToggler">
                    <i class="fa fa-fw fa-angle-left"></i>
                </a>
            </li>
        </ul>
        <ul class="navbar-nav ml-auto">
            <li class="nav-item">
                <a class="nav-link" data-toggle="modal" data-target="#exampleModal">
                    <i class="fa fa-fw fa-sign-out"></i>Logout</a>
            </li>
        </ul>
    </div>
</nav>

<div class="content-wrapper">