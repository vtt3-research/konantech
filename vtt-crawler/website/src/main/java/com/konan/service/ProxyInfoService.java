package com.konan.service;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import com.konan.vo.CommonSearchVO;
import com.konan.vo.ProxyInfoVO;

public interface ProxyInfoService {

	public List<ProxyInfoVO> selectProxyInfoList(CommonSearchVO vo);
	public ProxyInfoVO selectProxyInfo(ProxyInfoVO vo);
	public ProxyInfoVO deleteProxyInfo(ProxyInfoVO vo);	
	public ProxyInfoVO insertProxyInfo(HttpServletRequest request, ProxyInfoVO vo);	
	public ProxyInfoVO updateProxyInfo(HttpServletRequest request, ProxyInfoVO vo);	
}
