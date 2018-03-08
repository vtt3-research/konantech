<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@include file="./includes/taglib.jsp" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8" />
<html>
<head>
    <title>Sample</title>
    <link rel='stylesheet' href='/webjars/bootstrap/3.3.6/css/bootstrap.min.css'>
</head>

<body>
<div class="container">
    <div>
        <h1>WebJars Starter</h1>
        <p class="lead">test</p>
    </div>

    -- ${message1} <br/>
    -- ${message2} <br/>
    -- ${message3} <br/>

    <%--<spring:message code='hello.message'/>--%>

    <%---- <fmt:bundle basename="welcome.message"> <br/>--%>
</div>

<script src="/webjars/jquery/1.12.4/jquery.min.js"></script>
<script src="/webjars/bootstrap/3.3.6/js/bootstrap.min.js"></script>
<script type="text/javascript">
    $(function () {
        console.log("jQuery ready");
    });
</script>
</body>

</html>
