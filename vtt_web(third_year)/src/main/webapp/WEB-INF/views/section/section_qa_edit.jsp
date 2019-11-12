<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@include file="../includes/taglib.jsp" %>
<c:import url="../includes/header.jsp"/>
<style type="text/css">
    .container.section .section-list-Wrap:not(:first-child){
        padding-left:10px;
    }
</style>
<!-- top navigation -->
<div class="top_nav">
    <div class="nav_menu nav_menu-j">
        <nav>
            <div class="col-8 align-self-center">
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
                        묘사 & Q&A 편집(${contentField.orifilename})
                    </li>
                </ol>
            </div>
        </nav>
    </div>
</div>

<!-- page content -->
<div class="container section" role="main">

    <!-- 카드 컨텐츠 시작 -->
    <%--<div class="row">
        <div class="col-md-12 col-sm-12 col-xs-12 btn-group">
            <div class="x_panel">
                <!--
                <button class="btn btn-success btn-move" id="info">구간 편집</button>
                <button class="btn btn-success btn-move" id="depiction">구간 묘사</button>
                <button class="btn btn-success btn-move" id="qa">QA</button>
                <button class="btn btn-success btn-move" id="relation">인과/의도</button>
                -->
            </div>
        </div>
    </div>--%>
    <div class="row">

        <div class="section-list-Wrap">
            <div id="qaSection-list" class="x_panel section-list">
                <div class="x_title">
                    <h2> Scene 리스트 <small> Scene list</small></h2>
                    <div class="clearfix"></div>
                </div>
                <div class="x_content">
                    <form id="qaSectionForm">
                        <input type="hidden" name="idx" value="${idx}"/>
                        <input type="hidden" name="rate" value="${rate}"/>
                        <table class="table table-striped">
                            <thead>
                            <tr>
                                <th>구간 </th>
                                <th>구간 범위 </th>
                            </tr>
                            </thead>
                            <tbody  id="qaSectionList">
                            </tbody>
                        </table>
                    </form>
                </div>
            </div>
        </div>

        <div class="section-list-Wrap">
            <div id="section-list" class="x_panel section-list">
                <div class="x_title">
                    <h2> Shot 리스트<small> Shot list</small>   <%--<small> Section list </small>--%></h2>
                    <div class="clearfix"></div>
                </div>
                <div class="x_content" id="sectionList"> </div>
            </div>
        </div>

        <div class="section-list-Wrap" style="width:calc(100% - 620px)">
            <div class="row">
                <div class="x_panel">
                    <div class="x_title">
                        <h2>영상<small>Video</small></h2>
                        <ul class="nav navbar-right panel_toolbox">
                            <li >
                                <a class="table-btn" onclick="help_hotkey()">
                                    <i class="fas fa-keyboard"></i> 단축키
                                </a>
                            </li>
                        </ul>
                        <div class="clearfix"></div>
                    </div>
                    <div class="x_content" id="scrollImgDiv">
                        <div class="col-xs-6 col-sm-6 col-md-6 col-lg-6 videobox">
                            <video id="videojs" class="video-js vjs-default-skin vjs-big-play-centered" controls preload="auto" data-setup='{ "playbackRates" : [0.5, 1.0, 1.5, 2.0] }' style="width: 100%; height: 100%;"></video>
                        </div>
                    </div>
                </div>
            </div>

            <div class="row">
                <div class="img-shot-title x_panel">
                    <div class="x_title">
                        <div id="qaSceneTitle" class="qaTitle" style="display:none;">
                        <h2>Scene 묘사 QA 등록<small> Scene QA</small></h2>
                        <ul class="nav navbar-right panel_toolbox">
                            <li>
                                <a id="btnGuideScene" class="table-btn" onclick="custModalPopup('section/guide/scene','guideModal');">
                                    <i class="fas fa-file"></i> 가이드라인
                                </a>
                            </li>
                            <li>
                                <a id="allObjView" class="table-btn" onclick="putQuestion()">
                                    <i class="fas fa-file"></i> 저장
                                </a>
                            </li>
                        </ul>
                        </div>
                        <div id="qaShotTitle" class="qaTitle" style="display:none;">
                            <h2 >Shot 묘사 QA 등록<small> Shot QA</small></h2>
                            <ul class="nav navbar-right panel_toolbox">
                                <li>
                                    <a id="btnGuideShot" class="table-btn" onclick="custModalPopup('section/guide/shot','guideModal');">
                                        <i class="fas fa-file"></i> 가이드라인
                                    </a>
                                </li>
                                <li>
                                    <a id="allObjViewShot" class="table-btn" onclick="putShotQuestion()">
                                        <i class="fas fa-file"></i> 저장
                                    </a>
                                </li>
                            </ul>
                        </div>
                        <div class="clearfix"></div>
                    </div>
                    <div class="x_content" id="sceneEdit">
                        <div class="row fix" style="height:100%;">
                            <div class="x_content">
                                <form class="needs-validation" id="qaForm" name="qaForm">

                                </form>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>

    </div>
    <!-- row end -->
</div>
<!-- /page content -->
<!-- END #MAIN PANEL -->
<c:import url="../includes/script.jsp"/>
<c:import url="./_section_script.jsp"/>
<script>
    var _rate = ${rate};
    var _startsec = 0;
    var _endsec = 0;


    /*qa구간 선택*/
    function setQaSectionInfo(tr){
        if(myPlayer1!=null){
            myPlayer1.pause();
        }
        var $tr = $(tr);
        var hasClass =$tr.hasClass('ui-selected');
        $('.table tbody tr[name="qaSecTr"]').removeClass('ui-selected');
        $tr.addClass('ui-selected');
        _startsec = $tr.find("input[name=startsec]").val();
        _endsec = $tr.find("input[name=endsec]").val();
        if(myPlayer1!=null){
            myPlayer1.currentTime(_startsec);
        }
        var sectionid = $tr.find("[name=sectionid]").val();
        if(sectionid != null && sectionid != ""){
            getQuestionList(sectionid);
            getSectionOfSceneList('<c:out value="${idx}"/>',sectionid);
        }else{
            MSG.alert("getDepictionList </br> 생성된 QA구간이 없습니다.");
            return;
        }
    }

    /*QA조회*/
    function getQuestionList(sectionid){
        $(".qaTitle").hide();
        $("#qaSceneTitle").show();
        if(sectionid==null||sectionid==''){
            MSG.alert("getDepictionList </br> 생성된 QA구간이 없습니다.");
            return;
        }
        $.ajax({
            url: '<c:url value="/section/getQuestionList"/>',
            type: 'POST',
            data: {"sectionid":sectionid},
            async: false,
            dataType: 'html',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (html) {
                $div = html;
                $("#qaForm").html($div);
            },
            error: function (xhr, ajaxOptions, thrownError) {
                MSG.alert("getQuestionList </br> status:"+xhr.status+"</br> message:"+xhr.responseText);
            }
        });
    }
    function getShotQuestionList(shotid){
        $(".qaTitle").hide();
        $("#qaShotTitle").show();
        if(shotid==null||shotid==''){
            MSG.alert("shot 정보가 없습니다.");
            return;
        }
        $.ajax({
            url: '<c:url value="/section/getShotQuestionList"/>',
            type: 'POST',
            data: {"shotid":shotid},
            async: false,
            dataType: 'html',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (html) {
                $div = html;
                $("#qaForm").html($div);
            },
            error: function (xhr, ajaxOptions, thrownError) {
                MSG.alert("getShotQuestionList </br> status:"+xhr.status+"</br> message:"+xhr.responseText);
            }
        });
    }
    /*저장*/
    function putQuestion(){
        var len = $("input[name=questionid]").length;
        var qaTitle = "";
        for(var i=0; i<len; i++){
            if($("input[name=questiontype]").eq(i).val() == "DESC"){
                if($("input[name=question]").eq(i).val()==""){
                    MSG.alert("묘사를 입력하여 주새요.");
                    return false;
                }
                if(!checkSpaces($("input[name=question]").eq(i).val())){
                    MSG.alert("묘사의 공백을 최소 2개 이상 입력하여 주새요.");
                    return false;
                }
            } else {
                if($("input[name=questiontype]").eq(i).val() == "QNALV3") {
                    qaTitle = "Q&A Level3";
                }else{
                    qaTitle = "Q&A Level4";
                }
                if($("input[name=question]").eq(i).val()!="" || $("input[name=answer]").eq(i).val()!="" || $("input[name=wrong_answer1]").eq(i).val()!="" || $("input[name=wrong_answer2]").eq(i).val()!=""
                    || $("input[name=wrong_answer3]").eq(i).val()!="" || $("input[name=wrong_answer4]").eq(i).val()!=""){
                    if($("input[name=question]").eq(i).val()==""){
                        MSG.alert(qaTitle + " 질문을 입력하여 주새요.");
                        return false;
                    }
                    if(!checkSpaces($("input[name=question]").eq(i).val())){
                        MSG.alert(qaTitle + " 질문의 공백을 최소 2개 이상 입력하여 주새요.");
                        return false;
                    }
                    if(!lastWordChk($("input[name=question]").eq(i).val(),"?")){
                        MSG.alert(qaTitle + " 질문의 마지막 문자를 '?'로 입력하여 주세요.");
                        return false;
                    }
                    if($("input[name=answer]").eq(i).val()==""){
                        MSG.alert(qaTitle + " 정답을 입력하여 주새요.");
                        return false;
                    }
                    if(!checkSpaces($("input[name=answer]").eq(i).val())){
                        MSG.alert(qaTitle + " 정답의 공백을 최소 2개 이상 입력하여 주새요.");
                        return false;
                    }
                    if(!lastWordChk($("input[name=answer]").eq(i).val(),".")){
                        MSG.alert(qaTitle + " 정답의 마지막 문자를 '.'로 입력하여 주세요.");
                        return false;
                    }
                    if($("input[name=wrong_answer1]").eq(i).val()==""){
                        MSG.alert(qaTitle + " 오답1를 입력하여 주새요.");
                        return false;
                    }
                    if(!checkSpaces($("input[name=wrong_answer1]").eq(i).val())){
                        MSG.alert(qaTitle + " 오답1의 공백을 최소 2개 이상 입력하여 주새요.");
                        return false;
                    }
                    if(!lastWordChk($("input[name=wrong_answer1]").eq(i).val(),".")){
                        MSG.alert(qaTitle + " 오답1의 마지막 문자를 '.'로 입력하여 주세요.");
                        return false;
                    }
                    if($("input[name=wrong_answer2]").eq(i).val()==""){
                        MSG.alert(qaTitle + " 오답2를 입력하여 주새요.");
                        return false;
                    }
                    if(!checkSpaces($("input[name=wrong_answer2]").eq(i).val())){
                        MSG.alert(qaTitle + " 오답2의 공백을 최소 2개 이상 입력하여 주새요.");
                        return false;
                    }
                    if(!lastWordChk($("input[name=wrong_answer2]").eq(i).val(),".")){
                        MSG.alert(qaTitle + " 오답2의 마지막 문자를 '.'로 입력하여 주세요.");
                        return false;
                    }
                    if($("input[name=wrong_answer3]").eq(i).val()==""){
                        MSG.alert(qaTitle + " 오답3를 입력하여 주새요.");
                        return false;
                    }
                    if(!checkSpaces($("input[name=wrong_answer3]").eq(i).val())){
                        MSG.alert(qaTitle + " 오답3의 공백을 최소 2개 이상 입력하여 주새요.");
                        return false;
                    }
                    if(!lastWordChk($("input[name=wrong_answer3]").eq(i).val(),".")){
                        MSG.alert(qaTitle + " 오답3의 마지막 문자를 '.'로 입력하여 주세요.");
                        return false;
                    }
                    if($("input[name=wrong_answer4]").eq(i).val()==""){
                        MSG.alert(qaTitle + " 오답4를 입력하여 주새요.");
                        return false;
                    }
                    if(!checkSpaces($("input[name=wrong_answer4]").eq(i).val())){
                        MSG.alert(qaTitle + " 오답4의 공백을 최소 2개 이상 입력하여 주새요.");
                        return false;
                    }
                    if(!lastWordChk($("input[name=wrong_answer4]").eq(i).val(),".")){
                        MSG.alert(qaTitle + " 오답4의 마지막 문자를 '.'로 입력하여 주세요.");
                        return false;
                    }
                }

            }
        }
        $.ajax({
            url: '<c:url value="/section/putQuestionList"/>',
            type: 'POST',
            data: $("#qaForm").serializeArray(),
            async: false,
            dataType: 'html',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (html) {
                $div = html;
                $("#qaForm").html($div);
                MSG.alert("구간QA가 저장되었습니다.");
            },
            error: function (xhr, ajaxOptions, thrownError) {
                MSG.alert("putQuestion </br> status:"+xhr.status+"</br> message:"+xhr.responseText);
            }
        });
    }
    /*저장*/
    function putShotQuestion(){
        var len = $("input[name=questionid]").length;
        for(var i=0; i<len; i++){
            if($("input[name=questiontype]").eq(i).val() == "DESC"){
                if($("input[name=question]").eq(i).val()==""){
                    MSG.alert("묘사를 입력하여 주새요.");
                    return false;
                }
                if(!checkSpaces($("input[name=question]").eq(i).val())){
                    MSG.alert("묘사의 공백을 최소 2개 이상 입력하여 주새요.");
                    return false;
                }
            }else{
                if($("input[name=questiontype]").eq(i).val()==""){
                    MSG.alert("Q&A Level를 입력하여 주새요.");
                    return false;
                }
                if($("input[name=question]").eq(i).val()==""){
                    MSG.alert("Q&A 질문을 입력하여 주새요.");
                    return false;
                }
                if(!checkSpaces($("input[name=question]").eq(i).val())){
                    MSG.alert("Q&A 질문의 공백을 최소 2개 이상 입력하여 주새요.");
                    return false;
                }
                if(!lastWordChk($("input[name=question]").eq(i).val(),"?")){
                    MSG.alert("Q&A 질문의 마지막 문자를 '?'로 입력하여 주세요.");
                    return false;
                }
                if($("input[name=answer]").eq(i).val()==""){
                    MSG.alert("Q&A 정답을 입력하여 주새요.");
                    return false;
                }
                if(!checkSpaces($("input[name=answer]").eq(i).val())){
                    MSG.alert("Q&A 정답의 공백을 최소 2개 이상 입력하여 주새요.");
                    return false;
                }
                if(!lastWordChk($("input[name=answer]").eq(i).val(),".")){
                    MSG.alert("Q&A 정답의 마지막 문자를 '.'로 입력하여 주세요.");
                    return false;
                }
                if($("input[name=wrong_answer1]").eq(i).val()==""){
                    MSG.alert("Q&A 오답1를 입력하여 주새요.");
                    return false;
                }
                if(!checkSpaces($("input[name=wrong_answer1]").eq(i).val())){
                    MSG.alert("Q&A 오답1의 공백을 최소 2개 이상 입력하여 주새요.");
                    return false;
                }
                if(!lastWordChk($("input[name=wrong_answer1]").eq(i).val(),".")){
                    MSG.alert("Q&A 오답1의 마지막 문자를 '.'로 입력하여 주세요.");
                    return false;
                }
                if($("input[name=wrong_answer2]").eq(i).val()==""){
                    MSG.alert("Q&A 오답2를 입력하여 주새요.");
                    return false;
                }
                if(!checkSpaces($("input[name=wrong_answer2]").eq(i).val())){
                    MSG.alert("Q&A 오답2의 공백을 최소 2개 이상 입력하여 주새요.");
                    return false;
                }
                if(!lastWordChk($("input[name=wrong_answer2]").eq(i).val(),".")){
                    MSG.alert("Q&A 오답2의 마지막 문자를 '.'로 입력하여 주세요.");
                    return false;
                }
                if($("input[name=wrong_answer3]").eq(i).val()==""){
                    MSG.alert("Q&A 오답3를 입력하여 주새요.");
                    return false;
                }
                if(!checkSpaces($("input[name=wrong_answer3]").eq(i).val())){
                    MSG.alert("Q&A 오답3의 공백을 최소 2개 이상 입력하여 주새요.");
                    return false;
                }
                if(!lastWordChk($("input[name=wrong_answer3]").eq(i).val(),".")){
                    MSG.alert("Q&A 오답3의 마지막 문자를 '.'로 입력하여 주세요.");
                    return false;
                }
                if($("input[name=wrong_answer4]").eq(i).val()==""){
                    MSG.alert("Q&A 오답4를 입력하여 주새요.");
                    return false;
                }
                if(!checkSpaces($("input[name=wrong_answer4]").eq(i).val())){
                    MSG.alert("Q&A 오답4의 공백을 최소 2개 이상 입력하여 주새요.");
                    return false;
                }
                if(!lastWordChk($("input[name=wrong_answer4]").eq(i).val(),".")){
                    MSG.alert("Q&A 오답4의 마지막 문자를 '.'로 입력하여 주세요.");
                    return false;
                }
            }
        }

        $.ajax({
            url: '<c:url value="/section/putShotQuestionList"/>',
            type: 'POST',
            data: $("#qaForm").serializeArray(),
            async: false,
            dataType: 'html',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (html) {
                $div = html;
                $("#qaForm").html($div);
                MSG.alert("구간QA가 저장되었습니다.");
            },
            error: function (xhr, ajaxOptions, thrownError) {
                MSG.alert("putQuestion </br> status:"+xhr.status+"</br> message:"+xhr.responseText);
            }
        });
    }
    function lastWordChk(str,word){
        if(str.substr(str.length-1,str.length) == word){
            return true;
        }else{
            return false;
        }
    }
    function checkSpaces(str){
        var spaceCnt = (str.match(/ /g) || []).length;
        if(spaceCnt < 2){
            return false;
        }else{
            return true;
        }
    }
    $(document).ready(function() {
        var _player = player.init("videojs", "video", "${ videoServerUrl }/${ contentField.assetfilepath }/${ contentField.assetfilename}", "");
        _player.on('timeupdate', function(){
            var nowsec = _player.currentTime()+0.02;
            $("#sectionList tr").each(function(){
                var startsec = $(this).find("input[name=startsec]").val();
                var endsec = $(this).find("input[name=endsec]").val();
                if(nowsec>=startsec && nowsec<endsec){
                    $("#sectionList tr").removeClass('ui-selected');
                    $(this).addClass('ui-selected');
                    document.getElementById("section-list").scrollTop = $(this).position().top-40;
                    return false;
                }
            });
            if(_startsec!=_endsec){
                if(nowsec>=_endsec-0.2){
                    _player.pause();
                }
            }
        });
        _player.on("play",function(){
            _player.currentTime(_startsec);
        });

        // videojs

        //getSectionList(<c:out value="${idx}"/>); // 리스트 idx 넘겨받음.(ex) 624

        //getSectionOfSceneList(<c:out value="${idx}"/>); // 리스트 idx 넘겨받음.(ex) 624
        getQaSectionList(<c:out value="${idx}"/>); // 리스트 idx 넘겨받음.(ex) 624
        resizeVideo();
        proceedHotkey();
        $("#relationList").height($(".section-list-Wrap").height()/2);
    });
</script>
<c:import url="../includes/footer.jsp"/>