package com.cmsoft.konan.constants;

/**
 * 		Ajax 리턴시 사용되는 Logic 수형 결과 코드
 */
public final class ResultConstants {
	public static final int SUCCESS = 100;						//에러 없이 리턴 성공
	public static final int SUCCESS_EMPTY = 200;			//에러 없이 모든 로직을 수행 하였으나, 결과값 없음
	
	public static final int ERROR = -100;						//로직 에러로 인한 Exception Catch
	public static final int ERROR_EMPTY = -200;				//로직을 수행하는데 필요한 결과값이 없어 더 이상 로직을 수행 할 수 없음
	public static final int ERROR_DUPLICATION = -300;	//중복으로 인한 오류
	public static final int ERROR_LOGIN = -400;				//로그인을 하지 않아 더이상 로직을 수행 할 수 없음
}
