package com.konan.service.impl;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.konan.dao.DomainInfoDAO;
import com.konan.service.DomainInfoService;
import com.konan.vo.CommonSearchVO;
import com.konan.vo.DomainInfoVO;

@Service
public class DomainInfoServiceImpl implements DomainInfoService {
	@Autowired private DomainInfoDAO domainInfoDAO;
	
	@Override
	public Integer selectDomainInfoListCount(CommonSearchVO vo) {
		return domainInfoDAO.selectDomainInfoListCount(vo);
	}                              
	
	@Override
	public List<DomainInfoVO> selectDomainInfoList(CommonSearchVO vo) {
		return domainInfoDAO.selectDomainInfoList(vo);
	}	
	
	@Override	            
	public DomainInfoVO selectDomainInfo(DomainInfoVO vo){
		return domainInfoDAO.selectDomainInfo(vo);
	}
	
	@Override
	@Transactional
	public DomainInfoVO deleteDomainInfo(DomainInfoVO vo){
		domainInfoDAO.deleteDomainInfo(vo);
		return null;
	}
	
	@Override
	@Transactional
	public DomainInfoVO insertDomainInfo(HttpServletRequest request, DomainInfoVO vo){
		domainInfoDAO.insertDomainInfo(vo);
		return null;
	}
	
	@Override
	@Transactional
	public DomainInfoVO updateDomainInfo(HttpServletRequest request, DomainInfoVO vo){
		domainInfoDAO.updateDomainInfo(vo);
		return null;
	}
}
