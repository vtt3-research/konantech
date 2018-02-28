<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>
<c:import url="../includes/header.jsp"/>

<!-- dropzone style -->
<%--<link rel="stylesheet" type="text/css" media="screen" href="<c:url value="/resources/custom/js/dropzone/dropzone.min.css"/>">--%>
<style>



</style>


<aside id="left-panel">
    <!-- User info -->
    <div class="login-info">
        <span>KONANTECHNOLOGY</span>
    </div>
    <!-- end user info -->
    <nav>
        <ul>
            <li class="<c:if test="${fn:startsWith(servlet_path,'/content/write')}"> active </c:if>">
                <a href="<c:url value="/content"/>" title="blank_"><i class="fa fa-lg fa-fw fa-arrow-circle-o-right"></i>
                    <span class="menu-item-parent">콘텐츠 등록</span></a>
            </li>
        </ul>
    </nav>
    <span class="minifyme" data-action="minifyMenu"> <i class="fa fa-arrow-circle-left hit"></i> </span>
</aside>
<!-- #MAIN PANEL -->
<div id="main" role="main">

    <div id="ribbon"></div>
    <!-- col -->
    <div class="col-xs-12 col-sm-7 col-md-7 col-lg-4">
        <h1 class="page-title txt-color-blueDark">

            <!-- PAGE HEADER -->
            <i class="fa-fw fa fa-plus-square-o"></i>
            콘텐츠 등록<span> > 등록</span>
        </h1>
    </div>

    <div id="content">
        <div class="row">

            <!-- NEW WIDGET START -->
            <article class="col-xs-12 col-sm-12 col-md-12 col-lg-12 sortable-grid ui-sortable">
                <!-- Widget ID (each widget will need unique ID)-->
                <div class="jarviswidget jarviswidget-sortable jarviswidget-sortable" id="wid-id-2" data-widget-togglebutton="false" data-widget-deletebutton="false"
                     data-widget-colorbutton="false" data-widget-editbutton="false" data-widget-fullscreenbutton="true" data-widget-collapsed="false">
                    <header role="heading">
                        <span class="widget-icon"> <i class="fa fa-table"></i> </span>
                        <h2 style="font-family: Custom1 Gothic;">콘텐츠 등록</h2>
                    </header>
                    <div>
                        <div class="widget-body no-padding">

                            <div id="write_form" class="smart-form">
                                <fieldset>
                                    <section>
                                        <label class="label">제목</label>
                                        <label class="input">
                                            <i class="icon-append fa fa-list"></i>
                                            <input type="text" name="title" id="write_title" placeholder="제목을 입력하세요">
                                            <b class="tooltip tooltip-bottom-right">제목을 입력하세요</b>
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
<!-- END #MAIN PANEL -->


<c:import url="../includes/script.jsp"/>
<c:import url="write_script.jsp"/>
<script>

</script>

<c:import url="../includes/footer.jsp"/>