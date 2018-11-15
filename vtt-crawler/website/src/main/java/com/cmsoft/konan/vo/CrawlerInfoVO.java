package com.cmsoft.konan.vo;

import java.io.Serializable;

public class CrawlerInfoVO implements Serializable {

	private static final long serialVersionUID = 685237991411337078L;
	
	private String TITLE;
	private String CATEGORY;
	private String LICENSE;
	private String CONTENTS;
	private String ISSUCCESS;
	private String FILENAME;	
	private String FILE_URL;
	private String SERVER_IP;
	private String INPUT_DATE;
	private Integer ROWNUM;
	
	public Integer getROWNUM() {
		return ROWNUM;
	}
	public void setROWNUM(Integer rOWNUM) {
		ROWNUM = rOWNUM;
	}
	public String getTITLE() {
		return TITLE;
	}
	public void setTITLE(String tITLE) {
		TITLE = tITLE;
	}
	public String getCATEGORY() {
		return CATEGORY;
	}
	public void setCATEGORY(String cATEGORY) {
		CATEGORY = cATEGORY;
	}
	public String getLICENSE() {
		return LICENSE;
	}
	public void setLICENSE(String lICENSE) {
		LICENSE = lICENSE;
	}	
	public String getCONTENTS() {
		return CONTENTS;
	}
	public void setCONTENTS(String cONTENTS) {
		CONTENTS = cONTENTS;
	}
	public String getISSUCCESS() {
		return ISSUCCESS;
	}
	public void setISSUCCESS(String iSSUCCESS) {
		ISSUCCESS = iSSUCCESS;
	}
	public String getFILENAME() {
		return FILENAME;
	}
	public void setFILENAME(String fILENAME) {
		FILENAME = fILENAME;
	}
	public String getFILE_URL() {
		return FILE_URL;
	}
	public void setFILE_URL(String fILE_URL) {
		FILE_URL = fILE_URL;
	}
	public String getSERVER_IP() {
		return SERVER_IP;
	}
	public void setSERVER_IP(String sERVER_IP) {
		SERVER_IP = sERVER_IP;
	}	
	public String getINPUT_DATE() {
		return INPUT_DATE;
	}	
	public void setINPUT_DATE(String iNPUT_DATE) {
		INPUT_DATE = iNPUT_DATE;
	}	
}
