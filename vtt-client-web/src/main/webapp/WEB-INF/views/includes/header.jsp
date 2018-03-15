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
    <link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/custom/css/jquery.toast.css" />" />
    <!-- Videojs -->
    <link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/custom/css/video-js.css"/>">

    <!-- Custom -->
    <link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/custom/css/top_left_style.css?_d="/>${pageContext.session.id}"/>">
    <link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/custom/css/style.css?_d="/>${pageContext.session.id}" />
    <link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/custom/css/layout-custom.css?_d="/>${pageContext.session.id}"/>">

    <!-- #FAVICONS -->
    <link rel="shortcut icon" href="<c:url value="/resources/custom/img/favicon.ico"/>" type="image/x-icon">
    <link rel="icon" href="<c:url value="/resources/custom/img/favicon.ico"/>" type="image/x-icon">

</head>

<body class="fixed-nav sticky-footer bg-dark" id="page-top">

<!-- Navigation-->
<nav class="navbar navbar-expand-lg navbar-dark bg-dark fixed-top" id="mainNav">
    <a class="navbar-brand" href="javascript:void(0);">
        <img src="<c:url value="/resources/custom/img/Konan_CI.png"/>" style="width:113px;" >
        </a>
    <button class="navbar-toggler navbar-toggler-right" type="button" data-toggle="collapse" data-target="#navbarResponsive" aria-controls="navbarResponsive" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
    </button>
    <div class="collapse navbar-collapse" id="navbarResponsive">
        <ul class="navbar-nav navbar-sidenav" id="exampleAccordion">
            <li class="nav-item <c:if test="${fn:startsWith(servlet_path,'/content/write')}"> active </c:if>" data-toggle="tooltip" data-placement="right" title="Dashboard">

                <%--<li class="<c:if test="${fn:startsWith(servlet_path,'/content')}"> active </c:if>">--%>
                <%--<a href="<c:url value="/content"/>" title="blank_"><i class="fa fa-lg fa-fw fa-arrow-circle-o-right"></i>--%>
                <%--<span class="menu-item-parent">콘텐츠 리스트</span></a>--%>
                <%--</li>--%>

                <a class="nav-link" href="<c:url value="/content/write"/>">
                    <i class="fa fa-fw fa-plus-square-o"></i>
                    <span class="nav-link-text">콘텐츠 등록</span>
                </a>
            </li>
            <li class="nav-item " data-toggle="tooltip" data-placement="right" title="Components">
                <a class="nav-link nav-link-collapse collapsed" data-toggle="collapse" href="#collapseComponents" data-parent="#exampleAccordion">
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
                <form class="form-inline my-2 my-lg-0 mr-lg-2">
                    <div class="input-group">
                        <input class="form-control" type="text" placeholder="Search for...">
                        <span class="input-group-btn">
                <button class="btn btn-primary" type="button">
                  <i class="fa fa-search"></i>
                </button>
              </span>
                    </div>
                </form>
            </li>
            <li class="nav-item">
                <a class="nav-link" data-toggle="modal" data-target="#exampleModal">
                    <i class="fa fa-fw fa-sign-out"></i>Logout</a>
            </li>
        </ul>
    </div>
</nav>

<div class="content-wrapper">











<%--<!DOCTYPE html>--%>
<%--<html lang="ko-kr">--%>
<%--<head>--%>
    <%--<meta charset="utf-8">--%>
    <%--<title>VTT</title>--%>
    <%--<meta name="description" content="">--%>
    <%--<meta name="author" content="">--%>

    <%--<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">--%>

    <%--<!-- #CSS Links -->--%>
    <%--<!-- Basic Styles -->--%>
    <%--<link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/css/bootstrap.min.css"/>">--%>
    <%--<link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/css/font-awesome.min.css"/>">--%>

    <%--<!-- SmartAdmin Styles : Caution! DO NOT change the order -->--%>
    <%--<link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/css/smartadmin-production-plugins.min.css"/>">--%>
    <%--<link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/css/smartadmin-production.min.css"/>">--%>
    <%--<link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/css/smartadmin-skins.min.css"/>">--%>

    <%--<!-- Toast -->--%>
    <%--<link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/custom/css/jquery.toast.css" />" />--%>

    <%--<!-- Videojs -->--%>
    <%--<link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/custom/css/video-js.css"/>">--%>

    <%--<!-- custom style -->--%>
    <%--<link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/custom/css/top_left_style.css"/>">--%>
    <%--<link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/custom/css/style.css?_d="/>${pageContext.session.id}" />--%>
    <%--<link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/custom/css/layout-custom.css"/>">--%>


<%--</head>--%>

<%--<body class="">--%>
<%--<!-- #HEADER -->--%>
<%--<header id="header">--%>
    <%--<div id="logo-group">--%>

        <%--<!-- PLACE YOUR LOGO HERE -->--%>
        <%--<span id="logo"> <img src="<c:url value="/resources/custom/img/Konan_CI.png"/>" alt="KBS"> </span>--%>
        <%--<!-- END LOGO PLACEHOLDER -->--%>

        <%--<!-- Note: The activity badge color changes when clicked and resets the number to 0--%>
             <%--Suggestion: You may want to set a flag when this happens to tick off all checked messages / notifications -->--%>
        <%--<span id="activity" class="activity-dropdown"> <i class="fa fa-user"></i> <b class="badge"> 21 </b> </span>--%>

        <%--<!-- AJAX-DROPDOWN : control this dropdown height, look and feel from the LESS variable file -->--%>
        <%--<div class="ajax-dropdown">--%>

            <%--<!-- the ID links are fetched via AJAX to the ajax container "ajax-notifications" -->--%>
            <%--<div class="btn-group btn-group-justified" data-toggle="buttons">--%>
                <%--<label class="btn btn-default">--%>
                    <%--<input type="radio" name="activity" id="ajax/notify/mail.html">--%>
                    <%--Msgs (14) </label>--%>
                <%--<label class="btn btn-default">--%>
                    <%--<input type="radio" name="activity" id="ajax/notify/notifications.html">--%>
                    <%--notify (3) </label>--%>
                <%--<label class="btn btn-default">--%>
                    <%--<input type="radio" name="activity" id="ajax/notify/tasks.html">--%>
                    <%--Tasks (4) </label>--%>
            <%--</div>--%>

            <%--<!-- notification content -->--%>
            <%--<div class="ajax-notifications custom-scroll">--%>

                <%--<div class="alert alert-transparent">--%>
                    <%--<h4>Click a button to show messages here</h4>--%>
                    <%--This blank page message helps protect your privacy, or you can show the first message here automatically.--%>
                <%--</div>--%>

                <%--<i class="fa fa-lock fa-4x fa-border"></i>--%>

            <%--</div>--%>
            <%--<!-- end notification content -->--%>

            <%--<!-- footer: refresh area -->--%>
            <%--<span> Last updated on: 12/12/2013 9:43AM--%>
						<%--<button type="button" data-loading-text="<i class='fa fa-refresh fa-spin'></i> Loading..." class="btn btn-xs btn-default pull-right">--%>
							<%--<i class="fa fa-refresh"></i>--%>
						<%--</button> </span>--%>
            <%--<!-- end footer -->--%>

        <%--</div>--%>
        <%--<!-- END AJAX-DROPDOWN -->--%>
    <%--</div>--%>

    <%--<!-- #PROJECTS: projects dropdown -->--%>
    <%--<div class="project-context hidden-xs">--%>

        <%--<span class="label">Projects:</span>--%>
        <%--<span class="project-selector dropdown-toggle" data-toggle="dropdown">Recent projects <i class="fa fa-angle-down"></i></span>--%>

        <%--<!-- Suggestion: populate this list with fetch and push technique -->--%>
        <%--<ul class="dropdown-menu">--%>
            <%--<li>--%>
                <%--<a href="javascript:void(0);">Online e-merchant management system - attaching integration with the iOS</a>--%>
            <%--</li>--%>
            <%--<li>--%>
                <%--<a href="javascript:void(0);">Notes on pipeline upgradee</a>--%>
            <%--</li>--%>
            <%--<li>--%>
                <%--<a href="javascript:void(0);">Assesment Report for merchant account</a>--%>
            <%--</li>--%>
            <%--<li class="divider"></li>--%>
            <%--<li>--%>
                <%--<a href="javascript:void(0);"><i class="fa fa-power-off"></i> Clear</a>--%>
            <%--</li>--%>
        <%--</ul>--%>
        <%--<!-- end dropdown-menu-->--%>

    <%--</div>--%>
    <%--<!-- end projects dropdown -->--%>

    <%--<!-- #TOGGLE LAYOUT BUTTONS -->--%>
    <%--<!-- pulled right: nav area -->--%>
    <%--<div class="pull-right">--%>

        <%--<!-- collapse menu button -->--%>
        <%--<div id="hide-menu" class="btn-header pull-right">--%>
            <%--<span> <a href="javascript:void(0);" data-action="toggleMenu" title="Collapse Menu"><i class="fa fa-reorder"></i></a> </span>--%>
        <%--</div>--%>
        <%--<!-- end collapse menu -->--%>

        <%--<!-- #MOBILE -->--%>
        <%--<!-- Top menu profile link : this shows only when top menu is active -->--%>
        <%--<ul id="mobile-profile-img" class="header-dropdown-list hidden-xs padding-5">--%>
            <%--<li class="">--%>
                <%--<a href="#" class="dropdown-toggle no-margin userdropdown" data-toggle="dropdown">--%>
                    <%--<img src="<c:url value="/resources/img/avatars/sunny.png"/>" alt="John Doe" class="online" />--%>
                <%--</a>--%>
                <%--<ul class="dropdown-menu pull-right">--%>
                    <%--<li>--%>
                        <%--<a href="javascript:void(0);" class="padding-10 padding-top-0 padding-bottom-0"><i class="fa fa-cog"></i> Setting</a>--%>
                    <%--</li>--%>
                    <%--<li class="divider"></li>--%>
                    <%--<li>--%>
                        <%--<a href="#ajax/profile.html" class="padding-10 padding-top-0 padding-bottom-0"> <i class="fa fa-user"></i> <u>P</u>rofile</a>--%>
                    <%--</li>--%>
                    <%--<li class="divider"></li>--%>
                    <%--<li>--%>
                        <%--<a href="javascript:void(0);" class="padding-10 padding-top-0 padding-bottom-0" data-action="toggleShortcut"><i class="fa fa-arrow-down"></i> <u>S</u>hortcut</a>--%>
                    <%--</li>--%>
                    <%--<li class="divider"></li>--%>
                    <%--<li>--%>
                        <%--<a href="javascript:void(0);" class="padding-10 padding-top-0 padding-bottom-0" data-action="launchFullscreen"><i class="fa fa-arrows-alt"></i> Full <u>S</u>creen</a>--%>
                    <%--</li>--%>
                    <%--<li class="divider"></li>--%>
                    <%--<li>--%>
                        <%--<a href="login.html" class="padding-10 padding-top-5 padding-bottom-5" data-action="userLogout"><i class="fa fa-sign-out fa-lg"></i> <strong><u>L</u>ogout</strong></a>--%>
                    <%--</li>--%>
                <%--</ul>--%>
            <%--</li>--%>
        <%--</ul>--%>

        <%--<!-- logout button -->--%>
        <%--<div id="logout" class="btn-header transparent pull-right">--%>
            <%--<span> <a href="javascript:logout();" title="Sign Out" data-action="userLogout" data-logout-msg="로그아웃"><i class="fa fa-sign-out"></i></a> </span>--%>
        <%--</div>--%>
        <%--<!-- end logout button -->--%>

        <%--<!-- search mobile button (this is hidden till mobile view port) -->--%>
        <%--<div id="search-mobile" class="btn-header transparent pull-right">--%>
            <%--<span> <a href="javascript:void(0)" title="Search"><i class="fa fa-search"></i></a> </span>--%>
        <%--</div>--%>
        <%--<!-- end search mobile button -->--%>

        <%--<!-- #SEARCH -->--%>
        <%--<!-- input: search field -->--%>
        <%--<form action="#ajax/search.html" class="header-search pull-right">--%>
            <%--<input id="search-fld" type="text" name="param" placeholder="검색">--%>
            <%--<button type="submit">--%>
                <%--<i class="fa fa-search"></i>--%>
            <%--</button>--%>
            <%--<a href="javascript:void(0);" id="cancel-search-js" title="Cancel Search"><i class="fa fa-times"></i></a>--%>
        <%--</form>--%>
        <%--<!-- end input: search field -->--%>

        <%--<!-- fullscreen button -->--%>
        <%--<div id="fullscreen" class="btn-header transparent pull-right">--%>
            <%--<span> <a href="javascript:void(0);" data-action="launchFullscreen" title="Full Screen"><i class="fa fa-arrows-alt"></i></a> </span>--%>
        <%--</div>--%>
        <%--<!-- end fullscreen button -->--%>

        <%--<!-- multiple lang dropdown : find all flags in the flags page -->--%>
        <%--<ul class="header-dropdown-list hidden-xs">--%>
            <%--<li>--%>
                <%--<a href="#" class="dropdown-toggle" data-toggle="dropdown"> <img src="<c:url value="/resources/img/blank.gif"/>" class="flag flag-kr" alt="Korea"> <span> 한국어</span> <i class="fa fa-angle-down"></i> </a>--%>
                <%--<ul class="dropdown-menu pull-right">--%>
                    <%--<li class="active">--%>
                        <%--<a href="javascript:void(0);"><img src="<c:url value="/resources/img/blank.gif"/>" class="flag flag-kr" alt="Korea"> 한국어</a>--%>
                    <%--</li>--%>
                    <%--<li>--%>
                        <%--<a href="javascript:void(0);"><img src="<c:url value="/resources/img/blank.gif"/>" class="flag flag-us" alt="United States"> English (US)</a>--%>
                    <%--</li>--%>
                <%--</ul>--%>
            <%--</li>--%>
        <%--</ul>--%>
        <%--<!-- end multiple lang -->--%>

    <%--</div>--%>
    <%--<!-- end pulled right: nav area -->--%>

<%--</header>--%>
<%--<!-- END HEADER -->--%>

<%--<!-- #NAVIGATION -->--%>
<%--<aside id="top-panel">--%>
    <%--<nav>--%>
        <%--<ul>--%>
            <%--<li class="<c:if test="${fn:startsWith(servlet_path,'/content/write')}"> active </c:if>">--%>
                <%--<a href="<c:url value="/content/write"/>" title="blank_"><i class="fa fa-lg fa-fw fa-plus-square-o"></i>--%>
                    <%--<span class="menu-item-parent">콘텐츠 등록</span></a>--%>
            <%--</li>--%>
            <%--<li class="<c:if test="${(fn:startsWith(servlet_path,'/content') && !fn:startsWith(servlet_path,'/content/write')) || fn:startsWith(servlet_path,'/storyboard') }"> active </c:if>">--%>
                <%--<a href="<c:url value="/content"/>" title="blank_"><i class="fa fa-lg fa-fw fa-folder-o"></i>--%>
                    <%--<span class="menu-item-parent">콘텐츠</span></a>--%>
            <%--</li>--%>
            <%--&lt;%&ndash;<li class="<c:if test="${fn:startsWith(servlet_path,'/monitoring')}"> active </c:if>">&ndash;%&gt;--%>
                <%--&lt;%&ndash;<a href="<c:url value="/monitoring"/>" title="blank_"><i class="fa fa-lg fa-fw fa-line-chart"></i>&ndash;%&gt;--%>
                    <%--&lt;%&ndash;<span class="menu-item-parent">모니터링</span></a>&ndash;%&gt;--%>
            <%--&lt;%&ndash;</li>&ndash;%&gt;--%>
        <%--</ul>--%>
    <%--</nav>--%>
    <%--<span class="minifyme" data-action="minifyMenu"> <i class="fa fa-arrow-circle-left hit"></i> </span>--%>
<%--</aside>--%>
<%--<!-- END NAVIGATION -->--%>
