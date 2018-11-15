package com.konan.vo;

import java.io.Serializable;

public class DomainInfoVO implements Serializable {

	private static final long serialVersionUID = -794546841257883025L;
	
	private String DOMAIN;
	private String USER_ID;
	private String USER_PASS;
	private String INPUT_DATE;
	private String INPUT_ID;	
	private Integer ROWNUM;
	
	public Integer getROWNUM() {
		return ROWNUM;
	}
	public void setROWNUM(Integer rOWNUM) {
		ROWNUM = rOWNUM;
	}
	public String getDOMAIN() {
		return DOMAIN;
	}
	public void setDOMAIN(String dOMAIN) {
		DOMAIN = dOMAIN;
	}
	public String getUSER_ID() {
		return USER_ID;
	}
	public void setUSER_ID(String uSER_ID) {
		USER_ID = uSER_ID;
	}
	public String getUSER_PASS() {
		return USER_PASS;
	}
	public void setUSER_PASS(String uSER_PASS) {
		USER_PASS = uSER_PASS;
	}	
	public String getINPUT_DATE() {
		return INPUT_DATE;
	}
	public void setINPUT_DATE(String iNPUT_DATE) {
		INPUT_DATE = iNPUT_DATE;
	}
	public String getINPUT_ID() {
		return INPUT_ID;
	}
	public void setINPUT_ID(String iNPUT_ID) {
		INPUT_ID = iNPUT_ID;
	}	
}
