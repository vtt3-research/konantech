<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>
<c:import url="../includes/header.jsp"/>

<!-- dropzone style -->
<link rel="stylesheet" type="text/css" href="<c:url value="/resources/custom/js/dropzone/dropzone.min.css"/>">



<div class="container-fluid">
    <!-- Breadcrumbs-->
    <ol class="breadcrumb">
        <li class="breadcrumb-item">
            <a href="#">콘텐츠 등록</a>
        </li>
        <li class="breadcrumb-item active">등록</li>
    </ol>
    <div class="row">
        <div class="col-12">
        </div>
        <div class="col-12">
            <h1>콘텐츠 등록</h1>
        </div>
    </div>

    <div id="content">
        <div class="row">

            <!-- NEW WIDGET START -->
            <article class="col-xs-12 col-sm-12 col-md-12 col-lg-12 sortable-grid ui-sortable">
                <div>
                    <div>
                        <div class="widget-body no-padding">

                            <div id="write_form" class="smart-form">
                                <fieldset>
                                    <section>
                                        <label class="label">제목</label>
                                        <label class="input">
                                            <i class="icon-append fa fa-list"></i>
                                            <input type="text" name="title" id="write_title" placeholder="제목을 입력하세요">
                                            <b class="tooltip tooltip-bottom-right" title="This is my image's tooltip message!" >제목을 입력하세요</b>
                                        </label>
                                    </section>
                                    <section>
                                        <label class="label">내용</label>
                                        <label class="input">
                                            <i class="icon-append fa fa-list"></i>
                                            <textarea name="content" id="write_content" style="width:100%;height:150px;"></textarea>
                                            <b class="tooltip tooltip-bottom-right">본문 입력하세요</b>
                                        </label>
                                    </section>

                                </fieldset>

                                <fieldset>
                                    <dlv class="col col-xs-12 col-sm-12 col-md-12" id="uploader">
                                        <form class="dropzone" id="mydropzone" method="POST"></form>
                                    </dlv>
                                </fieldset>
                            </div>

                            <footer>
                                <button class="btn btn-primary" id="btn_write">콘텐츠 등록!</button>
                            </footer>
                        </div>
                        <!-- end widget content -->
                    </div>
                    <!-- end widget div -->
                </div>
                <!-- end widget -->
            </article>
            <!-- WIDGET END -->

        </div>
    </div>
</div>

<c:import url="../includes/script.jsp"/>
<c:import url="write_script.jsp"/>
<script>

</script>

<c:import url="../includes/footer.jsp"/>