package com.cmsoft.konan.dao;

import java.util.List;

import org.springframework.stereotype.Repository;

import com.cmsoft.konan.vo.CrawlerInfoVO;
import com.cmsoft.konan.vo.CollectionInfoVO;

@Repository
public interface CrawlerInfoDAO {
	public List<CrawlerInfoVO> selectCrawlerInfoList(CollectionInfoVO vo);

}
