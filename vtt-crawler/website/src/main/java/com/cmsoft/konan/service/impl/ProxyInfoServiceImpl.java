package com.cmsoft.konan.service.impl;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.cmsoft.konan.dao.ProxyInfoDAO;
import com.cmsoft.konan.service.ProxyInfoService;
import com.cmsoft.konan.vo.CommonSearchVO;
import com.cmsoft.konan.vo.ProxyInfoVO;

@Service
public class ProxyInfoServiceImpl implements ProxyInfoService {
	@Autowired private ProxyInfoDAO proxyInfoDAO;
	
	@Override
	public List<ProxyInfoVO> selectProxyInfoList(CommonSearchVO vo) {
		return proxyInfoDAO.selectProxyInfoList(vo);
	}	
	
	@Override	            
	public ProxyInfoVO selectProxyInfo(ProxyInfoVO vo){
		return proxyInfoDAO.selectProxyInfo(vo);
	}
	
	@Override
	@Transactional
	public ProxyInfoVO deleteProxyInfo(ProxyInfoVO vo){
		proxyInfoDAO.deleteProxyInfo(vo);
		return null;
	}
	
	@Override
	@Transactional
	public ProxyInfoVO insertProxyInfo(HttpServletRequest request, ProxyInfoVO vo){
		proxyInfoDAO.insertProxyInfo(vo);
		return null;
	}
	
	@Override
	@Transactional
	public ProxyInfoVO updateProxyInfo(HttpServletRequest request, ProxyInfoVO vo){
		proxyInfoDAO.updateProxyInfo(vo);
		return null;
	}
}
