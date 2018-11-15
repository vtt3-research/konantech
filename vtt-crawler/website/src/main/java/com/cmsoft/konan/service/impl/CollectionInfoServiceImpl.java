package com.cmsoft.konan.service.impl;

import java.util.List;

import javax.servlet.http.HttpServletRequest;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.cmsoft.konan.dao.CollectionInfoDAO;
import com.cmsoft.konan.service.CollectionInfoService;
import com.cmsoft.konan.vo.CommonSearchVO;
import com.cmsoft.konan.vo.CollectionInfoVO;

@Service
public class CollectionInfoServiceImpl implements CollectionInfoService {
	@Autowired private CollectionInfoDAO collectionInfoDAO;
	
	@Override
	public Integer selectCollectionInfoListCount(CommonSearchVO vo) {
		return collectionInfoDAO.selectCollectionInfoListCount(vo);
	}                              
	
	@Override
	public List<CollectionInfoVO> selectCollectionInfoList(CommonSearchVO vo) {
		return collectionInfoDAO.selectCollectionInfoList(vo);
	}	
	
	@Override	            
	public CollectionInfoVO selectCollectionInfo(CollectionInfoVO vo){
		return collectionInfoDAO.selectCollectionInfo(vo);
	}
	
	@Override
	@Transactional
	public CollectionInfoVO deleteCollectionInfo(CollectionInfoVO vo){
		collectionInfoDAO.deleteCollectionInfo(vo);
		return null;
	}
	
	@Override
	@Transactional
	public CollectionInfoVO cancelCollectionInfo(CollectionInfoVO vo){
		collectionInfoDAO.cancelCollectionInfo(vo);
		return null;
	}	
	
	@Override
	@Transactional
	public CollectionInfoVO insertCollectionInfo(HttpServletRequest request, CollectionInfoVO vo){
		collectionInfoDAO.insertCollectionInfo(vo);
		return null;
	}
	
	@Override
	@Transactional
	public CollectionInfoVO updateCollectionInfo(HttpServletRequest request, CollectionInfoVO vo){
		collectionInfoDAO.updateCollectionInfo(vo);
		return null;
	}
}
