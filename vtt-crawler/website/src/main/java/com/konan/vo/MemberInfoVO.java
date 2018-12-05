package com.konan.vo;

import java.io.Serializable;

public class MemberInfoVO implements Serializable {

	private static final long serialVersionUID = -794546841257883025L;
	
	private String ID;
	private String PASSWORD;
	private String ISADMIN;
	private String INPUT_DATE;
	private String INPUT_ID;	
	private Integer ROWNUM;
	
	public Integer getROWNUM() {
		return ROWNUM;
	}
	public void setROWNUM(Integer rOWNUM) {
		ROWNUM = rOWNUM;
	}
	public String getID() {
		return ID;
	}
	public void setID(String iD) {
		ID = iD;
	}
	public String getPASSWORD() {
		return PASSWORD;
	}
	public void setPASSWORD(String pASSWORD) {
		PASSWORD = pASSWORD;
	}
	public String getISADMIN() {
		return ISADMIN;
	}
	public void setISADMIN(String iSADMIN) {
		ISADMIN = iSADMIN;
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
