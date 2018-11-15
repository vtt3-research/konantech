package com.cmsoft.konan.service;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import com.cmsoft.konan.vo.CommonSearchVO;
import com.cmsoft.konan.vo.MemberInfoVO;

public interface MemberInfoService {

	public Integer selectMemberInfoListCount(CommonSearchVO vo);
	
	public List<MemberInfoVO> selectMemberInfoList(CommonSearchVO vo);
	
	public MemberInfoVO selectMemberInfo(MemberInfoVO vo);
	
	public MemberInfoVO deleteMemberInfo(MemberInfoVO vo);
	
	public MemberInfoVO insertMemberInfo(HttpServletRequest request, MemberInfoVO vo);
	
	public MemberInfoVO updateMemberInfo(HttpServletRequest request, MemberInfoVO vo);

}
