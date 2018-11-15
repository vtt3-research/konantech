package com.cmsoft.konan.service;

import java.util.List;

import com.cmsoft.konan.vo.CrawlerInfoVO;
import com.cmsoft.konan.vo.CollectionInfoVO;

public interface CrawlerInfoService {

	public List<CrawlerInfoVO> selectCrawlerInfoList(CollectionInfoVO vo);

}
