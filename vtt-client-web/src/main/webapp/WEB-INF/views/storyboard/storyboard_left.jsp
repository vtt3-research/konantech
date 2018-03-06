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
            <li class="<c:if test="${fn:startsWith(servlet_path,'/storyboard')  && !fn:startsWith(servlet_path,'/storyboard/play') }"> active </c:if>">
                <a href="<c:url value="/storyboard?idx=${param.idx}"/>" title="blank_"><i class="fa fa-lg fa-fw fa-arrow-circle-o-right"></i>
                    <span class="menu-item-parent">스토리보드</span></a>
            </li>
            <li class="<c:if test="${fn:startsWith(servlet_path,'/storyboard/play')}"> active </c:if>">
                <a href="<c:url value="/storyboard/play?idx=${param.idx}"/>" title="blank_"><i class="fa fa-lg fa-fw fa-arrow-circle-o-right"></i>
                    <span class="menu-item-parent">객체인식 재생</span></a>
            </li>

        </ul>
    </nav>
    <span class="minifyme" data-action="minifyMenu"> <i class="fa fa-arrow-circle-left hit"></i> </span>
</aside>