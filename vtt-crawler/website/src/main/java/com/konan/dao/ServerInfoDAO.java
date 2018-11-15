package com.konan.dao;

import java.util.List;

import org.springframework.stereotype.Repository;

import com.konan.vo.CommonSearchVO;
import com.konan.vo.ServerInfoVO;

@Repository
public interface ServerInfoDAO {
	
	public List<ServerInfoVO> selectServerInfoList(CommonSearchVO vo);

	public ServerInfoVO selectServerInfo(ServerInfoVO vo);
	
	public ServerInfoVO updateServerUnActive(ServerInfoVO vo);
	
	public ServerInfoVO updateServerActive(ServerInfoVO vo);
	
	public ServerInfoVO updateServerProxy(ServerInfoVO vo);
}
