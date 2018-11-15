package com.cmsoft.konan.dao;

import java.util.List;

import org.springframework.stereotype.Repository;

import com.cmsoft.konan.vo.CommonSearchVO;
import com.cmsoft.konan.vo.MemberInfoVO;

@Repository
public interface MemberInfoDAO {
	
	public Integer selectMemberInfoListCount(CommonSearchVO vo);	
	
	public List<MemberInfoVO> selectMemberInfoList(CommonSearchVO vo);

	public MemberInfoVO selectMemberInfo(MemberInfoVO vo);
	
	public void deleteMemberInfo(MemberInfoVO vo);
	
	public void insertMemberInfo(MemberInfoVO vo);
	
	public void updateMemberInfo(MemberInfoVO vo);
	
}
