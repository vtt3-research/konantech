<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>

<c:if test="${pageInfo.totalCount > 0}">
 <script type="text/javascript">
	 function fnPaging(page){

	if (!window.location.origin) {
		window.location.origin = window.location.protocol;
	}
	var url = window.location.origin + window.location.pathname;
	if (window.location.search) {
		var query = window.location.search.substr(1).split('&').reduce(function(obj, v){
			var sp = v.split('=');
			obj[sp[0]] = sp[1];
			return obj;
		}, {});
		query['pageNo'] = page;
		var search = Object.keys(query).map(function(key, idx, array){
			return [key, query[key]].join('=');
		}).join('&');
		url += '?'+search;
	} else {
		url += "?pageNo="+page;	
	}
	location.href = url;	
	}
</script> 

<div class="pagination">
<c:if test="${!pageInfo.firstWindow}">
	<a href="javascript:fnPaging('${pageInfo.prevWindow}')" class="first">처음</a>
</c:if>
<c:if test="${!pageInfo.firstPage}">
	<a href="javascript:fnPaging('${pageInfo.prevPage}')" class="prev">이전</a>
</c:if>
<span class="page">
<c:forEach var="pagenum" items="${pageInfo.pages}">
	<c:choose>
		<c:when test="${pagenum-1 eq pageInfo.pageNo}">
		<strong title="현위치">${pagenum}
		</strong>
		</c:when>
		<c:otherwise>
			<a href="javascript:fnPaging('${pagenum-1}')">${pagenum}</a>
		</c:otherwise>
	</c:choose>
</c:forEach>		
</span>
<c:if test="${!pageInfo.lastPage}">	
	<a href="javascript:fnPaging('${pageInfo.nextPage}')" class="next">다음</a>
</c:if>
<c:if test="${!pageInfo.lastWindow}">
	<a href="javascript:fnPaging('${pageInfo.nextWindow}')" class="last">끝</a>
</c:if>
</div>
</c:if> 