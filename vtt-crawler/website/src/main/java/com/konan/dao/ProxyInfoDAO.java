package com.konan.dao;

import java.util.List;

import org.springframework.stereotype.Repository;

import com.konan.vo.CommonSearchVO;
import com.konan.vo.ProxyInfoVO;

@Repository
public interface ProxyInfoDAO {
	
	public List<ProxyInfoVO> selectProxyInfoList(CommonSearchVO vo);

	public ProxyInfoVO selectProxyInfo(ProxyInfoVO vo);
	
	public void deleteProxyInfo(ProxyInfoVO vo);
	
	public void insertProxyInfo(ProxyInfoVO vo);
	
	public void updateProxyInfo(ProxyInfoVO vo);
	
}
