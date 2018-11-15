package com.cmsoft.konan.dao;

import java.util.List;

import org.springframework.stereotype.Repository;

import com.cmsoft.konan.vo.CommonSearchVO;
import com.cmsoft.konan.vo.ProxyInfoVO;

@Repository
public interface ProxyInfoDAO {
	
	public List<ProxyInfoVO> selectProxyInfoList(CommonSearchVO vo);

	public ProxyInfoVO selectProxyInfo(ProxyInfoVO vo);
	
	public void deleteProxyInfo(ProxyInfoVO vo);
	
	public void insertProxyInfo(ProxyInfoVO vo);
	
	public void updateProxyInfo(ProxyInfoVO vo);
	
}
