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
                        VTT 시각정보 편집
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

    <!-- 카드 컨텐츠 시작 -->
    <div class="row">

        <div class="section-list-Wrap">
            <div class="x_panel section-list">
                <div class="x_title">
                    <h2> 구간 리스트  </h2>
                    <button class="btn btn-success" style="padding:2px 7px; margin-left:6px;" onclick="toggleUpdateShot()">수정</button>
                    <div class="clearfix"></div>
                </div>
                <div class="x_content" id="sectionList"> </div>
            </div>
        </div>

        <div class="col-md-10 col-sm-10 col-xs-12">
            <div class="x_panel">
                <div class="x_title">
                    <h2> 구간 샷 리스트  <small> Section shot list </small></h2>
                    <div class="clearfix"></div>
                </div>
                <div class="x_content" id="scrollImgDiv"></div>
            </div>
        </div>

        <div class="col-md-7 col-sm-7 col-xs-12" id="imgShot">
            <div class="img-shot-title x_panel">
                <div class="x_title">
                    <h2> 이미지 샷   <small> Image shot </small></h2>
                    <div class="clearfix"></div>
                </div>
                <div class="x_content" id="repreImgView"></div>
            </div>
        </div>

        <div class="shot-info-wrap" >
            <div class="x_panel">
                <div class="x_title">
                    <h2> 시각정보편집  <small> Image edit </small></h2>
                    <div class="clearfix" >
                    </div>
                </div>


                <div class="x_content" id="visualInfoEdit" style="display: none">
                    <div class="row ">
                        <div class="edit-btns-wrap">
                            <sec:authorize access="hasAnyRole('ROLE_ADMIN,ROLE_EDITER')" var="u">
                            <ul class="nav navbar-right panel_toolbox">
                                <li class="m-right10">
                                    <a id="allObjView" class="table-btn">
                                        <i class="fa fa-file-o"></i>
                                        전체
                                    </a>
                                </li>
                                <li class="m-right10">
                                    <a id="btnNew" class="table-btn">
                                        <i class="fa fa-plus-square"></i>
                                        추가
                                    </a>
                                </li>
                                <li class="m-right10">
                                    <a id="multiFaceFormSumit" class="table-btn">
                                        <i class="fa fa-edit"></i>
                                        저장 / 수정
                                    </a>
                                </li>
                                <li class="m-right10">
                                    <span class="badge badge-secondary" id="clickInfo"></span>
                                </li>
                            </ul>
                            </sec:authorize>
                            <!--  <button type="submit" class="btn btn-primary"> 전체  </button> -->
                        </div>
                        <div class="x_content">
                            <div class="" role="tabpanel" data-example-id="togglable-tabs">

                                <ul id="objTabs" class="nav nav-tabs bar_tabs" role="tablist">
                                    <li role="presentation" class="active">
                                        <a href="#faceFormDiv" id="faceTab" role="tab" data-toggle="tab" aria-expanded="true"> 인물 <strong id="faceTabNo" >(1)</strong>
                                        </a>
                                    </li>
                                    <li role="presentation" class="">
                                        <a href="#personAllDiv" role="tab" id="personAllTab" data-toggle="tab" aria-expanded="false"> 인물 전체 <strong id="personAllTabNo" >(3)</strong>
                                        </a>
                                    </li>
                                    <li role="presentation" class="">
                                        <a href="#objTabDiv" role="tab" id="objTab" data-toggle="tab" aria-expanded="false"> 객체 <strong id="objTabNo">(1)</strong>
                                        </a>
                                    </li>
                                    <li role="presentation" class="">
                                        <a href="#placeFormDiv" id="placeTab" role="tab" id="place-tab3" data-toggle="tab" aria-expanded="false"> 장소 </a>
                                    </li>
                                    <sec:authorize access="hasAnyRole('ROLE_ADMIN,ROLE_EDITER')" var="u">
                                    <li role="presentation" class="">
                                        <a href="#qaFormDiv" id="qaTab" role="tab" id="qa-tab3" data-toggle="tab" aria-expanded="false"> QA </a>
                                    </li>
                                    </sec:authorize>
                                </ul>


                                <div id="nav-objTabContent" class="layer-edit-wrap tab-content">
                                    <div role="tabpanel" class="tab-pane fade active in" id="faceFormDiv" aria-labelledby="faceTab">
                                    </div>
                                    <div role="tabpanel" class="tab-pane fade" id="personAllDiv" aria-labelledby="personAllTab">
                                    </div>
                                    <div role="tabpanel" class="tab-pane fade" id="objTabDiv" aria-labelledby="objTab">
                                    </div>
                                    <div role="tabpanel" class="tab-pane fade" id="placeFormDiv" aria-labelledby="placeTab">
                                    </div>
                                    <sec:authorize access="hasAnyRole('ROLE_ADMIN,ROLE_EDITER')" var="u">
                                    <div role="tabpanel" class="tab-pane fade" id="qaFormDiv" aria-labelledby="qaTab">
                                        <!-- 레이어 시작 -->
                                        <div class="form-wrap">
                                            <div class="panel panel-heading">
                                                <div class="layer-name">
                                                    QA 정보 추가
                                                </div>
                                            </div>

                                            <form class="needs-validation" id="qaForm" name="qaForm">
                                                <div class="form-group edit-form">
                                                    <label class="control-label col-md-4 col-sm-4 col-xs-12"> 감정에 따른 행동
                                                    </label>
                                                    <div class="col-md-8 col-sm-8 col-xs-12">
                                                        <div class="input-group">
                                                            <input type="text" name="emotional_behavior" id="emotional_behavior" class="form-control" placeholder="감정에 따른 행동" value="">
                                                        </div>
                                                    </div>
                                                </div>

                                                <div class="form-group edit-form">
                                                    <label class="control-label col-md-4 col-sm-4 col-xs-12"> 객체간의 관계
                                                    </label>
                                                    <div class="col-md-8 col-sm-8 col-xs-12">
                                                        <div class="input-group">
                                                            <input type="text" name="relate_between_obj" id ="relate_between_obj" class="form-control" placeholder="객체간의 관계" value="">
                                                        </div>
                                                    </div>
                                                </div>

                                                <div class="form-group edit-form">
                                                    <label class="control-label col-md-4 col-sm-4 col-xs-12"> 자동묘사
                                                    </label>
                                                    <div class="col-md-8 col-sm-8 col-xs-12">
                                                        <div class="input-group">
                                                            <input type="text" name="cap" id ="cap" class="form-control" placeholder="자동묘사" value="">
                                                        </div>
                                                    </div>
                                                </div>
                                            </form>
                                        </div>
                                        <!-- 레이어 끝 -->
                                    </div>
                                    </sec:authorize>
                                    <!-- 5. QA 탭 끝 -->
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
                <!-- x_content 끝 -->
            </div>
        </div>
    </div>
    <!-- row end -->

    <!-- row 시작 -->
    <div class="row c-li-row" >
        <div class="col-md-12 col-sm-12 col-xs-12">
            <div class="x_panel">
                <div class="x_title">
                    <h2>시각정보 리스트</h2>
                    <div class="clearfix"></div>
                </div>

                <div class="x_content">
                    <div class="table-responsive" role="tabpanel" data-example-id="togglable-tabs" id="putMetaInfo">
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<!-- /page content -->
<!-- END #MAIN PANEL -->
<c:import url="../includes/script.jsp"/>
<script>
    function formBodyAct(){
        $(document).on({
            click : function (e) {
                var tgtClas= "."+$(this).attr('id');
                $('[class*=layer]').hide();
                $('[class*=ebox_]').hide();
                $('[class*=layer-edit-wrap]').show();
                $('[class*=layer-name]').show();
                $(tgtClas).show();
                $("[name=formBody]").removeClass("selected");
                $(this).addClass("selected");
                $(this).find("input[name=score]").val(1);

            }
        },'[name=formBody]');

    };
    function metaClickInfo(){
        $(document).on({
            click : function () {
                //$('[class*=layer]').show();
                //$('[name=formBody]').removeClass("alert-primary");
                var as = $(this).attr('class');
                var array = as.split(' ');
                var tgtId =  '#'+array[array.length-1];
                var tgtTab ="[href='#"+$("#nav-objTabContent").find(tgtId).parent().attr('id')+"']";
                $("#clickInfo").html(array[array.length-1]);
                $("#objTabs").find(tgtTab).tab().click();

                $("[name=formBody]").removeClass("selected");
                $(tgtId).addClass("selected");
                $(tgtId).find("input[name=score]").val(1);

                setTimeout(function(){
                    document.getElementById("nav-objTabContent").scrollTop =  document.getElementById(array[1]).offsetTop-40;
                },500);
            }
        },'.select-areas-background-area');
    }

    //레이어 변경
    var objCnt = 0;
    function onLayerChanged (event, id, areas) {
        if(objCnt==areas.length){
            fnVisualEditReset();
            displayAreas(areas);
        }else if(objCnt == 0){
            $.each(areas, function (idx, area) {
                if(id=="layer"+area.boxseq) {
                    var objxystr = parseInt(area.x) + ', ' + parseInt(area.y) + ', ' + parseInt(area.width) + ', ' + parseInt(area.height);
                    var input = $("#" + id).find("input.coordinate");
                    $(input).attr("value", objxystr);
                    $(input).attr("placeholder", objxystr);
                    area.score = 1;
                }
            });
        }
    }

    var __selectAreas ;
    function fnLoadSelectArea(ImgId){
        if(ImgId != null){
            __selectAreas = ImgId.selectAreas({
                minSize: [10, 10],
                onChanged: onLayerChanged,
                allowSelect: false
            });
        }
    }
    function fnLoadLayoutOpt() {
        var maxHeight = -1;

            $('div#edit').each(function() {
            maxHeight = maxHeight > $(this).height() ? maxHeight : $(this).height();
        });

        $(document).on("shown.bs.tab","[name='current-img-nav'],[name='represent-img-nav']",function(){
            fnVisualEditReset();
            if ($(this).attr("id") == "current-img") {
                $("#placeTab").hide();
                $("#qaTab").hide();
            } else {
                $("#placeTab").show();
                $("#qaTab").show();
            }
            var divId = $(this).attr("href");
            $(divId).find("[name='repImgSeq']").val();
            var targetImgId = $(divId).find("img");
            var repJosn = $(divId).find("[name='repJson']").val();

            var data = '';
            if (repJosn != null && repJosn != '') {
                data = JSON.parse(repJosn);

                objCnt += data.person.length;
                objCnt += data.personFull.length;
                objCnt += data.object.length;
                objCnt += data.places.length;
            }

            fnLoadSelectArea(targetImgId);
            if(targetImgId.attr("callArea")!="true" && data != ''){
                var areasArr = new Array();
                $.blockUI({
                    message: '<i class="fa fa-spinner fa-spin" style="font-size:50px;"></i> ',
                    css: {
                        border: 'none',
                        padding: '15px',
                        backgroundColor: '#000',
                        '-webkit-border-radius': '10px',
                        '-moz-border-radius': '10px',
                        opacity: .5,
                        color: '#fff'
                    }
                });
                setTimeout(function () {
                    fnVttBox(data, targetImgId, areasArr);
                    $(targetImgId).selectAreas('add', areasArr);
                }, 100);
                $(targetImgId).attr("callArea","true");
            }else{
                var areas = $(targetImgId).selectAreas('areas');
                displayAreas(areas);
            }

        });

        $(document).on("click","#allObjView",function(){
            $('[class*=layer]').show();
        });

        $(document).on("click","#multiFaceFormSumit",function(){
            var tgtId = $("#repImgTab").find(".represent-img-nav.active a").attr("href");
            var repVideoId = $(tgtId).find("[name=repVideoId]").val();
            var repIdx = $(tgtId).find("[name=repIdx]").val();
            var repFileId = $(tgtId).find("[name=repFileId]").val();
            var repImgSeq = $(tgtId).find("[name=repImgSeq]").val();
            var repSectionId = $(tgtId).find("[name=repSectionId]").val();
            var repShotid = $(tgtId).find("[name=repShotid]").val();
            var jsonArr1 = new Array();
            var jsonArr2 = new Array();
            var jsonArr3 = new Array();
            var jsonArr4 = new Array();
            var jsonArr5 = new Array();
            var param = new Object();

            for (var i = 0; i < $("[name=faceForm]").size(); i++) {
                var formJson = $("[name=faceForm]").eq(i).serializeJSON();
                var tmpJson =JSON.parse(formJson);
                var tmpCoordinate = tmpJson.faceCoordinate;
                var arrayStr =  tmpCoordinate.split(',');
                tmpJson.faceCoordinate = scaleToRealImg(arrayStr[0]) + "," + scaleToRealImg(arrayStr[1]) + ","
                    + scaleToRealImg(arrayStr[2]) + "," + scaleToRealImg(arrayStr[3]);

                jsonArr1.push(tmpJson);
            }

            for (var i = 0; i < $("[name=personAllForm]").size(); i++) {
                var formJson = $("[name=personAllForm]").eq(i).serializeJSON();
                var tmpJson =JSON.parse(formJson);
                var tmpCoordinate = tmpJson.personAllCoordinate;
                var arrayStr =  tmpCoordinate.split(',');
                tmpJson.personAllCoordinate = scaleToRealImg(arrayStr[0]) + "," + scaleToRealImg(arrayStr[1]) + ","
                    + scaleToRealImg(arrayStr[2]) + "," + scaleToRealImg(arrayStr[3]);

                jsonArr2.push(tmpJson);
            }

            for (var i = 0; i < $("[name=objForm]").size(); i++) {
                var formJson = $("[name=objForm]").eq(i).serializeJSON();
                var tmpJson =JSON.parse(formJson);
                var tmpCoordinate = tmpJson.objCoordinate;
                var arrayStr =  tmpCoordinate.split(',');
                tmpJson.objCoordinate = scaleToRealImg(arrayStr[0]) + "," + scaleToRealImg(arrayStr[1]) + ","
                    + scaleToRealImg(arrayStr[2]) + "," + scaleToRealImg(arrayStr[3]);
                jsonArr3.push(tmpJson);
            }
            for (var i = 0; i < $("[name=placeForm]").size(); i++) {
                jsonArr4.push($("[name=placeForm]").serializeJSON());
            }
            for (var i = 0; i < $("[name=qaForm]").size(); i++) {
                jsonArr5.push($("[name=qaForm]").serializeJSON());
            }

            param.faceForm = JSON.stringify(jsonArr1);
            param.personAllForm = JSON.stringify(jsonArr2);
            param.objForm = JSON.stringify(jsonArr3);
            param.placeForm = JSON.stringify(jsonArr4);
            param.qaForm = JSON.stringify(jsonArr5);
            param.repVideoId = repVideoId;
            param.repIdx = repIdx;
            param.repFileId = repFileId;
            param.repImgSeq = repImgSeq;
            param.repSectionId = repSectionId;
            param.repShotid = repShotid;


            $.ajax({
                url: '<c:url value="/visual/getPutMetaInfo"/>',
                type: 'POST',
                data: param,
                async: false,
                dataType: 'html',
                // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
                success: function (html) {
                    $("#putMetaInfo").html(html);
                    objCnt=0;
                    toast("시각정보리스트", "저장되었습니다.", "info", 5000);
                },
                error: function (xhr, ajaxOptions, thrownError) {
                    toast("시각정보리스트", "저장이 실패 하였습니다..", "info", 5000);
                },
                complete : function() {
                }
            });
        });

        $(document).on("delete-area",function(){

        });

        $('#btnNew').click(function () {
            var tgtImgId = "#"+$("#repTabContent").find("[name=repImgDiv].active").find("img").attr("id");

            var tgtEditId = $("#objTabs").find(".active a").attr("id");
            var boxtype ="";
            var boxname ="";
            if(tgtEditId == "faceTab"){
                boxtype = "face";
                boxname = "face"+ $(tgtImgId).selectAreas('areas').length;
            }else if(tgtEditId == "personAllTab"){
                boxtype = "person";
                boxname = "person";
            }else if(tgtEditId == "objTab"){
                boxtype = "object";
            }else{
                return false;
            }

            var areas = $(tgtImgId).selectAreas('areas');
            var seq = areas[areas.length-1].boxseq+1;
            var areaOptions = {
                x: Math.floor((Math.random() * 200)),
                y: Math.floor((Math.random() * 200)),
                width: Math.floor((Math.random() * 100)) + 50,
                height: Math.floor((Math.random() * 100)) + 20,
                boxtype: boxtype,
                boxname: boxname,
                score: 1,
                boxseq: seq
            };
            //fnVisualEditReset();
            $(tgtImgId).selectAreas('add', areaOptions);

            var regImgYn="Y";
            var objxystr = parseInt(areaOptions.x) + ', ' + parseInt(areaOptions.y) + ', ' + parseInt(areaOptions.width) + ', ' + parseInt(areaOptions.height);
            if (areaOptions.boxtype == "face") {
                addFaceInfo(areaOptions, objxystr, regImgYn);
            } else if (areaOptions.boxtype == "object") {
                addObjectInfo(areaOptions, objxystr, regImgYn);
            } else if (areaOptions.boxtype == "person") {
                addPersonAllInfo(areaOptions, objxystr, regImgYn);
            }
            areas = $(tgtImgId).selectAreas('areas');
            fnDataSet(areas);
        });
    }

    function delEditBox(no){
        if(no ==null){
            return false;
        }
        var tgtImgId = "#"+$("#repTabContent").find("[name=repImgDiv].active").find("img").attr("id");
        $(tgtImgId).selectAreas('remove',no);
        $("#layer"+no).remove();
    }

    function copyEditBox(no){
        if(no ==null){
            return false;
        }
        var tgtImgId = "#"+$("#repTabContent").find("[name=repImgDiv].active").find("img").attr("id");

        var areas = $(tgtImgId).selectAreas('areas');
        var seq = areas[areas.length-1].boxseq+1;
        var areaOptions = {
            x: areas[no].x,
            y: areas[no].y,
            width: areas[no].width,
            height: areas[no].height,
            boxtype: "object",
            score: 1,
            boxname: areas[no].boxname,
            boxseq: seq,
            relPerson: areas[no].relPerson,
            predIcate: areas[no].predicate,
            boxsize: areas[no].boxsize
        };
        //fnVisualEditReset();
        $(tgtImgId).selectAreas('add', areaOptions);

        var regImgYn="Y";
        var objxystr = parseInt(areaOptions.x) + ', ' + parseInt(areaOptions.y) + ', ' + parseInt(areaOptions.width) + ', ' + parseInt(areaOptions.height);
        addObjectInfo(areaOptions, objxystr, regImgYn);
    }

    function fnVisualEditReset(){
        $("#clickInfo").text("");
        $("#faceFormDiv").html("");
        $("#faceTabNo").text(0);
        $("#personAllTabNo").text(0);
        $("#objTabNo").text(0);
        $("#objTabDiv").html("");
        $("#placeFormDiv").html("");
        $("#personAllDiv").html("");
    }

    $("#btn_edit_toggle").click(function () {
        if(!confirm("확대/축소시 저장하지 않은 데이터는 초기화됩니다.\n진행하시겠습니까?")){
            return false;
        }else{
            setTimeout(function(){ fnSetHeight(); },500);
        }
        setTimeout(function(){ fnSetHeight(); },500);
    });

    function getRepreImg(idx,shotid,sectionId){
        var $div='';
        var otheruserid = $("#userSelect").val();
        $.ajax({
            url: '<c:url value="/visual/getRepreImage"/>',
            type: 'POST',
            data: {
                'repVideoId': idx,
                'repShotid': shotid,
                'repSectionId': sectionId,
                'otheruserid' : otheruserid
            },
            async: false,
            dataType: 'html',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (html) {
                $div = html;
                $("#repreImgView").html($div);
                $("#currentSectionImg").attr("src", $("#scrollImgDiv .shot-list-wrap:last").find("[name='selShotImg']").attr("src"));
                $("#userSelect").find("option[value='"+otheruserid+"']").attr("selected","selected");
                $("#userSelect").on("change",function(){
                    getRepreImg(idx,shotid,sectionId)
                });
                getMetaInfo(idx,shotid);

                if(_repImgSeqVal!= null){
                    $("#represent-img-"+_repImgSeqVal).click();
                    _repImgSeqVal=null;

                }
            },
            error: function (xhr, ajaxOptions, thrownError) {
            }
        });
    }

    var imgWidth = 0;
    function fnVttBox(data,targetImgId,areasArr){
        var ImgId="";
        if(targetImgId ==""){
            ImgId="#currentSectionImg";
        }else{
            ImgId = targetImgId;
        }
        var jsonObj = data;
        //이미지 비율  계산
        var imgRatio = ($(ImgId).width() / $(ImgId).prop("naturalWidth"));
        var arrayFaceData = jsonObj.person;
        for (var i = 0; i < arrayFaceData.length ; i++) {
            var areaOptions = {
                id: arrayFaceData[i].person_name +i,
                x: scaleToPopImg(arrayFaceData[i].rect.x,imgRatio),
                y: scaleToPopImg(arrayFaceData[i].rect.y,imgRatio),
                width: scaleToPopImg(arrayFaceData[i].rect.w,imgRatio),
                height: scaleToPopImg(arrayFaceData[i].rect.h,imgRatio),
                boxname: arrayFaceData[i].person_name,
                boxtype:"face",
                score: arrayFaceData[i].score,
                behavior: arrayFaceData[i].behavior,
                emotion: arrayFaceData[i].emotion,
                boxsize: arrayFaceData.length,
                boxseq: areasArr.length
            };
            areasArr.push(areaOptions);
        }
        var arrayPersonData = jsonObj.personFull;
        for (var i = 0; i < arrayPersonData.length ; i++) {
            var areaOptions = {
                id: 'person'+i,
                x: scaleToPopImg(arrayPersonData[i].full_rect.x,imgRatio),
                y: scaleToPopImg(arrayPersonData[i].full_rect.y,imgRatio),
                width: scaleToPopImg(arrayPersonData[i].full_rect.w,imgRatio),
                height: scaleToPopImg(arrayPersonData[i].full_rect.h,imgRatio),
                boxname: 'person',
                boxtype:"person",
                score: arrayPersonData[i].score,
                relPerson: arrayPersonData[i].person_name,
                boxsize: arrayPersonData.length,
                boxseq: areasArr.length
            };
            areasArr.push(areaOptions);
        }
        var arrayObjData = jsonObj.object;
        for (var i = 0; i < arrayObjData.length ; i++) {
            var areaOptions = {
                id: 'object'+i,
                x: scaleToPopImg(arrayObjData[i].object_rect.x,imgRatio),
                y: scaleToPopImg(arrayObjData[i].object_rect.y,imgRatio),
                width: scaleToPopImg(arrayObjData[i].object_rect.w,imgRatio),
                height: scaleToPopImg(arrayObjData[i].object_rect.h,imgRatio),
                boxname: arrayObjData[i].object_name,
                boxtype: "object",
                score: arrayObjData[i].score,
                relPerson: arrayObjData[i].person_name,
                predIcate: arrayObjData[i].predicate,
                boxsize: arrayObjData.length,
                boxseq: areasArr.length
            };
            areasArr.push(areaOptions);
        }

        var arrayPlaceData = jsonObj.places;
        for (var i = 0; i < arrayPlaceData.length ; i++) {
            var areaOptions = {
                id: arrayPlaceData[i].place+i,
                x:0,
                y: 0,
                width: 0,
                height: 0,
                boxname: arrayPlaceData[i].place,
                boxtype:"place",
                score: arrayPlaceData[i].score,
                spot: arrayPlaceData[i].spot,
                relPerson: arrayPlaceData[i].person_name,
                boxsize: arrayPlaceData.length,
                boxseq: areasArr.length
            };
            areasArr.push(areaOptions);
        }

        $("#relate_between_obj").val(jsonObj.relate_between_obj);
        $("#emotional_behavior").val(jsonObj.emotional_behavior);
        $("#cap").val(jsonObj.cap);

        imgWidth = $(ImgId).width();
        return areasArr;
    }

    function fnGetObjName (obj){
        var resultObjName = "";
        if(obj.label[0] !=null){
            resultObjName = obj.label[0];
            return resultObjName;
        }else{
            resultObjName = "unknown"
            return resultObjName;
        }
    }

    /**
     * real size img --> popimg xy 변환
     * @param val
     * @returns
     */
    function scaleToPopImg (val,ImgRatio){
        return Math.round(val * ImgRatio);
    };

    // Display areas coordinates in a div
    function displayAreas (areas,regImgYn) {
        objCnt=0;
        if((regImgYn ==null||regImgYn == 'undefined') && $(".represent-img-nav").hasClass("active")){
            regImgYn ="Y";
        }
        if(areas != null){
            $("#visualInfoEdit").show();
            /*$.each(areas, function (id, area) {
                addMetaObjEditDiv(area,regImgYn);
            }); */
            addMetaObjEditDiv(areas,regImgYn);
        }
    };

    // 편집창 분류 및 추가
    function addMetaObjEditDiv (areas,regImgYn) {
        //var no = area.id.substr(5);
        $.each(areas, function (id, area) {
            if(typeof area.boxname == "undefined"){
                area.boxname = "";
            }
            // X:area.x , Y:area.y , W:area.width , Harea.height
            var objxystr = parseInt(area.x) + ', ' + parseInt(area.y) + ', ' + parseInt(area.width) + ', ' + parseInt(area.height);
            if (area.boxtype == "face") {
                addFaceInfo(area, objxystr, regImgYn);
            } else if (area.boxtype == "place") {
                addPlaceInfo(area, objxystr, regImgYn);
            } else if (area.boxtype == "korea.place") {
            } else if (area.boxtype == "object") {
                addObjectInfo(area, objxystr, regImgYn);
            } else if (area.boxtype == "person") {
                addPersonAllInfo(area, objxystr, regImgYn);
            }
        });
        fnDataSet(areas);
        $.unblockUI();
    }

    function addFaceInfo(area, objxystr,regImgYn) {
        var RepImageYn = false;
        if(regImgYn =="Y"){
            RepImageYn = true;
        }
        var vttBoxNo=(area.id).replace("layer","");
        var $area_str = '';

        $area_str += '<div class="form-wrap" id= "'+area.id+'" name="formBody">';
        $area_str += '    <div class="panel panel-heading">';
        $area_str += '        <div class="layer-name">';
        $area_str +=              area.id;
        $area_str += '        </div>';
        if(RepImageYn) {
            $area_str += '        <div class="col-md-3 col-sm-3 col-xs-12">';
            $area_str += '            <button name="btnDel" type="submit" class="btn-sm btn-red" onclick="delEditBox('+vttBoxNo+')"> 삭제 </button>';
            $area_str += '        </div>';
        }
        $area_str += '    </div>';
        $area_str += '    <form class="needs-validation faceForm" name="faceForm" action="getPutMetaInfo">';
        $area_str += '    <input type="hidden" name="score" value="'+((area.score==null||area.score=="\"null\"")?0.7:area.score)+'">';

        $area_str += '    <div class="form-group edit-form">';
        $area_str += '        <label class="control-label col-md-3 col-sm-3 col-xs-12" > 이름</label>';
        $area_str += '        <div class="col-md-9 col-sm-9 col-xs-12">';
        $area_str += '            <select class="form-control" name = "faceName" id="faceName_' +area.boxseq+'">';
        <c:forEach var="map" items="${codeMap01}" varStatus="i">
        $area_str += '                <option value="${map.code_name}">${map.code_name}</option>';
        </c:forEach>
        $area_str += '            </select>';
        $area_str += '       </div>';
        $area_str += '     </div>';

        $area_str += '     <div class="form-group edit-form">';
        $area_str += '        <label class="control-label col-md-3 col-sm-3 col-xs-12" > 얼굴좌표 </label>';
        $area_str += '        <div class="col-md-9 col-sm-9 col-xs-12">';
        $area_str += '            <input type="text" class="form-control form-control-sm coordinate" name="faceCoordinate" ';
        $area_str += '                   placeholder="' + objxystr + '" value="'+ objxystr +'" readonly/>';
        $area_str += '        </div>';
        $area_str += '     </div>';
        if(RepImageYn){
            $area_str += '    <div class="form-group edit-form">';
            $area_str += '        <label class="control-label col-md-3 col-sm-3 col-xs-12" > 감정 </label>';
            $area_str += '       <div class="col-md-9 col-sm-9 col-xs-12">';
            $area_str += '            <select class="form-control" name="faceEmotion" id="faceEmotion_' +area.boxseq+'">';
            $area_str += '                <option value="" selected disabled hidden>선택</option>';
            <c:forEach var="map" items="${codeMap03}" varStatus="i">
            $area_str += '                <option value="${map.code_name}" <c:if test="${map.code_name}== '+area.boxname+'">selected="selected"</c:if>> ${map.code_name}</option>';
            </c:forEach>
            $area_str += '            </select>';
            $area_str += '        </div>';
            $area_str += '     </div>';

            $area_str += '    <div class="form-group edit-form">';
            $area_str += '        <label class="control-label col-md-3 col-sm-3 col-xs-12" >행동 </label>';
            $area_str += '       <div class="col-md-9 col-sm-9 col-xs-12">';
            $area_str += '            <select class="form-control" name="faceAction" id="faceAction_'+area.boxseq+'">';
            $area_str += '               <option value="" selected disabled hidden>선택</option>';
            <c:forEach var="map" items="${codeMap02}" varStatus="i">
                $area_str += '               <option value="${map.code_name}">${map.code_name}</option>';
            </c:forEach>
            $area_str += '            </select>';
            $area_str += '        </div>';
        }
        $area_str += '     </div>';
        $area_str += '     </form>';
        $area_str += ' </div>';
        $("#faceFormDiv").append($area_str);
        var faceNme = "#faceName_"+area.boxseq;
        var faceEmotion = "#faceEmotion_"+area.boxseq;
        var faceAction = "#faceAction_"+area.boxseq;
        var findVal =$(faceNme).val(area.boxname);
        var findVal2 =$(faceEmotion).val(area.emotion);
        var findVal3 =$(faceAction).val(area.behavior);
        if(findVal != null){
            $(faceNme).val(area.boxname).prop("selected","selected");
        }else{
            $(faceNme).val('none').prop("selected","selected");
        }
        if(findVal2 != null){
            $(faceEmotion).val(area.emotion).prop("selected","selected");
        }
        if(findVal3 != null){
            $(faceAction).val(area.behavior).prop("selected","selected");
        }
        var count = $("form[name=faceForm]").length;
        $("#faceTabNo").text(count);



    }

    function addPersonAllInfo(area, objxystr,regImgYn) {
        var RepImageYn =false;
        if(regImgYn =="Y"){
            RepImageYn = true;
        }
        var $area_str = '';
        var vttBoxNo=(area.id).replace("layer","");


        $area_str += '<div class="form-wrap" id= "'+area.id+'" name="formBody">';
        $area_str += '    <form name="personAllForm">';
        $area_str += '    <input type="hidden" name="score" value="'+((area.score==null||area.score=="\"null\"")?0.7:area.score)+'">';
        $area_str += '    <div class="panel panel-heading">';
        $area_str += '        <div class="layer-name">';
        $area_str +=             area.id;
        $area_str += '        </div>';
        $area_str += '        <div class="col-md-3 col-sm-3 col-xs-12">';
        if(RepImageYn) {
            $area_str += '            <button name="btnDel" onclick="delEditBox('+vttBoxNo+')" type="submit" class="btn-sm btn-red"> 삭제 </button>';
        }
        $area_str += '        </div>';
        $area_str += '    </div>';

        $area_str += '    <div class="form-group edit-form">';
        $area_str += '        <label class="control-label col-md-3 col-sm-3 col-xs-12" > 이름';
        $area_str += '        </label>';
        $area_str += '        <div class="col-md-9 col-sm-9 col-xs-12">';
        $area_str += '            <select name = "personAllName" class="form-control" id="personAll_'+area.boxseq+'">';
        $area_str += '                <option value="" selected disabled hidden>선택</option>                           ';
        <c:forEach var="map" items="${codeMap01}" varStatus="i">
        $area_str += '                 <option value="${map.code_name}">${map.code_name}</option>';
        </c:forEach>
        $area_str += '            </select>';
        $area_str += '        </div>';
        $area_str += '    </div>';

        $area_str += '    <div class="form-group edit-form">';
        $area_str += '        <label class="control-label col-md-3 col-sm-3 col-xs-12" > 전체좌표 </label>';
        $area_str += '        <div class="col-md-9 col-sm-9 col-xs-12">';
        $area_str += '            <input type="text" class="form-control form-control-sm coordinate"            ';
        $area_str += '                   name="personAllCoordinate" value= "'+objxystr+'" placeholder="'+objxystr+'" readonly>';
        $area_str += '        </div>';
        $area_str += '    </div>';
        $area_str += '    </form>';
        $area_str += '</div>';



        $("#personAllDiv").append($area_str);
        var personAll = "#personAll_"+area.boxseq;
        var findVal =$(personAll).val(area.relPerson);
        if(findVal != null){
            $(personAll).val(area.relPerson).prop("selected","selected");
        }

        var count = $("form[name=personAllForm]").length;
        $("#personAllTabNo").text(count);
    }

    function addPlaceInfo(area,objxystr,regImgYn) {
        var RepImageYn =false;
        if(regImgYn =="Y"){
            RepImageYn = true;
        }

        var $area_str = '';
        var vttBoxNo=(area.id).replace("layer","");



        $area_str += '<div class="form-wrap"  id= "'+area.id+'" name="formBody">';
        $area_str += '    <div class="panel panel-heading">';
        $area_str += '      <div class="layer-name">';
        $area_str +=            area.id;
        $area_str += '      </div>';
       /* if(RepImageYn) {
            $area_str += '      <div class="col-md-3 col-sm-3 col-xs-12">';
            $area_str += '          <button name="btnDel" type="submit" onclick="delEditBox('+vttBoxNo+')" class="btn-sm btn-red"> 삭제 </button>';
            $area_str += '      </div>';
        }*/
        $area_str += '    </div>';

        $area_str += '    <form name = "placeForm">';
        $area_str += '    <input type="hidden" name="score" value="'+((area.score==null||area.score=="\"null\"")?0.7:area.score)+'">';
        $area_str += '    <div class="form-group edit-form">';
        $area_str += '      <label class="control-label col-md-3 col-sm-3 col-xs-12"> 장소';
        $area_str += '      </label>';
        $area_str += '      <div class="col-md-9 col-sm-9 col-xs-12">';
        $area_str += '          <select class="form-control" name = "place" id="place_'+area.boxseq+'">';
        $area_str += '              <option value="" selected disabled hidden>선택</option>';
        $area_str += '              <c:forEach var="map" items="${codeMap05}" varStatus="i">';
        $area_str += '              <option value="${map.code_name}">${map.code_name}</option> ';
        $area_str += '              </c:forEach>';
        $area_str += '          </select>';
        $area_str += '      </div>';
        $area_str += '    </div>';

        $area_str += '    <div class="form-group edit-form">';
        $area_str += '      <label class="control-label col-md-3 col-sm-3 col-xs-12"> 세부장소';
        $area_str += '      </label>';
        $area_str += '      <div class="col-md-9 col-sm-9 col-xs-12">';
        $area_str += '          <select class="form-control" name="placeDetail" id="placeDetail_'+area.boxseq+'">';
        $area_str += '              <option value="" selected>선택</option>';
        <c:forEach var="map" items="${codeMap08}" varStatus="i">
        $area_str += '              <option value="${map.code_name}">${map.code_name}</option> ';
        </c:forEach>
        $area_str += '          </select>';
        $area_str += '      </div>';
        $area_str += '    </div>';

        $area_str += '    <div class="form-group edit-form">';
        $area_str += '      <label class="control-label col-md-3 col-sm-3 col-xs-12" > 관련인물 </label>';
        $area_str += '      <div class="col-md-9 col-sm-9 col-xs-12">';
        $area_str += '          <select class="form-control" name="relatedPeople" id="relatedPeople_'+area.boxseq+'">';
        $area_str += '              <option value="" selected>선택</option>';
        <c:forEach var="map" items="${codeMap04}" varStatus="i">
        $area_str += '              <option value="${map.code_name}">${map.code_name}</option> ';
        </c:forEach>
        $area_str += '          </select>';
        $area_str += '      </div>';
        $area_str += '    </div>';
        $area_str += '    </form>';
        $area_str += '</div>';
        $("#placeFormDiv").append($area_str);
        var tgtId = "#place_"+area.boxseq;
        var tgtId2 = "#placeDetail_"+area.boxseq;
        var tgtId3 = "#relatedPeople_"+area.boxseq;
        var findVal =$(tgtId).val(area.boxname);
        if(findVal != null){
            $(tgtId).val(area.boxname).prop("selected","selected");
        }
        $(tgtId2).val(area.spot);
        if(findVal != null){
            $(tgtId3).val(area.relPerson).prop("selected","selected");
        }
    }
    function addObjectInfo(area,objxystr,regImgYn) {
        var RepImageYn =false;
        if(regImgYn =="Y"){
            RepImageYn = true;
        }

        var $area_str = '';
        var vttBoxNo=(area.id).replace("layer","");


        $area_str += '<div class="form-wrap" id= "'+area.id+'" name="formBody">';
        $area_str += '    <form name="objForm" onsubmit="return false;">';
        $area_str += '    <input type="hidden" name="score" value="'+((area.score==null||area.score=="\"null\"")?0.7:area.score)+'">';
        $area_str += '    <div class="panel panel-heading">';
        $area_str += '        <div class="layer-name">';
        $area_str += area.id
        $area_str += '        </div>';
        if(RepImageYn) {
            $area_str += '        <div class="col-md-2 col-sm-2 col-xs-6">';
            $area_str += '            <button name="btnDel" type="submit" class="btn-sm btn-red" onclick="delEditBox('+vttBoxNo+')">삭제 </button>';
            $area_str += '        </div>';
            $area_str += '        <div class="col-md-2 col-sm-2 col-xs-6">';
            $area_str += '            <button name="btnDel" type="submit" class="btn-sm btn-success" onclick="copyEditBox('+vttBoxNo+')">복제 </button>';
            $area_str += '        </div>';
            $area_str += '    </div>';
        }

        $area_str += '    <div class="form-group edit-form">';
        $area_str += '        <label class="control-label col-md-3 col-sm-3 col-xs-12" >관련 인물명';
        $area_str += '        </label>';
        $area_str += '        <div class="col-md-9 col-sm-9 col-xs-12">';
        $area_str += '            <select class="form-control" name="personName" id="personName_'+ area.boxseq+'">';
        <c:forEach var="map" items="${codeMap01}" varStatus="i">
        $area_str += '                <option value="${map.code_name}">${map.code_name}</option>';
        </c:forEach>
        $area_str += '            </select>';
        $area_str += '        </div>';
        $area_str += '    </div>';

        $area_str += '    <div class="form-group edit-form">';
        $area_str += '        <label class="control-label col-md-3 col-sm-3 col-xs-12" >관련객체명;'
        $area_str += '        </label>';
        $area_str += '            <div class="col-md-9 col-sm-9 col-xs-12">';
        $area_str += '            <select class="form-control" name ="relatedObj" id="relatedObj_' + area.boxseq+'">';
        <c:forEach var="map" items="${codeMap06}" varStatus="i">
        $area_str += '                <option value="${map.code_name}">${map.code_name}</option>';
        </c:forEach>
        $area_str += '            </select>';
        $area_str += '        </div>';
        $area_str += '    </div>';

        $area_str += '    <div class="form-group edit-form">';
        $area_str += '        <label class="control-label col-md-3 col-sm-3 col-xs-12" >객체 좌표 </label>';
        $area_str += '        <div class="col-md-9 col-sm-9 col-xs-12">';
        $area_str += '            <input type="text" class="form-control form-control-sm coordinate" name="objCoordinate" ';
        $area_str += '                   placeholder="'+objxystr+'" value ="'+objxystr+'" readonly/>';
        $area_str += '        </div>';
        $area_str += '    </div>';

        $area_str += '    <div class="form-group edit-form">';
        $area_str += '        <label class="control-label col-md-3 col-sm-3 col-xs-12" >서술어 </label>';
        $area_str += '        <div class="col-md-9 col-sm-9 col-xs-12">';
        $area_str += '            <select class="form-control" name="description" id="description_'+ area.boxseq+'">';
        <c:forEach var="map" items="${codeMap07}" varStatus="i">
        $area_str += '                <option value="${map.code_name}">${map.code_name}</option>';
        </c:forEach>
        $area_str += '            </select>';
        $area_str += '        </div>';
        $area_str += '    </div>';
        $area_str += '    </form>';
        $area_str += '</div>';

        $("#objTabDiv").append($area_str);
        var tgtId = "#relatedObj_"+area.boxseq;
        tgtId = tgtId.replace(/\//gi, "\\/");

        tgtId = tgtId.replace(/\(/gi, "\\(");
        tgtId = tgtId.replace(/\)/gi, "\\)");
        var findVal =$(tgtId).val(area.boxname);

        var tgtId2 = "#personName_"+area.boxseq;
        var findVal2 =$(tgtId2).val(area.relPerson);
        var tgtId3 = "#description_"+area.boxseq;
        var findVal3 =$(tgtId3).val(area.predIcate);
        if(findVal != null){
            $(tgtId).val(area.boxname).prop("selected","selected");
        }
        if(findVal2 != null){
            $(tgtId2).val(area.relPerson).prop("selected","selected");
        }
        if(findVal3 != null){
            $(tgtId3).val(area.predIcate).prop("selected","selected");
        }
        var count = $("form[name=objForm]").length;
        $("#objTabNo").text(count);

    }

    //레이어 page데이터 유지를 위한 이벤트
    var fnDataSet = function(areas){
    $("[name=formBody] select,input[name=placeDetail]").on("change",function(){
            var layerId = $(this).parents("[name=formBody]").attr("id");

            var value = $(this).val();
            var key = $(this).attr("name");
            $.each(areas, function (idx, area) {
                if(layerId =="layer"+area.boxseq) {
                    area.score=1;
                    if(key=="faceName"){
                        area.boxname = value;
                    }else if(key=="faceAction"){
                        area.behavior = value;
                    }else if(key=="faceEmotion"){
                        area.emotion = value;
                    }else if(key=="personAllName"){
                        area.relPerson = value;
                    }else if(key=="personName"){
                        area.relPerson = value;
                    }else if(key=="relatedObj"){
                        area.boxname = value;
                    }else if(key=="description"){
                        area.predIcate = value;
                    }else if(key=="place"){
                        area.boxname = value;
                    }else if(key=="placeDetail"){
                        area.spot = value;
                    }else if(key=="relatedPeople"){
                        area.relPerson = value;
                    }
                }
            });

        });
    }

    function getSectionList(idx) {
        var $div='';
        $.ajax({
            url: '<c:url value="/visual/getSectionList"/>',
            type: 'POST',
            data: {'idx': idx},
            async: false,
            dataType: 'html',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (html) {
                $div = html;
                $("#sectionList").html($div);
                $("tr[name='secTr']:first").click();
            },
            error: function (xhr, ajaxOptions, thrownError) {
                MSG.alert("getSectionList"+thrownError);
            }
        });
    }
    function updateShot(idx, shotid, delflag){
        $.ajax({
            url: '<c:url value="/visual/updateShot"/>',
            type: 'POST',
            data: {
                'videoid': idx,
                'shotid':shotid,
                'delflag':delflag
                  },
            dataType: 'json',
            success: function (data) {
                if(data.result==1){
                    getSectionList(idx);
                    toggleUpdateShot();
                }
            },
            error: function (xhr, ajaxOptions, thrownError) {
                MSG.alert(thrownError);
            }
        });
    }
    function toggleUpdateShot(){
        $(".section-num").find("button").toggle();
        $("[name=secTr][delflag=true]").toggle();
    }

    function getSectionShotList(idx, shotid,assetfilepath, assetfilename) {
        $('.table tbody tr[name="secTr"]').removeClass('ui-selected');
        $('#secTr_'+shotid).addClass('ui-selected');

        var $div='';
        $.ajax({
            url: '<c:url value="/visual/getSectionShotList"/>',
            type: 'POST',
            data: {
                'idx': idx,
                'shotid' : shotid,
                'assetfilepath': assetfilepath,
                'assetfilename': assetfilename
            },
            dataType: 'html',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (html) {
                $div = html;
                $("#scrollImgDiv").html($div);
                $("#sectionList").removeClass("col-12");
                $("#sectionList").addClass("col-2");
                $("#repreImgView").html("");
                $("#putMetaInfo").html("");
                $("#visualInfoEdit").hide();
                $("#scrollImgDiv").find("[name='selShotImgLi']:first").addClass("ui-selected");
                $("#scrollImgDiv").find("[name='selShotImgLi']:first").click();

                var shotid = $("#scrollImgDiv .shot-list-wrap:last").find("[name='shotid']").val();
                var sectionId = $("#scrollImgDiv .shot-list-wrap:last").find("[name='sectionId']").val();
                getRepreImg(${idx},shotid,sectionId);
                $( function() {
                    $( "#scrollImgDiv .shot-list-wrap" ).selectable({
                        filter: 'div[name=selShotImgLi]',
                        stop:function(){
                            fnVisualEditReset();
                            $("#currentSectionImg").attr("src", $(".ui-selected:last").find("[name='selShotImg']").attr("src"));
                        }
                    });
                } );
            },
            error: function (xhr, ajaxOptions, thrownError) {
            }
        });
    }

    $(document).on("click","#repImgSave",function(){
        getPutRepreImg();
    });
    var _repImgSeqVal;
    function getPutRepreImg() {
        var repImgSeqVal = 1;
        if($("#repImgSelect").val() == ""){
            if($("#scrollImgDiv .ui-selected").length==1) {
                toast("대표이미지", "대표이미지 순서를 선택하세요", "info", 5000);
                return false;
            }else if($("#scrollImgDiv .ui-selected").length!=3){
                toast("대표이미지", "대표이미지 3개를 선택하세요", "info", 5000);
                return false;
            }
        }else{
            if($("#scrollImgDiv .ui-selected").length!=1){
                toast("대표이미지", "대표이미지 1개만 선택하세요", "info", 5000);
                return false;
            }
            repImgSeqVal = parseInt($("#repImgSelect").val());
        }
        var paramArea = {};
        $("#scrollImgDiv .ui-selected").each(function(index){
            var paramJson = "{";
            var repShotid = $(this).find("[name='shotid']").val();
            var repSectionId = $(this).find("[name='sectionId']").val();
            var repSectionPath = $(this).find("[name='sectionPath']").val();
            var repFileId = $(this).find("[name='sectionShotId']").val();
            var repShotFileName = $(this).find("[name='shotFileName']").val();
            var repImgUrl = $(this).find("[name='selShotImg']").attr("src");
            var repVideoId = '${idx}';
            var repImgSeq = repImgSeqVal+index;
            paramJson += '"repShotid":"'+repShotid+'",';
            paramJson += '"repSectionId":"'+repSectionId+'",';
            paramJson += '"repSectionPath":"'+repSectionPath+'",';
            paramJson += '"repFileId":"'+repFileId+'",';
            paramJson += '"repShotFileName":"'+repShotFileName+'",';
            paramJson += '"repImgUrl":"'+repImgUrl+'",';
            paramJson += '"repVideoId":"'+repVideoId+'",';
            paramJson += '"repImgSeq":"'+repImgSeq+'"}';
            paramArea[repImgSeq] = paramJson;
        });


        $.blockUI({
            message: '<i class="fa fa-spinner fa-spin" style="font-size:50px;"></i> 저장중...',
            css: {
                border: 'none',
                padding: '15px',
                backgroundColor: '#000',
                '-webkit-border-radius': '10px',
                '-moz-border-radius': '10px',
                opacity: .5,
                color: '#fff'
            }
        });
        $.ajax({
            url: '<c:url value="/visual/getPutRepreImg"/>',
            type: 'POST',
            data: paramArea,
            dataType: 'json',
            // contentType: "application/x-www-form-urlencoded; charset=UTF-8",
            success: function (json) {
                var result="";
                if(json.success){
                    result= "성공"
                }else{
                    result= "실패"
                }
                toast("저장여부: " + result, json.message, "info", 5000);
                $.unblockUI();

                $("#sectionList").find(".ui-selected").click();
                $("#scrollImgDiv").find("[name='selShotImgLi']:first").click();
                _repImgSeqVal = repImgSeqVal;
            },
            error: function (xhr, ajaxOptions, thrownError) {
                $.unblockUI();
            }
        });
    }
    function scaleToRealImg (val){
        var tgtDivId = $("#repImgTab").find(".represent-img-nav.active a").attr("href");
        var tgtImgId = "#"+$(tgtDivId).find('img').attr('id');
        var imgRatio = $(tgtImgId).width() / $(tgtImgId).prop("naturalWidth");
        //var imgRatio = ($(tgtImgId).width() / $(tgtImgId).prop("naturalWidth"));
        return Math.ceil(val / imgRatio);
    }

    function getMetaInfo(idx,shotid){
        if(idx==null){
            return false;
        }
        if(shotid==null){
            return false;
        }
        var otheruserid = $("#userSelect").val();
        $.ajax({
            url: '<c:url value="/visual/getMetaInfo"/>',
            type: 'POST',
            data: {
                "idx":idx,
                "shotId":shotid,
                'otheruserid' : otheruserid
            },
            async: false,
            dataType: 'html',
            success: function (html) {
                $("#putMetaInfo").html(html);
                $("#current-img").tab('show');
            }
        });
    }
    $(window).resize(function(){

        var divId = $(".represent-img-nav.active a").attr("href");
        var targetImgId = $(divId).find("img");
        if(divId == null){
            return;
        }
        var areas = $(targetImgId).selectAreas('areas');

        var imgRatio  = ($(targetImgId).width() / imgWidth);
        for (var i = 0; i < areas.length ; i++) {
            areas[i].x = scaleToPopImg(areas[i].x,imgRatio);
            areas[i].y = scaleToPopImg(areas[i].y,imgRatio);
            areas[i].width = scaleToPopImg(areas[i].width,imgRatio);
            areas[i].height = scaleToPopImg(areas[i].height,imgRatio);
            var $layer = $(".layer"+areas[i].boxseq);
            var $delete = $(".delete-area.ebox_"+areas[i].boxseq);
            var $resize = $(".select-areas-resize-handler.ebox_"+areas[i].boxseq);
            $layer.css("left",areas[i].x);
            $layer.css("top",areas[i].y);
            $layer.css("width",areas[i].width);
            $layer.css("height",areas[i].height);
            $delete.css("left",areas[i].x+areas[i].width);
            $delete.css("top",areas[i].y);
            $resize.css("left",areas[i].x+areas[i].width);
            $resize.css("top",areas[i].y+areas[i].height);
            var rectAreas = areas[i].x + ", " + areas[i].y + ", "+ areas[i].width + ", " + areas[i].height;
            $("#layer"+areas[i].boxseq).find("input.coordinate").attr("placeholder",rectAreas);
            $("#layer"+areas[i].boxseq).find("input.coordinate").attr("value",rectAreas);
        }

        imgWidth = $(targetImgId).width();
    });
    $(document).ready(function() {
        getSectionList(<c:out value="${idx}"/>); // 리스트 idx 넘겨받음.(ex) 624
        fnLoadLayoutOpt();
        metaClickInfo();
        formBodyAct();
    });
</script>
<c:import url="../includes/footer.jsp"/>