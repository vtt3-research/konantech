<%--
  Created by IntelliJ IDEA.
  User: jeunghakmoon
  Date: 2018. 11. 1.
  Time: 오전 9:51
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@include file="../includes/taglib.jsp" %>
<c:import url="../includes/iframe/header.jsp"/>
<article>
    <div class="shot-list-wrap">
        <form>
            <input type="hidden" name="userid" value="${param.userid}" />
            <input type="hidden" name="site" value="${param.site}" />
            <input type="hidden" name="objectcode" value="" />
        <c:forEach var="result" items="${imageList}" varStatus="i">
            <div class="col-xs-6 exImg"  name="selShotImgLi" style="display:none;">
                <c:set var="img_path" value="${ result.filepath }"/>
                <div>
                    <img src="${img_path}"/>
                    <input type="hidden" name="videoid" value="${result.videoid}"/>
                    <input type="hidden" name="shotid" value="${result.shotid}"/>
                    <input type="hidden" name="targetid" value="${result.targetid}"/>
                    <input type="hidden" name="score" value="${result.score}"/>
                    <input type="hidden" name="target" value="${result.target}"/>
                    <input type="hidden" name="ischeck" value="false" />
                </div>
            </div>
            <c:if test="${result.score != '1' }">
                <c:set var="targetname" value="${result.codename}"/>
                <c:set var="objectcode" value="${result.objectcode}"/>
            </c:if>
        </c:forEach>
        </form>
    </div>
    <div class="qa">Please select ${targetname} in the picture.</div>
    <div class="buttonarea">
        <button class="btn btn-success" id="setCrowdData">제출</button>
    </div>
</article>
<c:import url="../includes/iframe/script.jsp"/>
<script>
    var _maxHeight=0;
    var _maxWidth=0;
    $(document).ready(function(){
        _maxWidth = $(".exImg").width();
        _maxHeight = $(".exImg").height();
        $(".exImg img").on("load", function(){
            setImg($(this));
        }).each(function() {
            if (this.complete) {
                $(this).load();
            }
        });
        $(".exImg").show();

        $(".exImg").on("click",function(){
           $(this).toggleClass("ui-selected");
           if($(this).find("[name=ischeck]").val()=="true"){
               $(this).find("[name=ischeck]").val("false");
           }else{
               $(this).find("[name=ischeck]").val("true")
           }
        });
        $("#setCrowdData").on("click", function(){
            $("input[name=objectcode]").val('${objectcode}');
           $.ajax({
               url: "<c:url value='/setCrowdData/background' />",
               async: false,
               type: "POST",
               data: $("form").serializeArray(),
               dataType: "json",
               success: function (response) {
                   if (response.status == "SUCCESS") {
                       MSG.alert(response.data.message);
                   } else if (response.status == "ERROR") {
                       MSG.alert(response.data.message);
                   } else {
                       MSG.alert("오류");
                       return false;
                   }
               },
               error: function (xhr, opt, err) {
                   MSG.alert(xhr.responseText);
               }
           });
        });

    });

    function setImg ($img){
        if($($img).attr("prop_height")>$($img).attr("prop_width"))
            $($img).height(_maxHeight+ "px");
        else
            $($img).width(_maxWidth+"px");
    }
</script>
<c:import url="../includes/iframe/footer.jsp"/>