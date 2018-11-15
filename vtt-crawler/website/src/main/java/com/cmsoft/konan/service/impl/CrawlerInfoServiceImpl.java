package com.cmsoft.konan.service.impl;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.cmsoft.konan.dao.CrawlerInfoDAO;
import com.cmsoft.konan.service.CrawlerInfoService;
import com.cmsoft.konan.vo.CrawlerInfoVO;
import com.cmsoft.konan.vo.CollectionInfoVO;

@Service
public class CrawlerInfoServiceImpl implements CrawlerInfoService {

	@Autowired private CrawlerInfoDAO crawlerInfoDAO;
	
	@Override
	public List<CrawlerInfoVO> selectCrawlerInfoList(CollectionInfoVO vo) {
		return crawlerInfoDAO.selectCrawlerInfoList(vo);
	}
}
