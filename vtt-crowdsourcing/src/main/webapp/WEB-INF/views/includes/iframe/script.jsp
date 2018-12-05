<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../taglib.jsp" %>

<script src="<c:url value="/webjars/jquery/1.12.4/jquery.min.js"/>"></script>
<script src="<c:url value="/webjars/jquery-ui/1.12.1/jquery-ui.min.js"/>"></script>
<script src="<c:url value="/webjars/bootstrap/3.3.7-1/js/bootstrap.min.js"/>"></script>
<script src="<c:url value="/webjars/bootbox/4.4.0/bootbox.js"/>"></script>
<script src="<c:url value="/resources/custom/js/vtt-crowd.js"/>"></script>
<script>

    function setCrowdData(type,data){
        $.ajax({
            url: "<c:url value='/setCrowdData/' />"+type,
            async: false,
            type: "POST",
            data: data,
            dataType: "json",
            success: function (response) {
                if (response.status == "SUCCESS") {
                    try {
                        MSG.alert(response.data.message, callback);
                    }catch(exception){
                        MSG.alert(response.data.message,reload);
                    }
                } else if (response.status == "ERROR") {
                    MSG.alert(response.data.message,reload);
                } else {
                    MSG.alert("오류");
                    return false;
                }
            },
            error: function (xhr, opt, err) {
                MSG.alert(xhr.responseText);
            }
        });
    }
</script>