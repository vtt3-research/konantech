package com.konan.controller;


import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
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
import com.konan.service.DomainInfoService;
import com.konan.service.MemberInfoService;
import com.konan.service.ProxyInfoService;
import com.konan.service.ServerInfoService;
import com.konan.utility.CommonUtil;
import com.konan.vo.CollectionInfoVO;
import com.konan.vo.CommonResultVO;
import com.konan.vo.CommonSearchVO;
import com.konan.vo.CrawlerInfoVO;
import com.konan.vo.DomainInfoVO;
import com.konan.vo.MemberInfoVO;
import com.konan.vo.ProxyInfoVO;
import com.konan.vo.ServerInfoVO;

@Controller
@RequestMapping("/admin")
public class AdminController {

	@Autowired private CollectionInfoService collectionInfoService;
	
	@Autowired private DomainInfoService domainInfoService;
	
	@Autowired private CrawlerInfoService crawlerInfoService;
	
	@Autowired private ServerInfoService serverInfoService;
	
	@Autowired private MemberInfoService memberInfoService;
	
	@Autowired private ProxyInfoService proxyInfoService;
	
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
		
		return "/admin/collectionList";
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
		return "/admin/collectionWrite";
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
				
		return "/admin/collectionView";
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
		
		return "/admin/serverState";
	}	

	@ResponseBody
	@RequestMapping(value="/serverState", method=RequestMethod.POST)
	public CommonResultVO  server(
			CommonSearchVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {

		List<ServerInfoVO> serverInfoList = serverInfoService.selectServerInfoList(vo);		

		Integer conTrue = 0;
		Integer conFalse = 0;
		for (int i = 0; i < serverInfoList.size(); i++) {
			Integer port = 22;
			try{
				Socket sock = new Socket (serverInfoList.get(i).getLOCAL_IP(), port);
							
             	conTrue = conTrue+1;
             	serverInfoList.get(i).setSERVER_CHECK("O");

			}catch(UnknownHostException e){

            	conFalse=conFalse+1;
            	serverInfoList.get(i).setSERVER_CHECK("X");
			}catch(SocketException e){

            	conFalse=conFalse+1;
            	serverInfoList.get(i).setSERVER_CHECK("X");
			}catch(IOException e){

            	conFalse=conFalse+1;
            	serverInfoList.get(i).setSERVER_CHECK("X");
			}

		}	

		if(!CommonUtil.isEmpty(serverInfoList)){
			HashMap<String, Object> resultObject = CommonUtil.makeParams(
				"list", CommonUtil.parseToJson(serverInfoList)
			);
			return new CommonResultVO(ResultConstants.SUCCESS, null, resultObject);
		}else{
			HashMap<String, Object> resultObject = CommonUtil.makeParams("list", new ArrayList<ServerInfoVO>());
			return new CommonResultVO(ResultConstants.SUCCESS_EMPTY, null, resultObject);
		}
		
	}
	
	@RequestMapping(value="/serverActive", method=RequestMethod.GET)	
	public String serverActive(
			ServerInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {	    
		
		if(!CommonUtil.isEmpty(vo.getSID())){			
			serverInfoService.updateServerUnActive(request, vo);
			vo = serverInfoService.updateServerActive(request, vo);
		}
		
		return "/admin/serverActive";
	}	
	
	@RequestMapping(value="/serverProxy", method=RequestMethod.GET)	
	public String serverProxy(
			ServerInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {	    
		
		if(!CommonUtil.isEmpty(vo.getSID())){			
			vo = serverInfoService.updateServerProxy(request, vo);			
		}
		
		return "/admin/serverProxy";
	}	
	
	@RequestMapping(value = "/domainList", method = RequestMethod.GET)
	public String domainList(
			CommonSearchVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {
		
		vo.setPageSize(20);			
		Integer totalCount = domainInfoService.selectDomainInfoListCount(vo);
		vo.setTotalCount(totalCount);
		pgno = vo.getPageNo();
		
		model.addAttribute("pageInfo", CommonUtil.getPagingInfo(vo));
		
		return "/admin/domainList";
	}	
	
	@ResponseBody
	@RequestMapping(value="/domainList", method=RequestMethod.POST)
	public CommonResultVO  domain(
			CommonSearchVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {
		
		vo.setPageSize(20);	
		Integer totalCount = domainInfoService.selectDomainInfoListCount(vo);
		vo.setTotalCount(totalCount);
        vo.setPageNo(pgno);

		List<DomainInfoVO> domainInfoList = domainInfoService.selectDomainInfoList(vo);
		
		if(!CommonUtil.isEmpty(domainInfoList)){
			HashMap<String, Object> resultObject = CommonUtil.makeParams(
				"list", CommonUtil.parseToJson(domainInfoList)
			);
			return new CommonResultVO(ResultConstants.SUCCESS, null, resultObject);
		}else{
			HashMap<String, Object> resultObject = CommonUtil.makeParams("list", new ArrayList<CollectionInfoVO>());
			return new CommonResultVO(ResultConstants.SUCCESS_EMPTY, null, resultObject);
		}
		
	}	
	
	@RequestMapping(value = "/domainWrite", method = RequestMethod.GET)
	public String domainWrite(
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {
		return "/admin/domainWrite";
	}	
	
	@RequestMapping(value = "/domainView", method = RequestMethod.GET)
	public String domainView(
			DomainInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {

		if(!CommonUtil.isEmpty(vo.getDOMAIN())){
			vo = domainInfoService.selectDomainInfo(vo);	
			if(vo == null) vo = new DomainInfoVO();
			model.addAttribute("domainInfo", vo);			
		}				
		return "/admin/domainView";
	}	
	
	@ResponseBody
	@RequestMapping(value="/domainSave", method=RequestMethod.POST)	
	public CommonResultVO domainSave(
			DomainInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {	    
		
		if(!CommonUtil.isEmpty(vo.getDOMAIN())){
			vo = domainInfoService.insertDomainInfo(request, vo);
		}
		
		return new CommonResultVO(ResultConstants.SUCCESS, null, vo);
	}	
	
	@ResponseBody
	@RequestMapping(value="/domainEdit", method=RequestMethod.POST)	
	public CommonResultVO domainEdit(
			DomainInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {	    
		
			vo = domainInfoService.updateDomainInfo(request, vo);
		
			return new CommonResultVO(ResultConstants.SUCCESS, null, vo);
	}	
	
	@ResponseBody
	@RequestMapping(value="/domainDelete", method=RequestMethod.POST)
	public CommonResultVO domainDelete(
			DomainInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {	    
		
			domainInfoService.deleteDomainInfo(vo);
			
		    return new CommonResultVO(ResultConstants.SUCCESS, null, vo);		

	}		
	
	@RequestMapping(value = "/memberList", method = RequestMethod.GET)
	public String memberList(
			CommonSearchVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {
		
		vo.setPageSize(20);			
		Integer totalCount = memberInfoService.selectMemberInfoListCount(vo);
		vo.setTotalCount(totalCount);
		pgno = vo.getPageNo();

		model.addAttribute("pageInfo", CommonUtil.getPagingInfo(vo));
		
		return "/admin/memberList";
	}	
	
	@ResponseBody
	@RequestMapping(value="/memberList", method=RequestMethod.POST)
	public CommonResultVO  member(
			CommonSearchVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {
		
		vo.setPageSize(20);	
		Integer totalCount = memberInfoService.selectMemberInfoListCount(vo);
		vo.setTotalCount(totalCount);
        vo.setPageNo(pgno);

		List<MemberInfoVO> memberInfoList = memberInfoService.selectMemberInfoList(vo);
		
		if(!CommonUtil.isEmpty(memberInfoList)){
			HashMap<String, Object> resultObject = CommonUtil.makeParams(
				"list", CommonUtil.parseToJson(memberInfoList)
			);
			return new CommonResultVO(ResultConstants.SUCCESS, null, resultObject);
		}else{
			HashMap<String, Object> resultObject = CommonUtil.makeParams("list", new ArrayList<CollectionInfoVO>());
			return new CommonResultVO(ResultConstants.SUCCESS_EMPTY, null, resultObject);
		}
		
	}	
	
	@RequestMapping(value = "/memberWrite", method = RequestMethod.GET)
	public String memberWrite(
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {
		return "/admin/memberWrite";
	}	
	
	@RequestMapping(value = "/memberView", method = RequestMethod.GET)
	public String memberView(
			MemberInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {

		if(!CommonUtil.isEmpty(vo.getID())){
			vo = memberInfoService.selectMemberInfo(vo);	
			if(vo == null) vo = new MemberInfoVO();
			model.addAttribute("memberInfo", vo);			
		}				
		return "/admin/memberView";
	}	
	
	@ResponseBody
	@RequestMapping(value="/memberSave", method=RequestMethod.POST)	
	public CommonResultVO memberSave(
			MemberInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {	    
		
		if(!CommonUtil.isEmpty(vo.getID())){
			vo = memberInfoService.insertMemberInfo(request, vo);
		}
		
		return new CommonResultVO(ResultConstants.SUCCESS, null, vo);
	}	
	
	@ResponseBody
	@RequestMapping(value="/memberEdit", method=RequestMethod.POST)	
	public CommonResultVO memberEdit(
			MemberInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {	    
		
			vo = memberInfoService.updateMemberInfo(request, vo);
		
			return new CommonResultVO(ResultConstants.SUCCESS, null, vo);
	}	
	
	@ResponseBody
	@RequestMapping(value="/memberDelete", method=RequestMethod.POST)
	public CommonResultVO memberDelete(
			MemberInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {	    
		
			memberInfoService.deleteMemberInfo(vo);
			
		    return new CommonResultVO(ResultConstants.SUCCESS, null, vo);		

	}		
	
	@RequestMapping(value = "/proxyList", method = RequestMethod.GET)
	public String proxyList(
			CommonSearchVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {
						
		return "/admin/proxyList";
	}		
	
	@ResponseBody
	@RequestMapping(value="/proxyList", method=RequestMethod.POST)
	public CommonResultVO  proxy(
			CommonSearchVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {
		
		List<ProxyInfoVO> proxyInfoList = proxyInfoService.selectProxyInfoList(vo);
		
		if(!CommonUtil.isEmpty(proxyInfoList)){
			HashMap<String, Object> resultObject = CommonUtil.makeParams(
				"list", CommonUtil.parseToJson(proxyInfoList)
			);
			return new CommonResultVO(ResultConstants.SUCCESS, null, resultObject);
		}else{
			HashMap<String, Object> resultObject = CommonUtil.makeParams("list", new ArrayList<CollectionInfoVO>());
			return new CommonResultVO(ResultConstants.SUCCESS_EMPTY, null, resultObject);
		}
		
	}		
	
	@RequestMapping(value = "/proxyWrite", method = RequestMethod.GET)
	public String proxyWrite(
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {
		return "/admin/proxyWrite";
	}	
	
	@RequestMapping(value = "/proxyView", method = RequestMethod.GET)
	public String proxyView(
			ProxyInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {

		if(!CommonUtil.isEmpty(vo.getPID())){
			vo = proxyInfoService.selectProxyInfo(vo);	
			if(vo == null) vo = new ProxyInfoVO();
			model.addAttribute("proxyInfo", vo);			
		}				
		return "/admin/proxyView";
	}	
	
	@ResponseBody
	@RequestMapping(value="/proxySave", method=RequestMethod.POST)	
	public CommonResultVO proxySave(
			ProxyInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {	    
		
		if(!CommonUtil.isEmpty(vo.getPROXY())){
			vo = proxyInfoService.insertProxyInfo(request, vo);
		}
		
		return new CommonResultVO(ResultConstants.SUCCESS, null, vo);
	}	
	
	@ResponseBody
	@RequestMapping(value="/proxyEdit", method=RequestMethod.POST)	
	public CommonResultVO proxyEdit(
			ProxyInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {	    
		
			vo = proxyInfoService.updateProxyInfo(request, vo);
		
			return new CommonResultVO(ResultConstants.SUCCESS, null, vo);
	}	
	
	@ResponseBody
	@RequestMapping(value="/proxyDelete", method=RequestMethod.POST)
	public CommonResultVO proxyDelete(
			ProxyInfoVO vo,
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {	    
		
			proxyInfoService.deleteProxyInfo(vo);
			
		    return new CommonResultVO(ResultConstants.SUCCESS, null, vo);		

	}		
}
