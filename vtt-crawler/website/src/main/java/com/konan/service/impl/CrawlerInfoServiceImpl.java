package com.konan.service.impl;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.konan.dao.CrawlerInfoDAO;
import com.konan.service.CrawlerInfoService;
import com.konan.vo.CollectionInfoVO;
import com.konan.vo.CrawlerInfoVO;

@Service
public class CrawlerInfoServiceImpl implements CrawlerInfoService {

	@Autowired private CrawlerInfoDAO crawlerInfoDAO;
	
	@Override
	public List<CrawlerInfoVO> selectCrawlerInfoList(CollectionInfoVO vo) {
		return crawlerInfoDAO.selectCrawlerInfoList(vo);
	}
}
