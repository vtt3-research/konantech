<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@include file="../includes/taglib.jsp" %>

<input type="hidden" name="sectionid" value="${param.sectionid}"/>
<c:forEach var="result" items="${questionList}" varStatus="status">
    <input type="hidden" name="questionid" value="${result.questionid}"/>
<div class="form-group edit-form">
    <label class="control-label col-md-1 col-sm-1 col-xs-12"> 질문${status.count}
    </label>
    <div class="col-md-11 col-sm-11 col-xs-12">
        <div class="input-group">
            <input type="text" name="question" class="form-control" placeholder="질문${status.count}" value="${result.question}">
        </div>
    </div>
</div>
<div class="form-group edit-form">
    <label class="control-label col-md-1 col-sm-1 col-xs-12"> 정답${status.count}
    </label>
    <div class="col-md-11 col-sm-11 col-xs-12">
        <div class="input-group">
            <input type="text" name="answer" class="form-control" placeholder="정답${status.count}" value="${result.answer}">
        </div>
    </div>
</div>
<div class="form-group edit-form">
    <label class="control-label col-md-1 col-sm-1 col-xs-12"> 오답${status.count}-1
    </label>
    <div class="col-md-11 col-sm-11 col-xs-12">
        <div class="input-group">
            <input type="text" name="wrong_answer1" class="form-control" placeholder="오답${status.count}-1" value="${result.wrong_answer1}">
        </div>
    </div>
</div>
<div class="form-group edit-form">
    <label class="control-label col-md-1 col-sm-1 col-xs-12"> 오답${status.count}-2
    </label>
    <div class="col-md-11 col-sm-11 col-xs-12">
        <div class="input-group">
            <input type="text" name="wrong_answer2" class="form-control" placeholder="오답${status.count}-2" value="${result.wrong_answer2}">
        </div>
    </div>
</div>
<div class="form-group edit-form">
    <label class="control-label col-md-1 col-sm-1 col-xs-12"> 오답${status.count}-3
    </label>
    <div class="col-md-11 col-sm-11 col-xs-12">
        <div class="input-group">
            <input type="text" name="wrong_answer3" class="form-control" placeholder="오답${status.count}-3" value="${result.wrong_answer3}">
        </div>
    </div>
</div>
<div class="form-group edit-form">
    <label class="control-label col-md-1 col-sm-1 col-xs-12"> 오답${status.count}-4
    </label>
    <div class="col-md-11 col-sm-11 col-xs-12">
        <div class="input-group">
            <input type="text" name="wrong_answer4" class="form-control" placeholder="오답${status.count}-4" value="${result.wrong_answer4}">
        </div>
    </div>
</div>
<div class="h-line-gray"></div>
</c:forEach>

<c:forEach begin="1" end="${3-questionList.size()}" step="1" varStatus="status">
<input type="hidden" name="questionid" value=""/>
<div class="form-group edit-form">
    <label class="control-label col-md-1 col-sm-1 col-xs-12"> 질문${questionList.size()+status.count}
    </label>
    <div class="col-md-11 col-sm-11 col-xs-12">
        <div class="input-group">
            <input type="text" name="question" class="form-control" placeholder="질문${questionList.size()+status.count}" value="">
        </div>
    </div>
</div>
<div class="form-group edit-form">
    <label class="control-label col-md-1 col-sm-1 col-xs-12"> 정답${questionList.size()+status.count}
    </label>
    <div class="col-md-11 col-sm-11 col-xs-12">
        <div class="input-group">
            <input type="text" name="answer" class="form-control" placeholder="정답${questionList.size()+status.count}" value="">
        </div>
    </div>
</div>
<div class="form-group edit-form">
    <label class="control-label col-md-1 col-sm-1 col-xs-12"> 오답${questionList.size()+status.count}-1
    </label>
    <div class="col-md-11 col-sm-11 col-xs-12">
        <div class="input-group">
            <input type="text" name="wrong_answer1" class="form-control" placeholder="오답${questionList.size()+status.count}-1" value="">
        </div>
    </div>
</div>
<div class="form-group edit-form">
    <label class="control-label col-md-1 col-sm-1 col-xs-12"> 오답${questionList.size()+status.count}-2
    </label>
    <div class="col-md-11 col-sm-11 col-xs-12">
        <div class="input-group">
            <input type="text" name="wrong_answer2" class="form-control" placeholder="오답${questionList.size()+status.count}-2" value="">
        </div>
    </div>
</div>
<div class="form-group edit-form">
    <label class="control-label col-md-1 col-sm-1 col-xs-12"> 오답${questionList.size()+status.count}-3
    </label>
    <div class="col-md-11 col-sm-11 col-xs-12">
        <div class="input-group">
            <input type="text" name="wrong_answer3" class="form-control" placeholder="오답${questionList.size()+status.count}-3" value="">
        </div>
    </div>
</div>
<div class="form-group edit-form">
    <label class="control-label col-md-1 col-sm-1 col-xs-12"> 오답${questionList.size()+status.count}-4
    </label>
    <div class="col-md-11 col-sm-11 col-xs-12">
        <div class="input-group">
            <input type="text" name="wrong_answer4" class="form-control" placeholder="오답${questionList.size()+status.count}-4" value="">
        </div>
    </div>
</div>
<div class="h-line-gray"></div>
</c:forEach>