'use strict';

var OP_DOMAIN_NAME = 'gpsms.co.kr';
var IS_ON_OPERATING = function(){try{return location.hostname.indexOf(OP_DOMAIN_NAME) > -1}catch(e){return true;}}();
var IS_DEBUG_MODE = !IS_ON_OPERATING; //IS_DEBUG_MODE = false;
if(IS_DEBUG_MODE === false){
	console.log = function(){ return; };
	console.debug = function(){ return; };
}

var RESULT_CODE = {
		SUCCESS : 100,						//에러 없이 리턴 성공
		SUCCESS_EMPTY : 200,			//에러 없이 모든 로직을 수행 하였으나, 결과값 없음
		
		ERROR : -100,							//로직 에러로 인한 Exception Catch
		ERROR_EMPTY : -200,				//로직을 수행하는데 필요한 결과값이 없어 더 이상 로직을 수행 할 수 없음
		ERROR_DUPLICATION : -300,		//중복으로 인한 오류
		ERROR_LOGIN : -400,				//로그인을 하지 않아 더이상 로직을 수행 할 수 없음
		ERROR_AUTH : -500					//권한 오류
	};

/**
 * jQuery Function
 */
(function($){
	
	/* Ajax 기본 셋팅 */
	$.ajaxSetup({
		async : true,
		dataType : 'json',
		contentType: 'application/x-www-form-urlencoded; charset=UTF-8', //'application/json; charset=UTF-8',
	    error : function(xhr, textStatus, errorThrown) {
			console.debug('Error ::: ' + this.url + ', errorThrown : ' + errorThrown + ', statusCode : ' + xhr.status);
			console.debug(xhr);
			if(xhr.status == '400') {
				//check required parameter or contentType
				alert('작업을 수행하는데 필요한 필수 값을 찾을 수 없습니다.');
			}else if(xhr.status == '401') {
				//login session check
				alert('사용자 세션이 만료 되었습니다. 로그인 후 다시 시도해주세요.');
			}else if(xhr.status == '403') {
				//forbidden exception
				alert('해당 작업에 접근 또는 수행 할 수 있는 권한이 없습니다.');
			}else if(xhr.status == '404') {
				//url check
				alert('요청하신 페이지를 찾을 수 없습니다.');
			}else{
				//exception
				alert('해당 작업을 수행하던 중 오류가 발생하였습니다. 관리자에게 문의 하세요.');
			}
		}
	});
	
	/* Form data parser  */
	$.fn.serializeObject = function() {
		var obj = null;
		try {
			if (this[0].tagName && this[0].tagName.toUpperCase() == "FORM") {
				var arr = this.serializeArray();
				if (arr) {
					obj = {};
					jQuery.each(arr, function() {
						if(!fnIsEmpty($.trim(this.value))){
							if(obj[this.name] == null){
								obj[this.name] = $.trim(this.value);							
							}else{
								obj[this.name] = obj[this.name]+','+$.trim(this.value);
							}
						}
					});
				}
			}
		} catch (e) {
			console.debug(e.message);
		} finally {
		}
		return obj;
	};
	
	$.getScripts = function(scripts, callback){
		var script_arr = [];
		try{
			if(fnTypeof(scripts) == 'string' && scripts.indexOf(',') < 0){
				script_arr.push(scripts);
			}else if(fnTypeof(scripts) == 'string' && scripts.indexOf(',') > 0){
				for(var i=0; i<scripts.split(',').length; i++){
					script_arr.push($.trim(scripts.split(',')[i]));
				}
			}else if(fnTypeof(scripts) == 'array'){
				for(var i=0; i<scripts.length; i++){
					script_arr.push($.trim(scripts[i]));
				}
			}
		}catch(e){
			console.error(e);
		}
		
		if(script_arr.length > 0){
			var ajaxes = [];
			for(var i=0; i<script_arr.length; i++){
				var ajax = $.ajax({
					cache : true,
					async : false,
					dataType : 'script',
					url : script_arr[i]+(script_arr[i].indexOf('?')>-1?'&':'?')+'v='+resource_version
				});
			}
			
			$.when.apply($, ajaxes).always(callback);
		}
	};
	
})($);

/**
 * 		객체가 null 이거나 empty 인지 확인한다.
 */
function fnIsEmpty(obj){
	if(obj == null || obj == undefined || obj == "undefined") return true;
	if(fnTypeof(obj) == "string"){
		if(obj.trim() == "" || obj.toLowerCase() == "undefined" || obj.toLowerCase() == "null"){
			return true;
		}
	}else {
		if(fnTypeof(obj) == "object"){
			var emptyValue = true;
			for(var property in obj) if(!fnIsEmpty(obj[property])){emptyValue = false; break;}
			if(emptyValue) return true;
		}else if(fnTypeof(obj) == "array"){
			if(obj.length == 0) return true;
			else{
				var emptyValue = true;
				for(var i=0; i<obj.length; i++) if(!fnIsEmpty(obj[i])){emptyValue = false; break;}
				if(emptyValue) return true;
			}
		}
	}
	return false;
};

/**
 * 		객체의 타입을 찾아온다.
 */
function fnTypeof(object) {
	if (object === null) return "null";
	else if (object === undefined) return "undefined";
	else if (typeof object === "number") return "number";
	else if (typeof object === "function") return "function";
	else if (typeof object === "string") return "string";
	else if (typeof object === "boolean") return "boolean";
	else if (object.constructor === "".constructor) return "string";
	else if (object.constructor === [].constructor) return "array";
	else if (object.constructor === {}.constructor) return "object";
	else if (object.constructor === new Date().constructor) return "date";
	else return "unknow";
}

/**
 * 		쿠키 생성
 * 		cookieName 으로 days(몇일) 동안 cookieValue(값) 을 담아 보관한다.
 */
function fnSetCookie(cookieName, days, cookieValue){
	var expireDate = new Date();
	expireDate.setDate(expireDate.getDate()+Number(days));
	document.cookie = cookieName+"="+escape(cookieValue?cookieValue:cookieName)+"; path=/; expires="+expireDate.toGMTString()+";";	
}

/**
 * 		쿠키 삭제
 * 		cookieName 으로 생성된 쿠키를 삭제 한다.
 * 		cookieName 이 안넘어 왔을경우(=is Null) 모든 쿠키를 삭제한다.
 */
function fnDelCookie(cookieName){
	var expireDate = new Date();
	expireDate.setDate(expireDate.getDate()-1);
	if(cookieName){
		if(fnGetCookie(cookieName)){
			document.cookie = cookieName+"=; path=/; expires="+expireDate.toGMTString()+";";
		}
	}else{
		if(document.cookie != ""){
			var cookies = document.cookie.split(";");
			for (var i=0; i<cookies.length; i++){
				var name = cookies[i].split("=")[0];
				document.cookie = name+"=; path=/; expires="+expireDate.toGMTString()+";";
			}
		}
	}
}

/**
 * 		쿠키 value 리턴
 * 		cookieName 으로 생성된 쿠키들중 cookieValue 값을 가지고 있는 쿠키의 값을 반환 한다.
 * 		cookieValue 가 안넘어 왔을경우(=is Null) cookieName으로 된 첫번째 쿠키의 값을 반환 한다.
 */
function fnGetCookie(cookieName, cookieValue){
	var nameOfCookie = cookieName + "=";
  	var x = 0;   	
  	while (x <= document.cookie.length) { 
  		var y = (x+nameOfCookie.length); 
  		if(document.cookie.substring(x, y) == nameOfCookie){ 
  			var endOfCookie = 0;
			if((endOfCookie=document.cookie.indexOf( ";", y )) == -1) endOfCookie = document.cookie.length;
			var v = unescape(document.cookie.substring(y, endOfCookie));
			if(cookieValue){ if(v == cookieValue) return v; }
			else { return v; }
		} 
		x = document.cookie.indexOf(" ", x) + 1; 
		if ( x == 0 ) break;
	}
   	return null;
}

/**
 * 		분 정보를 시간:분 으로 변경하여 반환한다. 
 */
function fnMinsToHHmm(minutes) {
	var h = Math.floor(minutes / 60);
	var m = minutes % 60;
	h = (h<10) ? '0'+ h : h;
	m = (m<10) ? '0'+ m : m;
	return h+':'+m;
}

/**
 * 		Array 중복 항목 제거 후 리턴
 */
Array.prototype.unique = function(){
	if(this == null) return null;
	if(!(this.constructor === [].constructor)) return this;
	
	var arr = [];
	for(var i=0; i<this.length; i++){
		if(arr.indexOf(this[i]) === -1) arr.push(this[i]);
	}
	
	return arr;
};

$(document).ready(function () {


	$(".input-search input:disabled").each(function(){
		$(this).parent().addClass('disabled');
	});
	$(this).removeAttr('disabled');
	$(this).parent(".input-search").removeClass('disabled');



	//gnb
	/*$('.gnb .depth01').hover(function () {
			var eq = $('.depth01').index(this);

			$('#header').addClass('active');
			$('.header-wrap').addClass('active');
			$('.gnb .depth01').removeClass('active');
			$('.gnb .depth01:eq(' + eq + ')').addClass('active');

		}, function () {});


		$('.header-wrap').hover(function(){},function(){
			$('.gnb .depth01').removeClass('active');
			var cnt= 0;
			$('.gnb .depth02').find('li').each(function(eq){
				if($(this).hasClass('active')){
					cnt++;
					$(this).parent('ul').parent('.depth01').addClass('active');
				}
			});

			if(cnt==0) $('.header-wrap, #header').removeClass('active');
		});*/


/*	// 탭
	$('.tab li a').each(function(){

		var this_href=$(this).attr('href');
		$(this_href).css('display','none').siblings('.tab-cont').first().css('display','block');
		$(this).click(function(e){

			e.preventDefault();
			//탭활성화
			$(this).parent('li').addClass('active').siblings('li').removeAttr('class');
			//타켓 디스플레이
			$(this_href).css('display','block').siblings('.tab-cont').css('display','none');

		});

	});*/

	//탭 large
	$('.tab.large').each(function () {
		var divide = $(this).find('li').size();
		$(this).find('li').each(function () {
			$(this).css('width', parseFloat(100 / divide) + '%');
		});
	});


	// button
	$(document).on('mousedown', 'button, input[type="button"], input[type="submit"]', function(e) {
		e.preventDefault();
	});


	jQuery.fn.center = function () {
		this.css("position","absolute");
		this.css("top", Math.max(0, (($(window).height() - $(this).outerHeight()) / 2) +
													$(window).scrollTop()) + "px");
		this.css("left", Math.max(0, (($(window).width() - $(this).outerWidth()) / 2) +
													$(window).scrollLeft()) + "px");
		return this;
	}
	$('.reset-password,.login, pop').center();
	$(window).resize(function(){
		$('.reset-password,.login, pop').center();
	});

	//팝업 닫기
	$('.pop-close, .btn-cancel').on('click', function() {
		$(this).parents('.pop').removeClass('active');
		return false;
	});

	// 테이블 hover, click 활성화
/*	
	$('.data-table.highlight tr').on('mouseover', function(){
		console.log($(this).children('td'));
		$(this).children('td').addClass('active');
		$(this).children('td').siblings().addClass('active');
	}).on('mouseout', function(){
		$(this).children('td').removeClass('active');
		$(this).children('td').siblings().removeClass('active');
	});*/
	
/*	$('.data-table.highlight tr').on('mouseover', function(){
	      $(this).children('td').css('background-color','#f0f9fd');
	      $(this).children('td').siblings().css('background-color','#f0f9fd');
	   }).on('mouseout', function(){
	      $(this).children('td').css('background-color','#fff');
	      $(this).children('td').siblings().css('background-color','#fff');
	   });*/
	
	
	// 테이블 hover, click 활성화c
/*	$('.data-table.highlight tr').on('mouseover', function(){
		$(this).children('td');
		$(this).children('td').css('background-color','#f0f9fd');
		$(this).children('td').siblings().css('background-color','#f0f9fd');
	}).on('mouseout', function(){
		$(this).children('td').css('background-color','#fff');
		$(this).children('td').siblings().css('background-color','#fff');
	});*/

	$('.checkbox input').bind("change", function(data) {
		if (this.checked) {
		  $(this).parents('.data-table td').css('background-color','#f0f9fd');
		  $(this).parents('.data-table td').siblings('td').css('background-color','#f0f9fd');
		} else {
		  $(this).parents('.data-table td').siblings('td').css('background-color','#fff');
		}
	});

});

// calendar
$( function() {
	$( ".datepicker" ).datepicker({
	  showOn: "button",
	  buttonImage: "../../resources/images/common/calendar.png",
	  buttonImageOnly: true,
	  buttonText: "calendar",
	  dateFormat: "yymmdd"
	});


	$('.radio input').bind("change", function(data) {
		if (this.checked) {
		  $(this).siblings('label').css('color', '#119adc');
		  $(this).parents('.radio').siblings('.radio').children('.label').css('color', '#666666');
		} else {
		  $(this).siblings('label').css('color', '#666666');
		}
	});

	$('.checkbox input').bind("change", function(data) {
		if (this.checked) {
		  $(this).siblings('label').css('color', '#119adc');
		} else {
		  $(this).siblings('label').css('color', '#666666');
		}
	});
} );