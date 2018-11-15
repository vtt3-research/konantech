package com.konan.vo;

import java.io.Serializable;

public class ServerInfoVO implements Serializable {

	private static final long serialVersionUID = -794546841257883025L;
	
	private Integer SID;
	private String SERVER_NAME;
	private String SERVER_IP;
	private String LOCAL_IP;
	private String SERVER_ACTIVE;
	private String SERVER_CHECK;
	private String USE_PROXY;
	private Integer ROWNUM;
	
	public Integer getROWNUM() {
		return ROWNUM;
	}
	public void setROWNUM(Integer rOWNUM) {
		ROWNUM = rOWNUM;
	}
	public Integer getSID() {
		return SID;
	}
	public void setSID(Integer sID) {
		SID = sID;
	}	
	public String getSERVER_NAME() {
		return SERVER_NAME;
	}
	public void setSERVER_NAME(String sERVER_NAME) {
		SERVER_NAME = sERVER_NAME;
	}
	public String getSERVER_IP() {
		return SERVER_IP;
	}
	public void setSERVER_IP(String sERVER_IP) {
		SERVER_IP = sERVER_IP;
	}
	public String getLOCAL_IP() {
		return LOCAL_IP;
	}
	public void setLOCAL_IP(String lOCAL_IP) {
		LOCAL_IP = lOCAL_IP;
	}	
	public String getSERVER_ACTIVE() {
		return SERVER_ACTIVE;
	}
	public void setSERVER_ACTIVE(String sERVER_ACTIVE) {
		SERVER_ACTIVE = sERVER_ACTIVE;
	}
	public String getSERVER_CHECK() {
		return SERVER_CHECK;
	}
	public void setSERVER_CHECK(String sERVER_CHECK) {
		SERVER_CHECK = sERVER_CHECK;
	}
	public String getUSE_PROXY() {
		return USE_PROXY;
	}
	public void setUSE_PROXY(String uSE_PROXY) {
		USE_PROXY = uSE_PROXY;
	}	
}
