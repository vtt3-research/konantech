package com.konan.utility;

import java.util.Locale;

import org.springframework.context.support.MessageSourceAccessor;

/**
* 
* Comment : context-config.xml 33Line Dservice.mode="실행모드" 값 기준으로 /resource/실행모드/*.properties 파일의 Value 가져온다.
* 
*/
public class MessageUtil {

	private static MessageSourceAccessor messageSourceAccessor;
	public void setMessageSourceAccessor(MessageSourceAccessor messageSourceAccessor) {
		MessageUtil.messageSourceAccessor = messageSourceAccessor;
	}


	/**
	* Method	  : getValue
	* Comment	  : Request Locale 정보로 해당 국가에 해당되는 프로퍼티파일의 value 값을 가져온다.
	* 				ex) Locale = Korean 경우 *_ko.propeties 
	* 				*_ko.propetis 파일이 없을경우  Defalut = *.properties 
	* Modify Memo : 
	**/
	public static String getValue(String key) {
		/**
		HttpServletRequest httpRequest = ((ServletRequestAttributes) RequestContextHolder.getRequestAttributes()).getRequest();
		if(httpRequest.getLocale() == null){
			//request.getLocale 값이 없으면 기본으로 지역설정을 한국으로 설정
			Locale message_locale = Locale.KOREAN;
			//한국에 해당되는 *_ko.properties 파일의 value 가져온다.
			return msAcc.getMessage(key, message_locale);
			
		} else {			
			//request.getLocale 값에 해당되는 프로퍼티파일의 value 가져온다.
			return msAcc.getMessage(key, httpRequest.getLocale());
		}
		**/
		return messageSourceAccessor.getMessage(key, Locale.KOREAN);
	}
	
	
	/**
	* Method	  : getValue
	* Comment	  : 프로퍼티 Key에 해당하는 Value가없을경우 defalutValue값을 반환
	* Modify Memo : 
	**/
	public static String getValue(String key, String defalutValue) {
		return messageSourceAccessor.getMessage(key, defalutValue, Locale.KOREAN);
	}	
	
	public static int getIntValue(String key , int defaultValue) {
		return Integer.parseInt(getValue(key, "0"));
	}
}