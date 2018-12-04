<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@include file="../includes/taglib.jsp" %>
<table class="table table-striped">
    <thead>
    <tr>
        <th>구간 </th>
        <th>구간 범위 </th>
    </tr>
    </thead>
    <tbody>

    <c:forEach var="result" items="${sectionList}" varStatus="i">
        <tr style="display:${result.delflag?'none':'table-row'};" delflag="${result.delflag}" name ="secTr" id="secTr_${result.shotid}" onclick="getSectionShotList('${result.videoid}','${result.shotid}','<c:out value="${result.assetfilepath}"/>', '<c:out value="${result.assetfilename}"/>');">
            <th name="secTdNum" scope="row" class="section-num">
                <c:out value="${i.count}"/>구간
                <button class="btn ${result.delflag?'btn-success':'btn-red'}" style="display:none;padding:2px 7px;" onclick="updateShot(${result.videoid},${result.shotid},${!result.delflag})">${result.delflag?'복구':'삭제'}</button>
            </th>
            <td name="secTdTime">
                ${result.starttimecode}~${result.endtimecode}
            </td>
            <input type="hidden" name="startsec" value="${result.startframeindex/param.rate}"/>
            <input type="hidden" name="endsec" value="${result.endframeindex/param.rate}"/>
            <input type="hidden" name="shotid" value="${result.shotid}"/>
        </tr>
    </c:forEach>


    </tbody>
</table>