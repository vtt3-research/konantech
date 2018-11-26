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
<div class="container container-multiple">
    <form id="answerFrom">
    <ul class="top connectedSortable" id="sortable1">
        <input type="hidden" name="userid" value="${param.userid}" />
        <input type="hidden" name="site" value="${param.site}" />
        <c:forEach var="result" items="${imageList}" varStatus="i">
        <li>
            <div class="image-box">
                <c:set var="img_path" value="${ result.filepath }"/>
                <div id="${result.targetid}">
                    <img src="${img_path}" prop_x="${result.x}" prop_y="${result.y}" prop_width="${result.width}" prop_height="${result.height}"/>
                    <input type="hidden" name="videoid" value="${result.videoid}"/>
                    <input type="hidden" name="shotid" value="${result.shotid}"/>
                    <input type="hidden" name="targetid" value="${result.targetid}"/>
                    <input type="hidden" name="score" value="${result.score}"/>
                    <input type="hidden" name="target" value="${result.target}"/>
                    <input type="hidden" name="objectcode" value="${result.objectcode}"/>
                    <input type="hidden" name="targetcode" value="${result.targetcode}"/>
                </div>
            </div>
        </li>
        </c:forEach>
    </ul>
    <ul class="drag-area">
        <c:forEach var="obj" items="${objectList}" varStatus="i">
        <li class="objectarea" >
            <p>${obj.codename}</p>
            <ul class="connectedSortable" id="${obj.objectcode}"></ul>
        </li>
        </c:forEach>
    </ul>
    </form>
</div>
<c:import url="../includes/iframe/footer.jsp"/>
<c:import url="../includes/iframe/script.jsp"/>
<script>
    var _maxHeight=0;
    var _maxWidth=0;
    var _param={};
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

        $( ".connectedSortable" ).sortable({
            connectWith: ".connectedSortable",
            placeholder: "portlet-placeholder ui-corner-all",
            rever:true,
            update:function(event,ui){
                $(ui.item).find("input[name=targetcode]").val( $(ui.item).parent().attr("id"));
                _param[$(ui.item).find("input[name=targetid]").val()]=$(ui.item).parent().attr("id");
            }
        }).disableSelection();


        $("#setCrowdData").on("click", function(){
            if(Object.keys(_param).length!=6){
                MSG.alert("Please drag and drop all image to the matching area.");
                return;
            }

            $.ajax({
                url: "<c:url value='/setCrowdData/drag' />",
                async: false,
                type: "POST",
                data: $("#answerFrom").serialize(),
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