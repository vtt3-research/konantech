<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
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
                        음성 편집
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

<!-- page content -->
<div class="right_col" role="main">
    <div class="row">
        <div class="col-md-12 col-sm-12 col-xs-12">
            <div class="x_panel">
                <div class="x_title">
                    <h2>영상<small>Video</small></h2>
                    <ul class="nav navbar-right panel_toolbox">
                        <li class="m-right10">
                            <a class="table-btn" onclick="help_hotkey()">
                                <i class="fa fa-file-o"></i>
                                단축키
                            </a>
                        </li>
                    </ul>
                    <div class="clearfix"></div>
                </div>
                <div class="x_content superbox" id = "content">
                    <div class="row">
                        <div class="col-xs-6 col-sm-6 col-md-6 col-lg-6 videobox">
                            <video id="videojs" class="video-js vjs-default-skin vjs-big-play-centered" controls preload="auto" data-setup='{ "playbackRates" : [0.5, 1.0, 1.5, 2.0] }' style="width: 100%; height: 100%;"></video>
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <div class="col-md-12 col-sm-12 col-xs-12" id="imgShot">
            <div class="img-shot-title x_panel">
                <div class="x_title">
                    <h2>구간 편집<small> Section edit</small></h2>
                    <div class="clearfix"></div>
                </div>
                <div class="x_content" id="repreImgView">
                    <button class="btn btn-success" style="padding:5px 7px; margin-left:6px; float:left;" onclick="setTime('start')">시작구간 설정</button>
                    <button class="btn btn-success" style="padding:5px 7px; margin-left:6px; float:left;" onclick="setTime('end')">종료구간 설정</button>
                    <button class="btn btn-success" style="padding:5px 7px; margin-left:6px; float:left;" onclick="replay()">구간듣기</button>
                    <button class="btn btn-success" style="padding:5px 7px; margin-left:6px; float:left;" onclick="appendSection()">신규구간 생성</button>
                    <button class="btn btn-success" style="padding:5px 7px; margin-left:6px; float:left;" onclick="deleteSeoundSection()">삭제</button>
                    <button class="btn btn-success" style="padding:5px 7px; margin-left:6px; float:left;" onclick="putSeoundSection()">저장</button>

                    <form class="needs-validation" id="soundForm" name="soundForm">
                        <input type="hidden" name="idx" value="${idx}"/>
                        <table class="table table-striped">
                            <thead>
                            <tr>
                                <th>순번</th>
                                <th>시작시간</th>
                                <th>종료시간</th>
                                <th>소리타입</th>
                            </tr>
                            </thead>
                            <tbody>
                            </tbody>
                        </table>
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<c:import url="../includes/script.jsp"/>
<script>
    var _startsec = "0:00:00";
    var _endsec = "0:00:00";
    var isReplay = false;

    /* 오디오정보 조회 */
    function getSeoundSectionList(idx){
        $.ajax({
            url: '<c:url value="/sound/soundList"/>',
            type: 'POST',
            data: {'idx': idx},
            async: false,
            dataType: 'html',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (html) {
                $div = html;
                $("#soundForm tbody").html($div);
            },
            error: function (xhr, ajaxOptions, thrownError) {
                MSG.alert("getSectionList </br> status:"+xhr.status+"</br> message:"+xhr.responseText);
            }
        });
    }

    /*qa구간 선택*/
    function getSoundInfo(tr){
        if(myPlayer1!=null) {
            myPlayer1.pause();
        }
        var $tr = $(tr);
        if($tr.hasClass("ui-selected")) {
            _startsec = $tr.find("input[name=starttimecode]").val();
            _endsec = $tr.find("input[name=endtimecode]").val();
        }else{
            _startsec = "0:00:00";
            _endsec = "0:00:00";
        }
        if(myPlayer1!=null&&_startsec!=0) {
            myPlayer1.currentTime(code2time(_startsec));
        }
    }
    /*신규구간 생성*/
    function appendSection(){
        $tr = $("#soundTr_new").clone();
        $tr.attr("id","");
        $tr.show();
        $("#soundForm tbody").append($tr);
        $("#soundForm tr").removeClass("ui-selected");
        $tr.addClass("ui-selected");
    }

    /*시간 설정*/
    function setTime(type){
        var nowTime = myPlayer1.currentTime();
        if(nowTime>0.4){
            nowTime-=0.4;
        }
        var time = time2Code('audio',nowTime,999);
        var $tr = $("#soundForm tr.ui-selected");
        if(type=="start") {
            $tr.find("input[name=starttimecode]").val(time);
            $tr.find("td[name=startTime]").text(time);
        }else{
            $tr.find("input[name=endtimecode]").val(time);
            $tr.find("td[name=endTime]").text(time);
            myPlayer1.pause();
        }
    }

    function replay(){
        isReplay = true;
        var $tr = $("#soundForm tr.ui-selected");

        _startsec = $tr.find("input[name=starttimecode]").val();
        _endsec = $tr.find("input[name=endtimecode]").val();
        myPlayer1.currentTime(code2time(_startsec));
        myPlayer1.play();
    }

    /*구간 삭제*/
    function deleteSeoundSection(){
        var $this = $("#soundForm .ui-selected");
        var delflag = $this.find("input[name=delflag]").val();
        $this.toggleClass("delete");
        if(delflag =='false') {
            $this.find("input[name=delflag]").val("true");
        }else{
            $this.find("input[name=delflag]").val("false");
        }
    }

    /*저장*/
    function putSeoundSection(){
        $("#soundTr_new").remove();
        $.ajax({
            url: '<c:url value="/sound/putSoundList"/>',
            type: 'POST',
            data: $("#soundForm").serializeArray(),
            async: false,
            dataType: 'html',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (html) {
                $div = html;
                $("#soundForm tbody").html($div);
                MSG.alert("오디오 정보가 저장되었습니다.");
            },
            error: function (xhr, ajaxOptions, thrownError) {
                MSG.alert("putQaSectionList </br> status:"+xhr.status+"</br> message:"+xhr.responseText);
            }
        });
    }
    function resizeVideo() {
        var w = $("#content").width() - 30;
        var h = $(window).height() - 400;
        $(".videobox").css("height",h);
        $(".videobox-layout").css("height",h);
        $(".videobox").css("width",w);
        $(".videobox-layout").css("width",w);
        $(".select-video").css("margin-top",h * -1);
    }
    var param = {};
    param.idx = ${idx};
    param.viewChk = 'section';
    $(window).resize(function(){
        resizeVideo();
    });
    $(document).ready(function() {
        // videojs
        var _player = player.init("videojs", "video", "${ videoServerUrl }/${ contentField.assetfilepath }/${ contentField.assetfilename}", "");
        _player.on('timeupdate',function(){
            var nowTime = _player.currentTime();
            if(isReplay&&nowTime>=code2time(_endsec)){
                isReplay=false;
                _player.pause();
            }
        });
        resizeVideo();
        getSeoundSectionList(<c:out value="${idx}"/>);

        $("#soundForm").on("click","tr", function(){
            $("#soundForm tr").removeClass("ui-selected");
            $(this).addClass("ui-selected");
            var startTime = $(this).find("input[name=starttimecode]").val();
            _player.currentTime(code2time(startTime));
        });
        //단축키
        proceedHotkey();
        $(this).bind('keydown.ctrl_[',function (e){ /* ctrl + [ = 마크인 */
            setTime("start");
            return false;
        });
        $(this).bind('keydown.ctrl_]',function (e){ /* ctrl + ] = 마크아웃 */
            setTime("end");
            return false;
        });
        $(this).bind('keydown.ctrl_space',function (e){ /* ctrl + ] = 구간 반복 */
            replay();
            return false;
        });
        $(this).bind('keydown.ctrl_n',function (e){ /* ctrl + ] = 신규 생성 */
            appendSection();
            return false;
        });
    });
</script>
<c:import url="../includes/footer.jsp"/>