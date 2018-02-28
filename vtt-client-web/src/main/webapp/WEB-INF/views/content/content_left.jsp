<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>
<%@include file="../includes/taglib.jsp" %>

<aside id="left-panel">
    <!-- User info -->
    <div class="login-info">
        <span>KONANTECHNOLOGY</span>
    </div>
    <!-- end user info -->
    <nav>
        <ul>
            <li class="<c:if test="${fn:startsWith(servlet_path,'/content')}"> active </c:if>">
                <a href="<c:url value="/content"/>" title="blank_"><i class="fa fa-lg fa-fw fa-arrow-circle-o-right"></i>
                    <span class="menu-item-parent">콘텐츠 리스트</span></a>
            </li>
        </ul>
    </nav>
    <span class="minifyme" data-action="minifyMenu"> <i class="fa fa-arrow-circle-left hit"></i> </span>
</aside>