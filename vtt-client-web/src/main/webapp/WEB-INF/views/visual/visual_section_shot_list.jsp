<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@include file="../includes/taglib.jsp" %>
        <div class="row shot-list-wrap">
        <c:forEach var="result" items="${sectionShotList}" varStatus="i">
            <div class="col-xs-2"  name="selShotImgLi">
                <c:set var="img_path" value="${ shotServerUrl }/${ result.sectionPath }/${ result.sectionShotFile }"/>
                <div >
                    <div class="image view view-first">
                        <img style="width: 100%; display: block;" name="selShotImg" src="<c:url value="${img_path}"/>" class="img-fluid rounded" alt="${ result.shotId }"/>
                        <input type="hidden" name="shotid" value="${ result.shotId }">
                        <input type="hidden" name="shotServerUrl" value="${ shotServerUrl }">
                        <input type="hidden" name="sectionPath" value="${ result.sectionPath }">
                        <input type="hidden" name="sectionId" value="${ result.sectionId }">
                        <input type="hidden" name="sectionShotId" value="${ result.sectionShotId }">
                        <input type="hidden" name="shotFileName" value="${ result.shotFileName }">
                    </div>
                </div>
            </div>
        </c:forEach>
        </div>