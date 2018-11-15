package com.konan.service;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import com.konan.vo.CollectionInfoVO;
import com.konan.vo.CommonSearchVO;

public interface CollectionInfoService {

	public Integer selectCollectionInfoListCount(CommonSearchVO vo);
	
	public List<CollectionInfoVO> selectCollectionInfoList(CommonSearchVO vo);
	
	public CollectionInfoVO selectCollectionInfo(CollectionInfoVO vo);
	
	public CollectionInfoVO deleteCollectionInfo(CollectionInfoVO vo);
	
	public CollectionInfoVO cancelCollectionInfo(CollectionInfoVO vo);
	
	public CollectionInfoVO insertCollectionInfo(HttpServletRequest request, CollectionInfoVO vo);
	
	public CollectionInfoVO updateCollectionInfo(HttpServletRequest request, CollectionInfoVO vo);

}
