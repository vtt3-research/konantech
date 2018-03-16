<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>

<!-- Bootstrap core JavaScript-->
<script src="<c:url value="/webjars/jquery/1.12.4/jquery.min.js"/>"></script>
<script src="<c:url value="/webjars/jquery-ui/1.12.1/jquery-ui.min.js"/>"></script>
<script src="<c:url value="/webjars/bootstrap/4.0.0-2/js/bootstrap.bundle.min.js"/>"></script>
<script src="<c:url value="/webjars/bootbox/4.4.0/bootbox.js"/>"></script>
<script src="<c:url value="/webjars/tooltipster/4.1.4-1/js/tooltipster.bundle.min.js"/>"></script>
<script src="<c:url value="/webjars/bootpag/1.0.7/lib/jquery.bootpag.min.js"/>"></script>

<!-- Core plugin JavaScript-->
<script src="<c:url value="/resources/vendor/jquery-easing/jquery.easing.min.js"/>"></script>
<!-- Custom scripts for all pages-->
<script src="<c:url value="/resources/js/sb-admin.min.js"/>"></script>


<!-- Toast -->
<script src="<c:url value="/resources/custom/js/jquery-toast/jquery.toast.js"/>"></script>

<!-- Videojs -->
<script src="<c:url value="/resources/custom/js/videojs/video.js"/>"></script>

<!-- SVG -->
<script src="<c:url value="/resources/custom/js/jquery-svg/jquery.svg.js" />"></script>

<!-- jcanvas -->
<script src="<c:url value="/resources/custom/js/jcanvas.min.js" />"></script>

<!-- superbox -->
<script src="<c:url value="/resources/custom/js/superbox.js"/>"></script>

<!-- custom -->
<script src="<c:url value="/resources/custom/js/custom.js?_d=${pageContext.session.id}"/>"></script>

<script>
    <c:if test="${(fn:startsWith(servlet_path,'/content') && !fn:startsWith(servlet_path,'/content/write')) || (fn:startsWith(servlet_path,'/storyboard'))}">
            $(".sidenav-second-level").collapse();
    </c:if>
</script>
