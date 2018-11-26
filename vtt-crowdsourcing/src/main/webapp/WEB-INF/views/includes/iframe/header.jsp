<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../taglib.jsp" %>

<c:set var="servlet_path" scope="request"><c:out value="${ requestScope['javax.servlet.forward.servlet_path'] }"/></c:set>
<sec:authentication var="user" property="principal" />

<!DOCTYPE html>
<html lang="ko-kr">
<head>
    <meta charset="utf-8">
    <title><spring:message code="common.title.crowd" text="VTT-Crowd!" /></title>
    <meta name="viewport" content="width=device-width,initial-scale=1.0,minimum-scale=1.0,maximum-scale=1.0"/>
    <meta name="description" content="">
    <meta name="author" content="">

    <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Roboto:400,500"  />
    <link rel="stylesheet" href="https://ajax.googleapis.com/ajax/libs/jqueryui/1.12.1/themes/smoothness/jquery-ui.css">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/bootstrap/3.3.7-1/css/bootstrap.min.css"/>">
    <link rel="stylesheet" type="text/css" href="<c:url value="/webjars/bootstrap-duallistbox/3.0.6/dist/bootstrap-duallistbox.min.css"/>">
    <link rel="stylesheet" href="<c:url value="/resources/custom/css/style.css?v1.0&_d="/>${pageContext.session.id}" />

</head>
<body>
<header>
    <h3>SELECT PICTURE</h3>
    <span class="btn_close" onclick="parent.modalClose();"></span>
</header>