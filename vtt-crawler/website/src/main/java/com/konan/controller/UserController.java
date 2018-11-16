package com.konan.controller;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import com.konan.constants.ResultConstants;
import com.konan.service.CollectionInfoService;
import com.konan.service.CrawlerInfoService;
import com.konan.utility.CommonUtil;
import com.konan.vo.CollectionInfoVO;
import com.konan.vo.CommonResultVO;
import com.konan.vo.CommonSearchVO;
import com.konan.vo.CrawlerInfoVO;

@Controller
@RequestMapping("/user")
public class UserController {

	@Autowired private CollectionInfoService collectionInfoService;
	@Autowired private CrawlerInfoService crawlerInfoService;

	Integer pgno =0;

	@RequestMapping(value = "/collectionList", method = RequestMethod.GET)
	public String collectionList(
			CommonSearchVO vo,
			HttpServletRequest request,
			HttpServletResponse response,
			Model model) throws Exception {
		/*
		vo.setSearchField1("SC_STATE");
		vo.setSearchValue1(-1);

		vo.setSearchField2("SC_NAME");
		vo.setSearchValue2("");

		vo.setSearchField3("USER_ID");
		vo.setSearchValue3("");
		*/
		vo.setPageSize(20);
		Integer totalCount = collectionInfoService.selectCollectionInfoListCount(vo);
		vo.setTotalCount(totalCount);
		pgno = vo.getPageNo();

		model.addAttribute("pageInfo", CommonUtil.getPagingInfo(vo));

		return "/user/collectionList";
	}

	@ResponseBody
	@RequestMapping(value="/collectionList", method=RequestMethod.POST)
	public CommonResultVO  collection(
			CommonSearchVO vo,
			HttpServletRequest request,
			HttpServletResponse response,
			Model model) throws Exception {

		vo.setPageSize(20);
		Integer totalCount = collectionInfoService.selectCollectionInfoListCount(vo);
		vo.setTotalCount(totalCount);
        vo.setPageNo(pgno);

		List<CollectionInfoVO> collectionInfoList = collectionInfoService.selectCollectionInfoList(vo);

		if(!CommonUtil.isEmpty(collectionInfoList)){
			HashMap<String, Object> resultObject = CommonUtil.makeParams(
				"list", CommonUtil.parseToJson(collectionInfoList)
			);
			return new CommonResultVO(ResultConstants.SUCCESS, null, resultObject);
		}else{
			HashMap<String, Object> resultObject = CommonUtil.makeParams("list", new ArrayList<CollectionInfoVO>());
			return new CommonResultVO(ResultConstants.SUCCESS_EMPTY, null, resultObject);
		}

	}

	@RequestMapping(value = "/collectionWrite", method = RequestMethod.GET)
	public String collectionWrite(
			HttpServletRequest request,
			HttpServletResponse response,
			Model model) throws Exception {
		return "/user/collectionWrite";
	}


	@RequestMapping(value = "/collectionView", method = RequestMethod.GET)
	public String collectionView(
			CollectionInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,
			Model model) throws Exception {

		if(!CommonUtil.isEmpty(vo.getSID())){
			vo = collectionInfoService.selectCollectionInfo(vo);
			if(vo == null) vo = new CollectionInfoVO();
			model.addAttribute("collectionInfo", vo);
		}

		return "/user/collectionView";
	}

	@ResponseBody
	@RequestMapping(value="/collectionView", method=RequestMethod.POST)
	public CommonResultVO  collectionViewDetail(
			CollectionInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,
			Model model) throws Exception {

		List<CrawlerInfoVO> crawlerInfoList = crawlerInfoService.selectCrawlerInfoList(vo);

		if(!CommonUtil.isEmpty(crawlerInfoList)){
			HashMap<String, Object> resultObject = CommonUtil.makeParams(
				"list", CommonUtil.parseToJson(crawlerInfoList)
			);
			return new CommonResultVO(ResultConstants.SUCCESS, null, resultObject);
		}else{
			HashMap<String, Object> resultObject = CommonUtil.makeParams("list", new ArrayList<CrawlerInfoVO>());
			return new CommonResultVO(ResultConstants.SUCCESS_EMPTY, null, resultObject);
		}

	}

	@ResponseBody
	@RequestMapping(value="/collectionSave", method=RequestMethod.POST)
	public CommonResultVO collectionSave(
			CollectionInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,
			Model model) throws Exception {

		if(!CommonUtil.isEmpty(vo.getSC_NAME())){
			vo = collectionInfoService.insertCollectionInfo(request, vo);
			/*
			String commands = "D:/Work/Spring/konan/RunPython.BAT";
			Runtime.getRuntime().exec(commands);
			*/
		}

		return new CommonResultVO(ResultConstants.SUCCESS, null, vo);
	}

	@ResponseBody
	@RequestMapping(value="/collectionEdit", method=RequestMethod.POST)
	public CommonResultVO collectionEdit(
			CollectionInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,
			Model model) throws Exception {

			vo = collectionInfoService.updateCollectionInfo(request, vo);

			return new CommonResultVO(ResultConstants.SUCCESS, null, vo);
	}

	@ResponseBody
	@RequestMapping(value="/collectionDelete", method=RequestMethod.POST)
	public CommonResultVO collectionDelete(
			CollectionInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,
			Model model) throws Exception {

		collectionInfoService.deleteCollectionInfo(vo);

		    return new CommonResultVO(ResultConstants.SUCCESS, null, vo);

	}

	@ResponseBody
	@RequestMapping(value="/collectionCancel", method=RequestMethod.POST)
	public CommonResultVO collectionCancel(
			CollectionInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,
			Model model) throws Exception {

		collectionInfoService.cancelCollectionInfo(vo);

		return new CommonResultVO(ResultConstants.SUCCESS, null, vo);

	}

	@RequestMapping(value = "/serverState", method = RequestMethod.GET)
	public String serverState(
			HttpServletRequest request,
			HttpServletResponse response,
			Model model) throws Exception {

		return "/user/serverState";
	}

}
