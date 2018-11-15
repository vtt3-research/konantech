package com.cmsoft.konan.vo;

import java.io.Serializable;

public class CommonPagingVO implements Serializable {

	private static final long serialVersionUID = -1872125732287472991L;
	
	private Boolean isPaging = true;
	private Integer pageNo = 0;					//현재 페이지 번호(0 부터 시작)
	private Integer start;							//현재 페이지의 시작 Row Index
	private Integer pageSize = 20;				//한 페이지에 보여줄 Row 갯수
	private Integer totalCount = 0;				//전체 Row 갯수
	private Integer pageCount;					//전체 페이지 숫자의 갯수
	private Integer windowCount = 10;			//한 영역에 보여줄 페이징 숫자의 갯수
	
	public Boolean getIsPaging() {
		return isPaging;
	}
	public void setIsPaging(Boolean isPaging) {
		this.isPaging = isPaging;
	}
	public Integer getPageNo() {
		return pageNo;
	}
	public void setPageNo(Integer pageNo) {
		this.pageNo = pageNo;
	}
	public Integer getStart() {
		this.start = this.pageNo * this.pageSize;
		return start;
	}
	public void setStart(Integer start) {
		this.start = start;
	}
	public Integer getPageSize() {
		return pageSize;
	}
	public void setPageSize(Integer pageSize) {
		this.pageSize = pageSize;
	}
	public Integer getTotalCount() {
		return totalCount;
	}
	public void setTotalCount(Integer totalCount) {
		this.totalCount = totalCount;
	}
	public Integer getPageCount() {
		this.pageCount = this.totalCount/this.pageSize==0 ? 1 : this.totalCount/this.pageSize+(this.totalCount%this.pageSize==0 ? 0 : 1);
		return pageCount;
	}
	public void setPageCount(Integer pageCount) {
		this.pageCount = pageCount;
	}
	public Integer getWindowCount() {
		return windowCount;
	}
	public void setWindowCount(Integer windowCount) {
		this.windowCount = windowCount;
	}
	
	
	
	
	public boolean isFirstPage() {
        return pageNo == 0;
    }
	public boolean isFirstWindow() {
        return pageNo < windowCount;
    }
	public int getPrevPage() {
        return pageNo > 0 ? pageNo -1 : pageNo;
    }
	public int getPrevWindow() {
		int _prevWindow = pageNo-windowCount;
		if(_prevWindow < windowCount) return windowCount-1;
		else{
			while(true){
        		if((_prevWindow+1)%windowCount == 0){
        			break;
        		}else{
        			_prevWindow = _prevWindow+1;
        		}
        	}
			return _prevWindow;
		}
    }
	public int[] getPages() {
        int npages = getPageCount();
        int off = pageNo/windowCount*windowCount;
        int len = Math.min(Math.min(windowCount, npages), npages - off);
        int[] pages = new int[len];
        for (int i = 0; i < pages.length; i++) {
            pages[i] = off + i + 1;
        }
        return pages;
    }
	public boolean isLastPage() {
        return getPageCount() <= pageNo+1;
    }
	public boolean isLastWindow() {
		if(isLastPage()) return true;
		int _pageCount = getPageCount();
		if((getNextWindow()+1) > _pageCount) return true;
		else return false;
    }
	public int getNextPage() {
        return pageNo < getPageCount() ? pageNo + 1 : pageNo;
    }
	public int getNextWindow() {
		int _pageCount = getPageCount();
		int _nextWindow = pageNo+windowCount;
		if(_nextWindow+1 > _pageCount) return _pageCount;
		else{
			while(true){
        		if(_nextWindow%windowCount == 0){
        			break;
        		}else{
        			_nextWindow = _nextWindow-1;
        		}
        	}
			return _nextWindow;
		}
    }
}
