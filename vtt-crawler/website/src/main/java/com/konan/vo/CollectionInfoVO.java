package com.konan.vo;

import java.io.Serializable;

public class CollectionInfoVO implements Serializable {

	private static final long serialVersionUID = -3769452295301082521L;
	
	private Integer	SID;
	private String SC_NAME;
	private String SC_SITE;
	private String SC_URL;
	private Integer SC_TYPE;
	private String SC_EXT;
	private Integer SC_MAXSIZE;
	private String SC_STATE;
	private Integer SC_COUNT;
	private Integer SC_COUNT_E;
	private String SC_COUNT_W;	
	private String USER_ID;
	private String INPUT_DATE;
	private String END_DATE;	
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
	public String getSC_NAME() {
		return SC_NAME;
	}
	public void setSC_NAME(String sC_NAME) {
		SC_NAME = sC_NAME;
	}
	public String getSC_SITE() {
		return SC_SITE;
	}
	public void setSC_SITE(String sC_SITE) {
		SC_SITE = sC_SITE;
	}
	public String getSC_URL() {
		return SC_URL;
	}
	public void setSC_URL(String sC_URL) {
		SC_URL = sC_URL;
	}
	public Integer getSC_TYPE() {
		return SC_TYPE;
	}
	public void setSC_TYPE(Integer sC_TYPE) {
		SC_TYPE = sC_TYPE;
	}
	public String getSC_EXT() {
		return SC_EXT;
	}
	public void setSC_EXT(String sC_EXT) {
		SC_EXT = sC_EXT;
	}
	public Integer getSC_MAXSIZE() {
		return SC_MAXSIZE;
	}
	public void setSC_MAXSIZE(Integer sC_MAXSIZE) {
		SC_MAXSIZE = sC_MAXSIZE;
	}
	public String getSC_STATE() {
		return SC_STATE;
	}
	public void setSC_STATE(String sC_STATE) {
		SC_STATE = sC_STATE;
	}
	public Integer getSC_COUNT() {
		return SC_COUNT;
	}
	public void setSC_COUNT(Integer sC_COUNT) {
		SC_COUNT = sC_COUNT;
	}
	public Integer getSC_COUNT_E() {
		return SC_COUNT_E;
	}
	public void setSC_COUNT_E(Integer sC_COUNT_E) {
		SC_COUNT_E = sC_COUNT_E;
	}
	public String getSC_COUNT_W() {
		return SC_COUNT_W;
	}
	public void setSC_COUNT_W(String sC_COUNT_W) {
		SC_COUNT_W = sC_COUNT_W;
	}	
	public String getUSER_ID() {
		return USER_ID;
	}
	public void setUSER_ID(String uSER_ID) {
		USER_ID = uSER_ID;
	}
	public String getINPUT_DATE() {
		return INPUT_DATE;
	}
	public void setINPUT_DATE(String iNPUT_DATE) {
		INPUT_DATE = iNPUT_DATE;
	}
	public String getEND_DATE() {
		return END_DATE;
	}
	public void setEND_DATE(String eND_DATE) {
		END_DATE = eND_DATE;
	}
	
}
