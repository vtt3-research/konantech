package com.konan.service.impl;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.konan.dao.MemberInfoDAO;
import com.konan.service.MemberInfoService;
import com.konan.vo.CommonSearchVO;
import com.konan.vo.MemberInfoVO;

@Service
public class MemberInfoServiceImpl implements MemberInfoService {
	@Autowired private MemberInfoDAO memberInfoDAO;
	
	@Override
	public Integer selectMemberInfoListCount(CommonSearchVO vo) {
		return memberInfoDAO.selectMemberInfoListCount(vo);
	}                              
	
	@Override
	public List<MemberInfoVO> selectMemberInfoList(CommonSearchVO vo) {
		return memberInfoDAO.selectMemberInfoList(vo);
	}	
	
	@Override	            
	public MemberInfoVO selectMemberInfo(MemberInfoVO vo){
		return memberInfoDAO.selectMemberInfo(vo);
	}
	
	@Override
	@Transactional
	public MemberInfoVO deleteMemberInfo(MemberInfoVO vo){
		memberInfoDAO.deleteMemberInfo(vo);
		return null;
	}
	
	@Override
	@Transactional
	public MemberInfoVO insertMemberInfo(HttpServletRequest request, MemberInfoVO vo){
		memberInfoDAO.insertMemberInfo(vo);
		return null;
	}
	
	@Override
	@Transactional
	public MemberInfoVO updateMemberInfo(HttpServletRequest request, MemberInfoVO vo){
		memberInfoDAO.updateMemberInfo(vo);
		return null;
	}
}
