package com.cmsoft.konan.constants;

/**
 * 		사이트에서 사용될 Role name을 Key map으로 정의하여 사용한다.
 * 			- 변경 또는 추가될 경우 여기만 변경 하면 사이트 전체 적용
 */
public class RolesForEL {

	private String USER					= Roles.USER;
	private String ADMIN				= Roles.ADMIN;
	
	public String getUSER() {
		return USER;
	}
	public void setUSER(String uSER) {
		USER = uSER;
	}
	public String getADMIN() {
		return ADMIN;
	}
	public void setADMIN(String aDMIN) {
		ADMIN = aDMIN;
	}	
}
