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
        <tr style="display:${result.delflag?'none':'table-row'};" delflag="${result.delflag}" name ="secTr" id="secTr_${result.shotid}" onclick="getSectionShotList('${result.videoid}','${result.shotid}','<c:out value="${result.assetfilepath}"/>', '<c:out value="${result.assetfilename}"/>');">
            <th name="secTdNum" scope="row" class="section-num">
                <c:choose>
                    <c:when test="${viewChk =='section'}">
                        <c:out value="${i.count}"/>구간
                    </c:when>
                    <c:otherwise>
                        <c:out value="${i.count}"/>구간
                    </c:otherwise>
                </c:choose>
                <button class="btn ${result.delflag?'btn-success':'btn-red'}" style="display:none;padding:2px 7px;" onclick="updateShot(${result.videoid},${result.shotid},${!result.delflag})">${result.delflag?'복구':'삭제'}</button>
            </th>
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