package com.cmsoft.konan.service;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import com.cmsoft.konan.vo.CommonSearchVO;
import com.cmsoft.konan.vo.DomainInfoVO;

public interface DomainInfoService {

	public Integer selectDomainInfoListCount(CommonSearchVO vo);
	
	public List<DomainInfoVO> selectDomainInfoList(CommonSearchVO vo);
	
	public DomainInfoVO selectDomainInfo(DomainInfoVO vo);
	
	public DomainInfoVO deleteDomainInfo(DomainInfoVO vo);
	
	public DomainInfoVO insertDomainInfo(HttpServletRequest request, DomainInfoVO vo);
	
	public DomainInfoVO updateDomainInfo(HttpServletRequest request, DomainInfoVO vo);

}
