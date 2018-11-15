<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include  file="../includes/header.jsp"%>

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
                            <li class="breadcrumb-item active">
                                콘텐츠 리스트
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
                            <h2> 콘텐츠 리스트 <small>Content list</small></h2>
                            <ul class="nav navbar-right panel_toolbox">
                                <li>
                                    <a class="table-btn" id="btn_vtt_visual_edit">
                                        <i class="fa fa-file-code-o " ></i>
                                        &nbsp; VTT 시각정보 편집
                                    </a>
                                </li>
                                <li>
                                    <a class="table-btn" id="btn_vtt_section_edit">
                                        <i class="fa fa-file-code-o " ></i>
                                        &nbsp; VTT 구간정보 편집
                                    </a>
                                </li>
                                <li>
                                    <a class="table-btn" id="btn_catalog">
                                        <i class="fa fa-file-code-o " ></i>
                                        &nbsp; 카탈로깅
                                    </a>
                                </li>
                                <li>
                                    <a class="table-btn" id="btn_delete">
                                        <i class="fa fa-file-code-o " ></i>
                                        &nbsp; 삭제
                                    </a>
                                </li>
                                <li>
                                    <a class="table-btn" id="btn_download">
                                        <i class="fa fa-file-code-o " ></i>
                                        &nbsp; 다운로드
                                    </a>
                                </li>
                                <li>
                                    <a class="table-btn" id="btn_storyboard">
                                        <i class="fa fa-file-code-o " ></i>
                                        &nbsp; 스토리보드
                                    </a>
                                </li>
                                <li>
                                    <a class="table-btn" id="btn_play">
                                        <i class="fa fa-file-code-o " ></i>
                                        &nbsp; 객체인식재생
                                    </a>
                                </li>
                                <li>
                                    <a class="table-btn" data-toggle="modal" data-target="#addContentsModal">
                                        <i class="fa fa-file-code-o " ></i>
                                        &nbsp; 콘텐츠 등록
                                    </a>
                                </li>
                            </ul>
                            <div class="clearfix"></div>
                        </div>

                        <div class="x_content">
                            <div class="table-responsive">
                                <table class="table table-striped jambo_table bulk_action">
                                    <thead>
                                    <tr class="">
                                        <th>
                                            <input type="checkbox" id="check-all" class="flat">
                                        </th>
                                        <th class="" > 순번 </th>
                                        <th class=""> IDX </th>
                                        <th class=""> 카탈로깅 </th>
                                        <th class=""> 제목 </th>
                                    </tr>
                                    </thead>

                                    <tbody id="content_assets">
                                    </tbody>
                                </table>
                            </div>
                        </div>

                        <div class="page-wrap">
                            <div class="col-sm-3">
                                <div class="dataTables_info" id="example2_info" role="status" aria-live="polite">
                                    Showing 1 to 10 of 57 entries
                                </div>
                            </div>

                            <div class="col-sm-5">
                                <div class="dataTables_paginate paging_simple_numbers" id="example2_paginate">
                                    <ul class="pagination"></ul>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <!-- /page content -->

    </div>
    <!--/main_container  -->
</div>
<!--/container body  -->
<c:import url="../includes/script.jsp"/>
<script>

    var params = {};
    params.offset = "${queryMap.offset}";
    params.limit = "${queryMap.limit}";

    $(document).ready(function(){
        $.ajax({
            type: "GET",
            url: "<c:url value='/content/list' />",
            data: params,
            dataType: "html",
            success: function(response) {
                $("#content_assets").html(response);
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

        $("#btn_vtt_visual_edit").on("click", function() {
            var $T = $("#content_assets .ui-selectee.ui-selected");
            if($T.length == 0) {
                toast("Content", "콘텐츠를 선택하세요.!", "info", 5000);
                return false;
            }
            var param = {};
            param.idx = $T.find("input[name=idx]").val();
            location.href = "<c:url value='/visual/main' />?" + $.param(param);

        });

        $("#btn_vtt_section_edit").on("click", function() {
            var $T = $("#content_assets .ui-selectee.ui-selected");
            if($T.length == 0) {
                toast("Content", "콘텐츠를 선택하세요.!", "info", 5000);
                return false;
            }
            var param = {};
            param.idx = $T.find("input[name=idx]").val();
            param.viewChk = 'section';
            location.href = "<c:url value='/visual/getSecInfoMain' />?" + $.param(param);

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

            MSG.confirm("재시도를 요청하겠습니까?", function(){
                $.ajax({
                    type: "GET",
                    url: "<c:url value='/content/retry/catalog/' />" + idx,
                    dataType: "json",
                    success: function(response) {
                        MSG.alert("재시도를 요청했습니다",function() {
                            location.href = "<c:url value='/content' />";
                        });
                    },
                    error: function(xhr, opt, err) {
                        MSG.error("오류가 발생했습니다.!" + err, function() {
                            location.href = "<c:url value='/content' />";
                        });
                        return false;
                    }
                });
            });
        });

        $("#btn_delete").on("click", function() {
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

            MSG.confirm("삭제를 하겠습니까?", function () {
                $.ajax({
                    type: "GET",
                    url: "<c:url value='/content/delete/' />" + idx,
                    dataType: "json",
                    success: function(response) {
                        MSG.alert("삭제를 했습니다",function() {
                            location.href = "<c:url value='/content' />";
                        });
                    },
                    error: function(xhr, opt, err) {
                        MSG.error("오류가 발생했습니다.!" + err, function() {
                            location.href = "<c:url value='/content' />";
                        });
                    }
                });
            });
        });

        $("#btn_download").on("click", function() {
            var $T = $("#content_assets .ui-selectee.ui-selected");
            if($T.length == 0) {
                toast("Content", "콘텐츠를 선택하세요.!", "info", 5000);
                return false;
            } else {
                var param = {};
                param.idx = $T.find("input[name=idx]").val();
                param.userId = '${user.username}';
                location.href = "<c:url value='/jsonFileDown' />?" + $.param(param);
            }
        });

        $("#btn_storyboard").on("click", function() {
            var $T = $("#content_assets .ui-selectee.ui-selected");
            if($T.length == 0) {
                toast("Content", "콘텐츠를 선택하세요.!", "info", 5000);
                return false;
            } else {
                var param = {};
                param.idx = $T.find("input[name=idx]").val();
                location.href = "<c:url value='/storyboard' />?" + $.param(param);
            }
        });

        $("#btn_play").on("click", function() {
            console.log("#btn_play");
            var $T = $("#content_assets .ui-selectee.ui-selected");
            if($T.length == 0) {
                toast("Content", "콘텐츠를 선택하세요.!", "info", 5000);
                return false;
            } else {
                var param = {};
                param.idx = $T.find("input[name=idx]").val();
                location.href = "<c:url value='/storyboard/play' />?" + $.param(param);
            }
        });
    });
</script>
<c:import url="write_script.jsp"/>
<c:import url="../includes/footer.jsp"/>