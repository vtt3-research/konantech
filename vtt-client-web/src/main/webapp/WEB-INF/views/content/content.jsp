<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>
<c:import url="../includes/header.jsp"/>

<%--<c:import url="content_left.jsp"/>--%>

<style>
    .body-content{height:100%;width:100%;}
</style>

<div class="container-fluid">
    <!-- Breadcrumbs-->
    <ol class="breadcrumb">
        <li class="breadcrumb-item">
            <a href="#">콘텐츠</a>
        </li>
        <li class="breadcrumb-item active">콘텐츠 리스트</li>
    </ol>
    <div class="row">
        <div class="col-12">
            <!-- RIBBON -->
            <span class="item"></span>
            <span class="ribbon-button-alignment pull-right" style="margin-right:25px">
            <span id="btn_transcoding" class="btn btn-ribbon hidden-xs" data-title="add"><i class="fa fa-video-camera txt-color-yellow"></i> 트랜스코딩</span>
            <span id="btn_catalog" class="btn btn-ribbon hidden-xs" data-title="add"><i class="fa fa-file-picture-o "></i> 카탈로깅</span>
            <span id="btn_delete" class="btn btn-ribbon hidden-xs" data-title="add"><i class="fa fa-trash-o txt-color-customRed"></i> 삭제</span>
            <span id="btn_download" class="btn btn-ribbon hidden-xs" data-title="add"><i class="fa fa-download txt-color-customGreen"></i> 다운로드</span>
            <span id="btn_storyboard" class="btn btn-ribbon hidden-xs" data-title="add"><i class="fa fa-picture-o txt-color-orange"></i> 스토리보드</span>
            </span>
        </div>
        <div class="col-12">
            <h1>콘텐츠</h1>
            <p>콘텐츠를 관리합니다.</p>
        </div>
    </div>
    <div id="content"> </div>
</div>



<c:import url="../includes/script.jsp"/>
<script>

    var params = {};
    params.offset = "${queryMap.offset}";
    params.limit = "${queryMap.limit}";

    $.ajax({
        type: "GET",
        url: "<c:url value='/content/list' />",
        data: params,
        dataType: "html",
        success: function(response) {
            $("#content").html(response);
        },
        error: function(xhr, opt, err) {
            MSG.error("오류가 발생했습니다.!");
            return false;
        }
    });


    // btn
    $("#btn_transcoding").on("click", function() {
        //
    });

    $("#btn_catalog").on("click", function() {
        var $T = $("#content_assets .ui-selectee.ui-selected");
        if($T.length == 0) {
            toast("Content", "콘텐츠를 선택하세요.!", "info", 5000);
            return false;
        }
        var ids = [];
        $T.each(function() {
            ids.push($(this).find("input[name=idx]").val());
        });
        var idx = ids.join("|");

        // MSG.confirm("재시도", "재시도를 요청하겠습니까?", function () {
        MSG2.confirm("재시도를 요청하겠습니까?", function () {
            $.ajax({
                type: "GET",
                url: "<c:url value='/content/retry/catalog/' />" + idx,
                dataType: "json",
                success: function(response) {
                    MSG.alert("SUCCESS", "재시도를 요청했습니다", "success", 3000);
                },
                error: function(xhr, opt, err) {
                    MSG.error("오류가 발생했습니다.!" + err);
                    return false;
                }
            });
        });

    });

    $("#btn_delete").on("click", function() {
        //
    });

    $("#btn_download").on("click", function() {
        //
    });

    $("#btn_storyboard").on("click", function() {
        var $T = $("#content_assets .ui-selectee.ui-selected");
        if($T.length == 0) {
            toast("Content", "콘텐츠를 선택하세요.!", "info", 5000);
            return false;
        } else if($T.length > 1) {
            toast("Content", "한개만 선택해 주세요.!", "info", 5000);
            return false;
        } else {
            var param = {};
            param.idx = $T.find("input[name=idx]").val();
            location.href = "<c:url value='/storyboard' />?" + $.param(param);
        }
    });




</script>
<c:import url="../includes/footer.jsp"/>