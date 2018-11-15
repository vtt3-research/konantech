package com.cmsoft.konan.dao;

import java.util.List;

import org.springframework.stereotype.Repository;

import com.cmsoft.konan.vo.CommonSearchVO;
import com.cmsoft.konan.vo.CollectionInfoVO;

@Repository
public interface CollectionInfoDAO {
	
	public Integer selectCollectionInfoListCount(CommonSearchVO vo);	
	
	public List<CollectionInfoVO> selectCollectionInfoList(CommonSearchVO vo);

	public CollectionInfoVO selectCollectionInfo(CollectionInfoVO vo);
	
	public void deleteCollectionInfo(CollectionInfoVO vo);
	
	public void cancelCollectionInfo(CollectionInfoVO vo);
	
	public void insertCollectionInfo(CollectionInfoVO vo);
	
	public void updateCollectionInfo(CollectionInfoVO vo);
	
}
