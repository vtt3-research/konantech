<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>

</div>


<!-- Logout Modal-->
<div class="modal fade" id="exampleModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel">로그아웃 하겠습니까?</h5>
                <button class="close" type="button" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">×</span>
                </button>
            </div>
            <div class="modal-footer">
                <button class="btn btn-secondary" type="button" data-dismiss="modal">취소</button>
                <a class="btn btn-primary" href="<c:url value="/user/logout"/>">로그아웃</a>
            </div>
        </div>
    </div>
</div>



</body>
</html>


<%--<!-- #PAGE FOOTER -->--%>
<%--<div class="page-footer">--%>
    <%--<div class="row">--%>
        <%--<div class="col-xs-12 col-sm-6">--%>
            <%--<span class="txt-color-white">DEMO <span class="hidden-xs"> - Web Application Framework</span> © 2018</span>--%>
        <%--</div>--%>
    <%--</div>--%>
    <%--<!-- end row -->--%>
<%--</div>--%>
<%--<!-- END FOOTER -->--%>

<%--<!-- #SHORTCUT AREA : With large tiles (activated via clicking user name tag)--%>
<%--Note: These tiles are completely responsive, you can add as many as you like -->--%>
<%--<div id="shortcut">--%>
    <%--<ul>--%>
        <%--<li>--%>
            <%--<a href="index.html" class="jarvismetro-tile big-cubes bg-color-blue"> <span class="iconbox"> <i class="fa fa-envelope fa-4x"></i> <span>Mail <span class="label pull-right bg-color-darken">14</span></span> </span> </a>--%>
        <%--</li>--%>
    <%--</ul>--%>
<%--</div>--%>
<%--<!-- END SHORTCUT AREA -->--%>

<%--</body>--%>
<%--</html>--%>
