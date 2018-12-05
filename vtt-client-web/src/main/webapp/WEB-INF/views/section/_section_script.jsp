<%--
  Created by IntelliJ IDEA.
  User: jeunghakmoon
  Date: 26/11/2018
  Time: 8:24 PM
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%@include file="../includes/taglib.jsp" %>
<script>
    /*구간 선택*/
    function getSectionShotList(videoid,shotid,delflag,filepath,filename){
        if(myPlayer1!=null) {
            myPlayer1.pause();
        }
        $('.table tbody tr[name="secTr"]').removeClass('ui-selected');
        var $tr = $('#secTr_'+shotid);
        $tr.addClass('ui-selected');
        var startsec=$tr.find("input[name=startsec]").val();
        myPlayer1.currentTime(startsec);
    }

    /*구간 조회*/
    function getSectionList(idx) {
        var $div='';
        $.ajax({
            url: '<c:url value="/visual/getSectionList"/>',
            type: 'POST',
            data: {'idx': idx, 'rate':_rate},
            async: false,
            dataType: 'html',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (html) {
                $div = html;
                $("#sectionList").html($div);
            },
            error: function (xhr, ajaxOptions, thrownError) {
                MSG.alert("getSectionList </br> status:"+xhr.status+"</br> message:"+xhr.responseText);
            }
        });
    }

    /*QA구간 조회*/
    function getQaSectionList(idx) {
        var $div='';
        $.ajax({
            url: '<c:url value="/section/getQaSectionList"/>',
            type: 'POST',
            data: {'idx': idx, 'rate':_rate},
            async: false,
            dataType: 'html',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (html) {
                $div = html;
                $("#qaSectionList").html($div);
                $("#qaSectionList tr:first").click();
            },
            error: function (xhr, ajaxOptions, thrownError) {
                MSG.alert("getQaSectionList </br> status:"+xhr.status+"</br> message:"+xhr.responseText);
            }
        });
    }



    function resizeVideo() {
        var w = $("#scrollImgDiv").width() - 30;
        var h = $(".section-list-Wrap").height();
        $(".videobox").css("height",h/3);
        $(".videobox-layout").css("height",h/3);
        $(".videobox").css("width",w);
        $(".videobox-layout").css("width",w);
        $(".select-video").css("margin-top",h/3 * -1);
    }


    var param = {};
    param.idx = ${idx};
    param.viewChk = 'section';

    $(".btn-move").on("click",function(){
        var type = $(this).attr("id");
        location.href="<c:url value="/section/"/>"+type+"?" + $.param(param);;
    });
    $(window).resize(function(){
        resizeVideo();
    });
</script>