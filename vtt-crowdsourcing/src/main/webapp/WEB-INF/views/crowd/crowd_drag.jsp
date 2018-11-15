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
        <form id="answerFrom">
            <input type="hidden" name="userid" value="${param.userid}" />
            <input type="hidden" name="site" value="${param.site}" />

        <c:forEach var="obj" items="${objectList}" varStatus="i">
            <div class="col-xs-6 objectarea"  name="selShotImgLi">
                <div id="${obj.objectcode}">
                    ${obj.codename}
                </div>
            </div>
        </c:forEach>
        <c:forEach var="result" items="${imageList}" varStatus="i">
            <div class="col-xs-2 exImg"  name="selShotImgLi" style="display:none;">
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
        </c:forEach>
        </form>
    </div>
    <div class="qa">Please drag and drop image to the matching area.</div>
    <div class="buttonarea">
        <button class="btn btn-success" id="setCrowdData">제출</button>
    </div>
</article>
<c:import url="../includes/iframe/script.jsp"/>
<script>
    var _maxHeight=0;
    var _maxWidth=0;
    var _param={};
    $(document).ready(function(){
        _maxWidth = $(".exImg").width();
        _maxHeight = $(".exImg").height();
        $(".exImg img").on("load", function(){
            setImg($(this));
            setParent($(this));
        }).each(function() {
            if (this.complete) {
                $(this).load();
            }
        });
        $(".exImg").show();

        $(".exImg>div" ).draggable({
            snap: ".objectarea", //drop 영역
            snapMode: "inner",  //drop 라인
            revert: "invalid", // when not dropped, the item will revert back to its initial position
            cursor: "move",//마우스 모양
            stack: ".exImg>div",
            start:function(){
                $(this).addClass("moveImg");
            },
            stop:function(){
                $(this).removeClass("moveImg");
            }
        });

        $(".objectarea").height(_maxHeight+"px");
        $(".objectarea").droppable({
            accept: ".exImg > div",
            classes: {
                "ui-droppable-active": "ui-state-highlight"
            },
            drop: function( event, ui ) {
                var $area = $(this);
                var $parent = $(".moveImg").parent();
                var top = $(".moveImg").position().top;
                var left = $(".moveImg").position().left;
                var bottom = $(".moveImg").position().top+$(".moveImg").height();
                var right = $(".moveImg").position().left+$(".moveImg").width();

                if($area.position().top - $parent.position().top>top) {
                    top = $area.position().top - $parent.position().top;
                }
                if($area.position().left - $parent.position().left>left){
                    left = $area.position().left - $parent.position().left;
                }
                if($area.position().top+$area.height() - $parent.position().top<bottom) {
                    top = $area.position().top - $parent.position().top;
                }
                if($area.position().left+$area.width() - $parent.position().left<right){
                    left = $area.position().left - $parent.position().left;
                }
                $(".moveImg").animate({top:top+'px', left:left+'px'});
                $(".moveImg").find("input[name=targetcode]").val( $(this).find("div").attr("id"));

                _param[$(".moveImg").find("input[name=targetid]").val()]=$(this).find("div").attr("id");
            }
        });

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
        var ratio;
        ratio = _maxHeight/$($img).attr("prop_height");
        if(ratio>_maxWidth/$($img).attr("prop_width"))
            ratio = _maxWidth/$($img).attr("prop_width");
        $($img).height($($img).prop("naturalHeight") * ratio + "px");
        $($img).width($($img).prop("naturalWidth")*ratio+"px");

        var setX = $($img).attr("prop_x")*ratio;
        $($img).css("margin-left","-"+setX+"px");
        var setY = $($img).attr("prop_y")*ratio;
        $($img).css("margin-top","-"+setY+"px");
        $($img).attr("prop_ratio",ratio);
    }

    function setParent ($img){
        var imgRatio = $($img).attr("prop_ratio");
        $($img).parent().width($($img).attr("prop_width")*imgRatio+"px");
        $($img).parent().height($($img).attr("prop_height")*imgRatio+"px");
        if(_maxHeight>$($img).parent().height()){
            $($img).parent().height(_maxHeight+"px");
        }
    }
</script>
<c:import url="../includes/iframe/footer.jsp"/>