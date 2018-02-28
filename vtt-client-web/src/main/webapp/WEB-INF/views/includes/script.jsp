<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>

<!-- #PLUGINS -->
<script src="<c:url value="/resources/custom/js/jquery-1.11.3.js"/>"></script>
<script src="<c:url value="/resources/js/libs/jquery-ui.min.js"/>"></script>

<!-- IMPORTANT: APP CONFIG -->
<script src="<c:url value="/resources/js/app.config.js"/>"></script>

<!-- BOOTSTRAP JS -->
<script src="<c:url value="/resources/js/bootstrap/bootstrap.min.js"/>"></script>

<!-- Toast -->
<script src="<c:url value="/resources/custom/js/jquery-toast/jquery.toast.js"/>"></script>

<!-- Videojs -->
<script src="<c:url value="/resources/custom/js/videojs/video.js"/>"></script>

<!-- SVG -->
<script src="<c:url value="/resources/custom/js/jquery-svg/jquery.svg.js" />"></script>

<!-- jcanvas -->
<script src="<c:url value="/resources/custom/js/jcanvas.min.js" />"></script>

<!-- superbox -->
<%--<script src="<c:url value="/resources/js/plugin/superbox/superbox.min.js"/>"></script>--%>
<script src="<c:url value="/resources/custom/js/superbox.js"/>"></script>

<!--[if IE 8]>
<h1>Your browser is out of date, please update your browser by going to www.microsoft.com/download</h1>
<![endif]-->

<!-- MAIN APP JS FILE -->
<script src="<c:url value="/resources/js/app.min.js"/>"></script>

<!-- CUSTOM NOTIFICATION -->
<script src="<c:url value="/resources/js/notification/SmartNotification.min.js"/>"></script>

<!-- JARVIS WIDGETS -->
<script src="<c:url value="/resources/js/smartwidgets/jarvis.widget.min.js"/>"></script>

<!-- custom -->
<script src="<c:url value="/resources/custom/js/custom.js"/>"></script>
<script src="<c:url value="/resources/custom/js/jquery.layout-1.3.0.rc30.80.js"/>"></script>
<script src="<c:url value="/resources/custom/js/bootpag/jquery.bootpag.min.js"/>"></script>

<script>

    function logout() {
        MSG.confirm("logout", "로그아웃 하겠습니까?", function () {
            location.href = "<c:url value="/user/logout"/>";
        });
    }
</script>
