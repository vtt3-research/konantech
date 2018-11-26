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

<div class="container">
    <ul class="box-list">
        <form>
            <input type="hidden" name="userid" value="${param.userid}" />
            <input type="hidden" name="site" value="${param.site}" />
            <input type="hidden" name="objectcode" value="" />
            <c:forEach var="result" items="${imageList}" varStatus="i">
                <li>
                    <div class="image-box" name="selShotImgLi">
                        <c:set var="img_path" value="${ result.filepath }"/>
                        <div>
                            <img src="${img_path}" prop_x="${result.x}" prop_y="${result.y}" prop_width="${result.width}" prop_height="${result.height}"/>
                            <input type="hidden" name="videoid" value="${result.videoid}"/>
                            <input type="hidden" name="shotid" value="${result.shotid}"/>
                            <input type="hidden" name="targetid" value="${result.targetid}"/>
                            <input type="hidden" name="score" value="${result.score}"/>
                            <input type="hidden" name="target" value="${result.target}"/>
                            <input type="hidden" name="ischeck" value="false" />
                        </div>
                    </div>
                </li>
                <c:if test="${result.score != '1' }">
                    <c:set var="targetname" value="${result.codename}"/>
                    <c:set var="objectcode" value="${result.objectcode}"/>
                </c:if>
            </c:forEach>
        </form>
    </ul>
    <p class="qa">Please select ${targetname} in the picture.</p>
</div>
<c:import url="../includes/iframe/footer.jsp"/>
<c:import url="../includes/iframe/script.jsp"/>
<script>

    var _maxHeight=160;
    var _maxWidth=0;
    $(document).ready(function(){
        _maxWidth = $(".image-box").width();
        _maxHeight = $(".image-box").height();
        $(".image-box img").on("load", function(){
            setImg($(this));
            setParent($(this));
        }).each(function() {
            if (this.complete) {
                $(this).load();
            }
        });
        $(".image-box").show();

        $(".image-box").on("click",function(){
           $(this).toggleClass("active");
           if($(this).find("[name=ischeck]").val()=="true"){
               $(this).find("[name=ischeck]").val("false");
           }else{
               $(this).find("[name=ischeck]").val("true")
           }
        });
        $("#setCrowdData").on("click", function(){
            $("input[name=objectcode]").val('${objectcode}');

           $.ajax({
               url: "<c:url value='/setCrowdData/object' />",
               async: false,
               type: "POST",
               data: $("form").serializeArray(),
               dataType: "json",
               success: function (response) {
                   if (response.status == "SUCCESS") {
                       MSG.alert(response.data.message,reload);
                   } else if (response.status == "ERROR") {
                       MSG.alert(response.data.message,reload);
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
        var ratio;
        ratio = _maxHeight/$img.attr("prop_height");
        if(ratio>_maxWidth/$img.attr("prop_width"))
            ratio = _maxWidth/$img.attr("prop_width");
        $img.height($img.prop("naturalHeight") * ratio + "px");
        $img.width($img.prop("naturalWidth")*ratio+"px");

        var setX = $img.attr("prop_x")*ratio*-1;
        $img.css("left",setX+"px");
        var setY = $img.attr("prop_y")*ratio*-1;
        $img.css("top",setY+"px");
        $img.attr("prop_ratio",ratio);
    }

    function setParent ($img){
        var imgRatio = $img.attr("prop_ratio");
        $img.parent().width($img.attr("prop_width")*imgRatio+"px");
        $img.parent().height($img.attr("prop_height")*imgRatio+"px");
        if(_maxHeight>$img.parent().height()){
            var marginTop = _maxHeight - $img.parent().height();
            $img.parent().css("margin-top",marginTop/2+"px");
        }
    }
</script>