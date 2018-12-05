<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>

    </section>
</div>
<!-- #PAGE FOOTER -->
<div class="page-footer">
    <div class="row">
        <div class="col-xs-12 col-sm-6">
            <span class="txt-color-white">VTT-Crowd <span class="hidden-xs"> - Web Application Framework</span> © 2018</span>
        </div>
        <div id="flag-area">
            <img src="<c:url value="/resources/custom/img/flags/blank.gif" />" data-flag="en" class="flag flag-us" alt="United States"></a>
            <img src="<c:url value="/resources/custom/img/flags/blank.gif" />" data-flag="ko" class="flag flag-kr" alt="Korea"></a>
        </div>
    </div>
    <!-- end row -->
</div>
<!-- END FOOTER -->

<!-- Modal -->
<div class="modal fade" id="baseModal" data-backdrop="static" tabindex="-1" role="dialog" aria-labelledby="baseModalLabel">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
        </div>
    </div>
</div>
<!-- Modal -->
<div class="modal fade" id="_800Modal" data-backdrop="static" tabindex="-1" role="dialog" >
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-content-wrap"></div>
        </div>
    </div>
</div>

<!-- Modal -->
<div class="modal fade" id="bigModal" data-refresh="true" tabindex="-1" role="dialog" aria-labelledby="bigModalLabel">
    <div class="modal-dialog" role="document">
        <div class="modal-content"></div>
    </div>
</div>
</body>
</html>
