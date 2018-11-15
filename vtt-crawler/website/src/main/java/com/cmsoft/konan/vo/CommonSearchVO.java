package com.cmsoft.konan.vo;

import java.io.Serializable;
import java.util.HashMap;
import java.util.List;

public class CommonSearchVO extends CommonPagingVO implements Serializable {

	private static final long serialVersionUID = 5110663103923354691L;
	
	private String searchField1;
	private Integer searchValue1; 	
	private String searchField2;
	private String searchValue2;	
	private String searchField3;
	private String searchValue3;	
	private String searchField4;
	private String searchValue4;
	
	private String searchDateField1;
	private String searchStartDateValue1;
	private String searchEndDateValue1;	
	private String searchDateField2;
	private String searchStartDateValue2;
	private String searchEndDateValue2;
	
	private String searchListField1;
	private String[] searchListValues1;
	
	private List<HashMap<String,Object>> selectedItems;
	
	public CommonSearchVO(){}
	
	public CommonSearchVO(Integer pageNo, Integer totalCount) {
		super.setPageNo(pageNo);
		super.setTotalCount(totalCount);
	}
	
	public CommonSearchVO(Integer pageNo, Integer pageSize, Integer totalCount) {
		super.setPageNo(pageNo);
		super.setPageSize(pageSize);
		super.setTotalCount(totalCount);
	}
	public String getSearchField1() {
		return searchField1;
	}
	public void setSearchField1(String searchField1) {
		this.searchField1 = searchField1;
	}
	public Integer getSearchValue1() {
		return searchValue1;
	}
	public void setSearchValue1(Integer searchValue1) {
		this.searchValue1 = searchValue1;
	}
	public String getSearchField2() {
		return searchField2;
	}
	public void setSearchField2(String searchField2) {
		this.searchField2 = searchField2;
	}
	public String getSearchValue2() {
		return searchValue2;
	}
	public void setSearchValue2(String searchValue2) {
		this.searchValue2 = searchValue2;
	}
	public String getSearchField3() {
		return searchField3;
	}
	public void setSearchField3(String searchField3) {
		this.searchField3 = searchField3;
	}
	public String getSearchValue3() {
		return searchValue3;
	}
	public void setSearchValue3(String searchValue3) {
		this.searchValue3 = searchValue3;
	}
	public String getSearchField4() {
		return searchField4;
	}
	public void setSearchField4(String searchField4) {
		this.searchField4 = searchField4;
	}
	public String getSearchValue4() {
		return searchValue4;
	}
	public void setSearchValue4(String searchValue4) {
		this.searchValue4 = searchValue4;
	}
	public String getSearchDateField1() {
		return searchDateField1;
	}
	public void setSearchDateField1(String searchDateField1) {
		this.searchDateField1 = searchDateField1;
	}
	public String getSearchStartDateValue1() {
		return searchStartDateValue1;
	}
	public void setSearchStartDateValue1(String searchStartDateValue1) {
		this.searchStartDateValue1 = searchStartDateValue1;
	}
	public String getSearchEndDateValue1() {
		return searchEndDateValue1;
	}
	public void setSearchEndDateValue1(String searchEndDateValue1) {
		this.searchEndDateValue1 = searchEndDateValue1;
	}
	public String getSearchDateField2() {
		return searchDateField2;
	}
	public void setSearchDateField2(String searchDateField2) {
		this.searchDateField2 = searchDateField2;
	}
	public String getSearchStartDateValue2() {
		return searchStartDateValue2;
	}
	public void setSearchStartDateValue2(String searchStartDateValue2) {
		this.searchStartDateValue2 = searchStartDateValue2;
	}
	public String getSearchEndDateValue2() {
		return searchEndDateValue2;
	}
	public void setSearchEndDateValue2(String searchEndDateValue2) {
		this.searchEndDateValue2 = searchEndDateValue2;
	}
	public String getSearchListField1() {
		return searchListField1;
	}
	public void setSearchListField1(String searchListField1) {
		this.searchListField1 = searchListField1;
	}
	public String[] getSearchListValues1() {
		return searchListValues1;
	}
	public void setSearchListValues1(String[] searchListValues1) {
		this.searchListValues1 = searchListValues1;
	}
	public List<HashMap<String, Object>> getSelectedItems() {
		return selectedItems;
	}
	public void setSelectedItems(List<HashMap<String, Object>> selectedItems) {
		this.selectedItems = selectedItems;
	}	
}
