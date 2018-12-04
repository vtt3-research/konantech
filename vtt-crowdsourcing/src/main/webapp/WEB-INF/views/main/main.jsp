<%--
  Created by IntelliJ IDEA.
  User: jeunghakmoon
  Date: 2018. 10. 31.
  Time: 오후 4:16
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@include file="../includes/taglib.jsp" %>
<c:import url="../includes/header.jsp"/>

<article class="col-xs-12 col-sm-12 col-md-8 col-lg-8 col-margin-small ">
    <div id="content-right">
        <div class="jarviswidget jarviswidget-color-darken">
            <header>Iframe Sample</header>
            <div>
            <c:if test = "${type=='object'}">
                <iframe src="<c:url value="/crowd/object?site=sugang&userid=konan"/>" style="width:600px; height:650px;" scrolling="no" ></iframe>
            </c:if>
            <c:if test = "${type=='background'}">
                <iframe src="<c:url value="/crowd/background?site=sugang&userid=konan"/>" style="width:600px; height:530px;" scrolling="no" ></iframe>
            </c:if>
            <c:if test = "${type=='drag'}">
                <iframe src="<c:url value="/crowd/drag?site=sugang&userid=konan"/>" style="width:600px; height:820px;" scrolling="no" ></iframe>
            </c:if>
            </div>
        </div>
    </div>
    <div >
        <div class="jarviswidget jarviswidget-color-darken">
            <header>Iframe Tag</header>
            <div>
            <c:if test = "${type=='object'}">
                <xmp><iframe src="http://10.10.30.21:8082/crowd/object?site=sugang&userid=konan" style="width:600px; height:650px;" scrolling="no" ></iframe></xmp>
            </c:if>
            <c:if test = "${type=='background'}">
                <xmp><iframe src="http://10.10.30.21:8082/crowd/background?site=sugang&userid=konan" style="width:600px; height:530px;" scrolling="no" ></iframe></xmp>
            </c:if>
            <c:if test = "${type=='drag'}">
                <xmp><iframe src="http://10.10.30.21:8082/crowd/drag?site=sugang&userid=konan" style="width:600px; height:820px;" scrolling="no" ></iframe></xmp>
            </c:if>
            </div>
        </div>
    </div>
</article>
<c:import url="../includes/script.jsp"/>
<c:import url="../includes/footer.jsp"/>
<script>
    window.addEventListener("message",function(event){
        alert(event.data);
        callback();
    });
    function callback(){
        alert("callback test");
    }
</script>