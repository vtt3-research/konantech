package com.konan.vo;

import java.io.Serializable;

public class ProxyInfoVO implements Serializable {

	private static final long serialVersionUID = -794546841257883025L;
	
	private Integer PID;
	private String PROXY;
	private String FAIL;
	private Integer ROWNUM;
	
	public Integer getROWNUM() {
		return ROWNUM;
	}
	public void setROWNUM(Integer rOWNUM) {
		ROWNUM = rOWNUM;
	}
	public Integer getPID() {
		return PID;
	}
	public void setPID(Integer pID) {
		PID = pID;
	}	
	public String getPROXY() {
		return PROXY;
	}
	public void setPROXY(String pROXY) {
		PROXY = pROXY;
	}
	public String getFAIL() {
		return FAIL;
	}
	public void setFAIL(String fAIL) {
		FAIL = fAIL;
	}	
}
