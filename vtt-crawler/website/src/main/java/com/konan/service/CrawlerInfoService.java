package com.konan.service;

import java.util.List;

import com.konan.vo.CollectionInfoVO;
import com.konan.vo.CrawlerInfoVO;

public interface CrawlerInfoService {

	public List<CrawlerInfoVO> selectCrawlerInfoList(CollectionInfoVO vo);

}
