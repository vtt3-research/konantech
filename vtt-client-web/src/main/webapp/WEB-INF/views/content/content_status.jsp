<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>

<c:choose>
    <c:when test="${ param.status == 1000 }">
        <div class="progress " rel="tooltip" data-original-title="대기" data-placement="bottom" style="margin-bottom: 0;">
            <div class="progress-bar bg-color-warning " role="progressbar" style="width: 100%">대기</div>
        </div>
    </c:when>

    <c:when test="${ param.status >= 2000 and param.status <= 2100 }">
        <c:set var="percentage" value="${ param.status - 2000 }" />
        <div class="progress progress-striped .active" rel="tooltip" data-original-title="${ percentage }%" data-placement="bottom" style="margin-bottom: 0;">
            <div class="progress-bar " role="progressbar" style="width: ${ percentage }%"></div>
            <div class="progress-bar-title">${ percentage } %</div>
        </div>
    </c:when>

    <c:when test="${ param.status == 3000 }">
        <div class="progress " rel="tooltip" data-original-title="성공" data-placement="bottom" style="margin-bottom: 0;">
            <div class="progress-bar bg-success" role="progressbar" style="width: 100%">성공</div>
        </div>
    </c:when>

    <c:when test="${ param.status == 4000 }">
        <div class="progress transcodingstatus" rel="tooltip" data-original-title="실패" data-placement="bottom" style="margin-bottom: 0;">
            <div class="progress-bar bg-color-redLight" role="progressbar" style="width: 100%">실패</div>
        </div>
    </c:when>

    <c:when test="${ param.status == 5000 }">
        <div class="progress " rel="tooltip" data-original-title="재시도" data-placement="bottom" style="margin-bottom: 0;">
            <div class="progress-bar bg-color-blueLight" role="progressbar" style="width: 100%">재시도</div>
        </div>
    </c:when>

    <c:otherwise>
        <div class="progress " rel="tooltip" data-original-title="연기" data-placement="bottom" style="margin-bottom: 0;">

        </div>
    </c:otherwise>
</c:choose>