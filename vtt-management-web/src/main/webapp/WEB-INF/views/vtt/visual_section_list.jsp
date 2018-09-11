<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@include file="../includes/taglib.jsp" %>
<table class="table table-striped">
    <thead>
    <tr>
        <th>구간 </th>
        <c:if test="${viewChk =='section'}">
            <th scope="col">
                구간번호
            </th>
        </c:if>
        <th>구간 범위 </th>
    </tr>
    </thead>
    <tbody>

    <c:forEach var="result" items="${sectionList}" varStatus="i">
        <tr name ="secTr" id="secTr_${result.shotid}" onclick="getSectionShotList('${result.videoid}','${result.shotid}','<c:out value="${result.assetfilepath}"/>', '<c:out value="${result.assetfilename}"/>');">
            <th name="secTdNum" scope="row" class="section-num">
                <c:choose>
                    <c:when test="${viewChk =='section'}">
                        <c:out value="${i.count}"/>구간
                    </c:when>
                    <c:otherwise>
                        <c:out value="${i.count}"/>구간
                    </c:otherwise>
                </c:choose>
            </th>
            <input type="hidden" value="${i.count}" name="period_num">
            <c:if test="${viewChk =='section'}">
                <td>
                   ${result.shotid}
                </td>
            </c:if>
            <td name="secTdTime" value="${result.shotid}">
                ${result.starttimecode}~${result.endtimecode}
            </td>
        </tr>
    </c:forEach>


    </tbody>
</table>