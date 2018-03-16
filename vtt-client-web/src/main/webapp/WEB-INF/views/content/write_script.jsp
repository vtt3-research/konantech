<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>


<!-- dropzonejs -->
<script src="<c:url value="/resources/custom/js/dropzone/dropzone.min.js"/>"></script>


<script>

    Dropzone.autoDiscover = false;
    $(document).ready(function() {
        $("#mydropzone").dropzone({
            url: "<c:url value="/content/upload"/>",
            maxFiles: 2000,
            addRemoveLinks: true,
            // acceptedFiles: ".mp4, .mp4",
            maxFilesize: 100000, //mb
            paramName: "file",
            params: {
                _token: "__token__"
            },
            autoProcessQueue: false,
            dictDefaultMessage: '<span class="text-center"><span class="font-lg visible-xs-block visible-sm-block visible-lg-block"><span class="font-md">여기에 드래그 & 드롭으로 파일을 추가하세요.</span><span>&nbsp&nbsp<h4 class="display-inline"> (또는 클릭)</h4></span>',
            dictResponseError: 'Error uploading file!',
            init: function () {
                var _this = this;
                $("#btn_write").click(function() {
                    _this.processQueue();
                });
                this.on("addedfile", function (file) {
                    console.log(file);
                }),
                this.on("success", function (file, response) {
                    console.log("success");
                }),
                this.on("sending", function(file, xhr, formData) {
                    formData.append("title", $("#write_title").val());
                    formData.append("content", $("#write_content").val());
                }),
                this.on("thumbnail", function(file, dataUrl) {
                }),
                this.on("error", function(file, error, xhr) {
                    //var ficheiro = { nome: file.name, status: xhr.status, statusText: xhr.statusText, erro: error.message };
                    MSG.alert(error.message);
                }),
                this.on("uploadprogress", function(file, progress, bytesSent) {
                }),
                this.on("queuecomplete", function(file, progress, bytesSent) {
                    MSG.alert( "작업이 완료되었습니다<br>'확인'버튼을 누르면 콘텐츠 페이지로 전환됩니다",function() {
                        location.href = "<c:url value='/content' />";
                    });
                }),
                this.on("processing", function() {
                    this.options.autoProcessQueue = true;
                });
            }
        });


    });

    $(document).ready(function() {
        $('.tooltip').tooltipster();
    });

</script>