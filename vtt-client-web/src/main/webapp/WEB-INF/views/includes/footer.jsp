<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>

<!-- Logout Modal-->
<div class="modal fade" id="exampleModal" tabindex="-1" role="dialog" aria-labelledby="exampleModalLabel" aria-hidden="true">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="exampleModalLabel">로그아웃 하겠습니까?</h5>
                <button class="close" type="button" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">×</span>
                </button>
            </div>
            <div class="modal-footer">
                <button class="btn btn-secondary" type="button" data-dismiss="modal">취소</button>
                <a class="btn btn-primary" href="<c:url value="/user/logout"/>">로그아웃</a>
            </div>
        </div>
    </div>
</div>

<!-- user add Modal-->
<div class="modal fade" id="addUserModal" tabindex="-1" role="dialog" aria-labelledby="addUserModalLabel" aria-hidden="true">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="addUserModalLabel">사용자 추가</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body">
                <form id= "userForm" class="container">
                    <div class="form-group" id="formGrpIdDiv">
                        <label for="username" class="form-control-label form-control-sm">사용자id:</label>
                        <input type="text" class="form-control" id="username" name="username" placeholder="사용자id" required>
                        <div class="invalid-feedback" id="usernameChk"></div>
                    </div>
                    <div class="form-group">
                        <label for="name" class="form-control-label form-control-sm">사용자명:</label>
                        <input type="text" class="form-control" id="name" name="name" placeholder="사용자명" required>
                        <div class="invalid-feedback"></div>
                    </div>
                    <div class="form-group">
                        <label for="userAuth" class="form-control-label form-control-sm">사용자권한:</label>
                        <select id = "userAuth" name = "userAuth" class="form-control" required>
                            <option value="1">전체관리자</option>
                            <option value="2">메타데이터작업자</option>
                            <option value="3">일반사용자</option>
                        </select>
                        <div class="invalid-feedback"></div>
                    </div>
                    <div class="form-group">
                        <label for="password1" class="form-control-label form-control-sm">비밀번호:</label>
                        <input type="password" class="form-control" id="password1" name="password1" placeholder="Password" required>
                        <div class="invalid-feedback"></div>
                    </div>
                    <div class="form-group">
                        <label for="password2" class="form-control-label form-control-sm">비밀번호 확인:</label>
                        <input type="password" class="form-control" id="password2" name="password2" placeholder="Password" required>
                        <div class="invalid-feedback" id="pwReChk"></div>
                    </div>
                    <div class="modal-footer">
                        <button type="button" class="btn btn-primary" id="btnSmit">등록</button>
                        <button type="button" class="btn btn-secondary" data-dismiss="modal">취소</button>
                    </div>
                </form>
            </div>
        </div>
    </div>
</div>

<!-- Contents add Modal-->
<div class="modal fade" id="addContentsModal" tabindex="-1" role="dialog" aria-labelledby="addContentsModalLabel" aria-hidden="true">
    <div class="modal-dialog" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <h5 class="modal-title" id="addContentsModalLabel">콘텐츠 등록</h5>
                <button type="button" class="close" data-dismiss="modal" aria-label="Close">
                    <span aria-hidden="true">&times;</span>
                </button>
            </div>
            <div class="modal-body">
                <div class="row">
                    <!-- NEW WIDGET START -->
                    <article class="col-xs-12 col-sm-12 col-md-12 col-lg-12 sortable-grid ui-sortable">
                        <div>
                            <div>
                                <div class="widget-body no-padding">

                                    <div id="write_form" class="smart-form">
                                        <fieldset>
                                            <section>
                                                <label>제목</label>
                                                <label class="input">
                                                    <i class="icon-append fa fa-list"></i>
                                                    <input type="text" name="title" id="write_title" placeholder="제목을 입력하세요">
                                                    <b class="tooltip tooltip-bottom-right" title="This is my image's tooltip message!" >제목을 입력하세요</b>
                                                </label>
                                            </section>
                                            <section>
                                                <label>내용</label>
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
                                    <div class="modal-footer">
                                        <button class="btn btn-primary" id="btn_write">콘텐츠 등록!</button>
                                    </div>
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
    </div>
</div>
<!-- Contents add Modal-->
<div class="modal fade" id="videoHotkeyModal" tabindex="-1" role="dialog" aria-labelledby="addContentsModalLabel" aria-hidden="true">
    <div class="modal-dialog" role="document">
        <div class="modal-content"></div>
    </div>
</div>
<!-- footer content -->
<footer>
    <div class="pull-right">
        © 2018 <a href="http://www.konantech.com/">KONAN</a>. All Rights reserved
    </div>
    <div class="clearfix"></div>
</footer>
<!-- /footer content -->
</div>
</div>

</body>
</html>
