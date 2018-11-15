package com.cmsoft.konan.dao;

import java.util.List;

import org.springframework.stereotype.Repository;

import com.cmsoft.konan.vo.CommonSearchVO;
import com.cmsoft.konan.vo.ServerInfoVO;

@Repository
public interface ServerInfoDAO {
	
	public List<ServerInfoVO> selectServerInfoList(CommonSearchVO vo);

	public ServerInfoVO selectServerInfo(ServerInfoVO vo);
	
	public ServerInfoVO updateServerUnActive(ServerInfoVO vo);
	
	public ServerInfoVO updateServerActive(ServerInfoVO vo);
	
	public ServerInfoVO updateServerProxy(ServerInfoVO vo);
}
