package com.konan.vo;

import java.io.Serializable;

public class CommonResultVO implements Serializable{
	
	private static final long serialVersionUID = -8991843020757605552L;
	
	private Integer resultCode;
	private String resultMessage;
	private Object resultObject;
	
	public CommonResultVO() {}	
	public CommonResultVO(Integer resultCode){
		this.resultCode = resultCode;
	}
	public CommonResultVO(Integer resultCode, String resultMessage){
		this.resultCode = resultCode;
		this.resultMessage = resultMessage;
	}	
	public CommonResultVO(Integer resultCode, String resultMessage, Object resultObject){
		this.resultCode = resultCode;
		this.resultMessage = resultMessage;
		this.resultObject = resultObject;
	}
	
	public int getResultCode() {
		return resultCode;
	}
	public void setResultCode(Integer resultCode) {
		this.resultCode = resultCode;
	}
	public String getResultMessage() {
		return resultMessage;
	}
	public void setResultMessage(String resultMessage) {
		this.resultMessage = resultMessage;
	}
	public Object getResultObject() {
		return resultObject;
	}
	public void setResultObject(Object resultObject) {
		this.resultObject = resultObject;
	}
}
