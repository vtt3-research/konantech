<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>

<c:forEach var="result" items="${soundList}" varStatus="status">
<tr name ="secTr" id="soundTr_${result.soundid}" onclick="getSoundInfo(this)">
    <input type="hidden" name="soundid" value="${result.soundid}"/>
    <input type="hidden" name="starttimecode" value="${result.starttimecode}"/>
    <input type="hidden" name="endtimecode" value="${result.endtimecode}"/>
    <input type="hidden" name="delflag" value="false"/>
    <td>
        ${status.count}
    </td>
    <td name="startTime" scope="row" class="section-num">
        ${result.starttimecode}
    </td>
    <td name="endTime" scope="row" class="section-num">
        ${result.endtimecode}
    </td>
    <td name="secTdTime">
        <select class="form-control form-select" style="float:left;" name = "soundtype" >
            <c:forEach var="map" items="${soundType}" varStatus="i">
                <option value="${map.code}" ${result.soundtype==map.code?'selected':''} >${map.code_name}</option>
            </c:forEach>
        </select>
    </td>
</tr>
</c:forEach>
<tr name ="secTr" id="soundTr_new" onclick="getSoundInfo(this)"  style="display:none;">>
    <input type="hidden" name="soundid" value=""/>
    <input type="hidden" name="starttimecode" value="0:00:00"/>
    <input type="hidden" name="endtimecode" value="0:00:00"/>
    <input type="hidden" name="relationid" value=""/>
    <input type="hidden" name="delflag" value="false"/>
    <td>
        New
    </td>
    <td name="startTime" scope="row" class="section-num">
        0:00:00
    </td>
    <td name="endTime" scope="row" class="section-num">
        0:00:00
    </td>
    <td name="secTdTime">
        <select class="form-control form-select" style="float:left;" name = "soundtype" >
            <option value="">선택</option>
            <c:forEach var="map" items="${soundType}" varStatus="i">
                <option value="${map.code}">${map.code_name}</option>
            </c:forEach>
        </select>
    </td>
</tr>