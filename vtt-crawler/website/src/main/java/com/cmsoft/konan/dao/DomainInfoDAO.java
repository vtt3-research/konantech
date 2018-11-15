package com.cmsoft.konan.dao;

import java.util.List;

import org.springframework.stereotype.Repository;

import com.cmsoft.konan.vo.CommonSearchVO;
import com.cmsoft.konan.vo.DomainInfoVO;

@Repository
public interface DomainInfoDAO {
	
	public Integer selectDomainInfoListCount(CommonSearchVO vo);	
	
	public List<DomainInfoVO> selectDomainInfoList(CommonSearchVO vo);

	public DomainInfoVO selectDomainInfo(DomainInfoVO vo);
	
	public void deleteDomainInfo(DomainInfoVO vo);
	
	public void insertDomainInfo(DomainInfoVO vo);
	
	public void updateDomainInfo(DomainInfoVO vo);
	
}
