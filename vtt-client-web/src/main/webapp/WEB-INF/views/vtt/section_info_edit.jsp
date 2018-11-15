<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@include file="../includes/taglib.jsp" %>
<c:import url="../includes/header.jsp"/>
<!-- top navigation -->
<div class="top_nav">
    <div class="nav_menu nav_menu-j">
        <nav>
            <div class="col-md-5 col-8 align-self-center">
                <div class="title-txt text-themecolor">
                    CONTENT
                </div>
                <ol class="breadcrumb">
                    <li class="breadcrumb-item">
                        <a href="javascript:void(0)"> 콘텐츠</a>
                    </li>
                    <li class="breadcrumb-item ">
                        <a href="<c:url value="/content"/>"> 콘텐츠 리스트</a>
                    </li>
                    <li class="breadcrumb-item strong active">
                        VTT 구간정보 편집
                    </li>
                </ol>
            </div>
            <div class="nav toggle">
                <a id="menu_toggle"><i class="fa fa-bars"></i></a>
            </div>
        </nav>
    </div>
</div>
<!-- /top navigation -->
<div class="right_col" role="main">
    <%--row : ribbon--%>
    <div class="row">
        <div class="col-12">
            <!-- RIBBON -->
            <span class="item"></span>
            <span class="ribbon-button-alignment pull-right" style="margin-right:25px">
            <span id="btn_edit_toggle" class="btn btn-ribbon hidden-xs" data-target=".multi-collapse" data-toggle="collapse" aria-expanded="true" aria-controls=".multi-collapse"><i class="fa fa-file-picture-o "></i> 시각정보 편집(확대/축소)</span>
            <span id="btn_delete" class="btn btn-ribbon hidden-xs" data-title="add"><i class="fa fa-trash-o txt-color-customRed"></i> 삭제</span>
            <span id="btn_storyboard" class="btn btn-ribbon hidden-xs" data-title="add"><i class="fa fa-picture-o txt-color-orange"></i> 스토리보드</span>
            <span id="btn_play" class="btn btn-ribbon hidden-xs" data-title="add"><i class="fa fa-youtube-play txt-color-orange"></i> 객체인식재생</span>
            </span>
        </div>
    </div>
    <div class="row">
        <div id="sectionList" class="collapse multi-collapse show col-3 ">
            <%--visul_section_list--%>
        </div>
        <div id="secNewList" class="collapse multi-collapse show col-3">
            <div class="card">
                <div class="card-body">
                    <h6 class="card-title">구간 리스트</h6>
                    <div class="" style="height:600px; overflow-y: scroll;" id="selectList">
                        <table class="table table-hover">
                            <thead>
                            <tr>
                                <th scope="col">
                                    <small>구간</small>
                                </th>
                                <th scope="col">
                                    <small>구간번호</small>
                                </th>
                                <th scope="col">
                                    <small>구간범위</small>
                                </th>
                            </tr>
                            </thead>
                            <tbody>
                            <c:forEach var="result" items="${rtnMap}" varStatus="i">
                                <tr>
                                    <td>
                                        <small><c:out value="${i.count}"/>구간</small>
                                    </td>
                                    <td>
                                            ${result.visualperiodnum}
                                    </td>
                                    <td>
                                        <small>${result.starttime}~${result.endtime}</small>
                                    </td>
                                </tr>
                            </c:forEach>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
        <div id="list2" class="collapse multi-collapse show col-6">
            <div class="card">
                <div class="card-body">
                    <h6 class="card-title">
                        비디오
                        <span id="tagTitle1" class="badge badge-secondary">
                            <small>현재구간 <span id="videoTabNo" class="badge badge-primary">0</span></small>
                        </span>
                    </h6>
                    <div class="videobox" style="height:600px;">
                        <video id="videojs" class="video-js vjs-default-skin vjs-big-play-centered" controls preload="auto" data-setup='{ "playbackRates" : [0.5, 1.0, 1.5, 2.0] }' style="width: 100%; height: 100%;"
                               poster src = "http://10.10.30.21:7070/darc4/video/2018/07/30/638/OV201800000638.mp4"></video>
                        <div class="select-video">원본</div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<c:import url="../includes/script.jsp"/>
<script>
    $(document).ready(function() {
        getSectionList(<c:out value="${idx}"/>,'section'); // 리스트 idx 넘겨받음.(ex) 624
        fnLoadLayoutOpt();
        playerSet();
    });

    var myPlayer =  videojs('#videojs');
    function playerSet(){
        // videojs
        player.init("videojs", "video", "${ videoServerUrl }/${ contentField.assetfilepath }/${ contentField.assetfilename}", "");
        //player.init("videojs", "video", "http://10.10.30.21:7070/darc4/video/2018/07/30/638/OV201800000638.mp4", "");
        videojs("videojs").on('timeupdate',getTimeBiz);
    }


    function getTimeBiz() {
            console.log("myPlayer.currentTime()", code2frame( time2Code('video',videojs("videojs").currentTime(),999),999));
        fnLoadPlaySec(time2Code('video',videojs("videojs").currentTime(),999));
    }

    function getSectionList(idx,viewChk) {
        var $div='';
        $.ajax({
            url: 'getSectionList',
            type: 'POST',
            data: {
                'idx': idx,
                'viewChk': viewChk
            },
            dataType: 'html',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (html) {
                $div = html;
                $("#sectionList").html($div);
            },
            error: function (xhr, ajaxOptions, thrownError) {
            }
        });
    }

    function fnLoadLayoutOpt(){
        $(document).on("click", "[name='secTr']", function () {
            if($(this).hasClass('table-info')){
                $(this).removeClass('table-info');
            }else if($(this).hasClass('table-warning')){
                $(this).removeClass('table-warning');
                $(this).addClass('table-success');
            }else if($(this).hasClass('table-success')){
                $(this).removeClass('table-success');
                $(this).addClass('table-info');
            }else{
                $(this).addClass('table-info');
            }
            //console.log( code2frame($(this).find('td[name=secTd]').text(),999));
            //console.log( $(this).find('td[name=secTd]').text());
            var tdText = $(this).find('td[name=secTdTime]').text();
            var arrVal =tdText.split('~');
            //console.log( arrVal[0]);
            //console.log( arrVal[1]);
        });

        $(document).on("click", "#btnNewSec", function () {
            var arrPrm = new Array();
            var arrObjPrm = new Array();
            var secSeq = 1;
            var startTime = "";
            var endTime = "";
            var total = $("tr[name='secTr']").length;
            $("tr[name='secTr']").each(function () {
                if ($(this).hasClass('table-info') || $(this).hasClass('table-success') ) {
                    arrPrm.push($(this).find('td[name=secTdTime]').attr('value'));
                    arrObjPrm.push($(this));
                }
            });

            if (arrObjPrm.length > 0) {
                    var tdText = arrObjPrm[0].find('td[name=secTdTime]').text();
                    var arrVal = $.trim(tdText).split('~');
                    startTime = arrVal[0];
                    var tdText = arrObjPrm[arrObjPrm.length-1].find('td[name=secTdTime]').text();
                    var arrVal = $.trim(tdText).split('~');
                    endTime = arrVal[1];
            }
            if($('#secNewList').find('tbody tr').length > 0){
                var secSeq = $('#secNewList').find('tbody tr').length +1;
            }

            $.ajax({
                url: 'getPutSecInfo',
                type: 'POST',
                data: {
                    'arrPrm': arrPrm,
                    'secSeq': secSeq,
                    'startTime': startTime,
                    'endTime': endTime,
                    'videoId': ${idx},
                },
                async: false,
                // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
                success: function (data) {
                    toast("시각정보리스트", "저장되었습니다.", "info", 5000);
                },
                error: function (xhr, ajaxOptions, thrownError) {
                    toast("시각정보리스트", "저장이 실패 하였습니다..", "info", 5000);
                },
                complete : function() {
                }
            });

        });

        $(document).on("click", "#btnReset", function () {
            $("tr[name='secTr']").removeClass('table-info');
            $("tr[name='secTr']").each(function () {
                if ($(this).hasClass('table-success')) {
                    $(this).addClass('table-warning');
                }
            });
        });
    }

    function fnLoadPlaySec(time){
        var curFrm =code2frame(time,999);
        $("tr[name='secTr']").each(function () {
            //console.log( $(this).find('td[name=secTd]').text());
            var tdText = $(this).find('td[name=secTdTime]').text();
            var arrVal =tdText.split('~');
            var preSecTime = code2frame(arrVal[0],999) ;
            var nextSecTime = code2frame(arrVal[1],999)
            if(curFrm >= preSecTime  && curFrm < nextSecTime  ){
                $("tr[name='secTr']").removeClass('table-warning');
                if($(this).hasClass('table-info')) {
                    $(this).removeClass('table-info');
                    $(this).addClass('table-success');
                }else if($(this).hasClass('table-success')){
                    $(this).removeClass('table-success');
                    $(this).addClass('table-info');
                }else{
                    $(this).addClass('table-warning');
                }
                var tdNum=$(this).find('td[name=secTdNum]').text();
                $('#videoTabNo').text(tdNum);
            }
        });

    }
</script>
</script>
<c:import url="../includes/footer.jsp"/>

