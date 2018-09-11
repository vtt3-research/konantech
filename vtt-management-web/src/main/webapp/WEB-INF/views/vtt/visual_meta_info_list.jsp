<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@include file="../includes/taglib.jsp" %>
<!-- #MAIN PANEL -->
<ul class="nav nav-tabs bar_tabs" id="myTab" role="tablist">
    <c:forEach var="visual_results" items="${metaJsonMap}" varStatus="i">
        <c:if test="${visual_results.person != null}">
         <li role="presentation" class="<c:if test="${i.index ==0}">active</c:if>">
            <a class="nav-item nav-link" id="nav-tab-person-${i.count}" data-toggle="tab" href="#nav-person-${i.count}" role="tab" aria-controls="nav-person-${i.count}" aria-selected="true">대표프레임${visual_results.period_frame_num} / 인물</a>
         </li>
        </c:if>
        <c:if test="${visual_results.object != null}">
        <li role="presentation">
            <a class="nav-item nav-link" id="nav-tab-obj-${i.count}" data-toggle="tab" href="#nav-obj-${i.count}" role="tab" aria-controls="nav-obj-${i.count}" aria-selected="true">대표프레임${visual_results.period_frame_num} / 객체</a>
        </li>
        </c:if>
    </li>
    </c:forEach>
</ul>
<div class="tab-content" id="nav-tabContent">
    <c:forEach var="visualResults" items="${metaJsonMap}" varStatus="i">
    <c:if test="${visualResults.person != null}">
    <div class="tab-pane fade <c:if test="${i.index ==0}">active in</c:if>" id="nav-person-${i.count}" role="tabpanel" aria-labelledby="nav-person-${i.count}">
        <table class="table table-striped">
            <thead>
            <tr>
                <th scope="col">이름</th>
                <th scope="col">얼굴좌표</th>
                <th scope="col">전체좌표</th>
                <th scope="col">행동</th>
                <th scope="col">감정</th>
                <th scope="col">서술어</th>
                <th scope="col">관련객체</th>
            </tr>
            </thead>
            <tbody>
            <c:forEach var="persons" items="${visualResults.person[0]}" varStatus="k">
                <c:forEach var="person" items="${persons.value}" varStatus="k">
                <tr>
                    <td>${persons.key}</td>
                    <td>${person.face_rect.min_x},${person.face_rect.min_y},${person.face_rect.max_x},${person.face_rect.max_y}</td>
                    <td>${person.full_rect.min_x},${person.full_rect.min_y},${person.full_rect.max_x},${person.full_rect.max_y}</td>
                    <td>${person.behavior}</td>
                    <td>${person.emotion}</td>
                    <td>
                    <c:forEach var="related_object" items="${person.related_object}" varStatus="k">
                        ${related_object.predicate}
                        <c:if test = "${!k.last}">||</c:if>
                    </c:forEach>
                    </td>
                    <td>
                    <c:forEach var="related_object" items="${person.related_object}" varStatus="k">
                        ${related_object.object_name}
                        <c:if test = "${!k.last}">||</c:if>
                    </c:forEach>
                    </td>
                </tr>
                </c:forEach>
            </c:forEach>
            </tbody>
        </table>
    </div>
    </c:if>
    </c:forEach>
    <c:forEach var="visual_results" items="${metaJsonMap}" varStatus="i">
    <div class="tab-pane fade" id="nav-obj-${i.count}" role="tabpanel" aria-labelledby="nav-obj-${i.count}">
        <table class="table table-striped">
            <thead>
            <tr>
                <th scope="col">객체명</th>
                <th scope="col">좌표</th>
                <th scope="col">구분</th>
            </tr>
            </thead>
            <tbody>
            <c:forEach var="persons" items="${visual_results.person[0]}" varStatus="k">
                <c:forEach var="person" items="${persons.value}" varStatus="k">
                    <c:forEach var="object" items="${person.related_object}" varStatus="k">
                        <tr>
                            <td>${object.object_name}</td>
                            <td>${object.object_rect.min_x},${object.object_rect.min_y},${object.object_rect.max_x},${object.object_rect.max_y}</td>
                            <td>
                                (관련객체)
                            </td>
                        </tr>
                    </c:forEach>
                </c:forEach>
            </c:forEach>
            <c:forEach var="object" items="${visual_results.object}" varStatus="j">
                <tr>
                    <td>${object.object_name}</td>
                    <td>${object.object_rect.min_x},${object.object_rect.min_y},${object.object_rect.max_x},${object.object_rect.max_y}</td>
                    <td>
                        (일반객체)
                    </td>
                </tr>
            </c:forEach>
            </tbody>
        </table>
    </div>
    </c:forEach>
</div>