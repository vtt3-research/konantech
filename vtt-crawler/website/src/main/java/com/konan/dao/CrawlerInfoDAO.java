package com.konan.dao;

import java.util.List;

import org.springframework.stereotype.Repository;

import com.konan.vo.CollectionInfoVO;
import com.konan.vo.CrawlerInfoVO;

@Repository
public interface CrawlerInfoDAO {
	public List<CrawlerInfoVO> selectCrawlerInfoList(CollectionInfoVO vo);

}
