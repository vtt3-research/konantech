package com.konan.service;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import com.konan.vo.CommonSearchVO;
import com.konan.vo.ServerInfoVO;

public interface ServerInfoService {

	public List<ServerInfoVO> selectServerInfoList(CommonSearchVO vo);
	public ServerInfoVO selectServerInfo(ServerInfoVO vo);
	public ServerInfoVO updateServerUnActive(HttpServletRequest request, ServerInfoVO vo);
	public ServerInfoVO updateServerActive(HttpServletRequest request, ServerInfoVO vo);
	public ServerInfoVO updateServerProxy(HttpServletRequest request, ServerInfoVO vo);
}
