package com.konantech.spring.controller.web;

import com.konantech.spring.domain.content.AnalyzerSogang;
import com.konantech.spring.domain.content.ContentQuery;
import com.konantech.spring.domain.response.ItemResponse;
import com.konantech.spring.domain.response.ListResponse;
import com.konantech.spring.service.ContentService;
import com.konantech.spring.service.StoryboardService;
import com.konantech.spring.util.RequestUtils;
import org.apache.commons.collections.MapUtils;
import org.postgresql.util.PGobject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import javax.servlet.http.HttpServletRequest;
import java.util.List;
import java.util.Map;

@Controller
public class StoryboardController {

    @Autowired
    private ContentService contentService;

    @Autowired
    private StoryboardService storyboardService;

//    private String videoServerUrl = "http://10.10.18.12:7070/darc4/proxyvideo";
    private String videoServerUrl = "http://10.10.18.12:7070/darc4/video";
    private String shotServerUrl = "http://10.10.18.12:7070/darc4/proxyshot";

    @RequestMapping(value = "/storyboard", method = RequestMethod.GET)
    public String storyboard(ModelMap modelMap, HttpServletRequest request) throws Exception {

        int idx = RequestUtils.getParameterInt(request, "idx", 0);

        ContentQuery query = new ContentQuery();
        query.setIdx(idx);

        Map<String, Object> item = contentService.getContentItem(query);
        int total = storyboardService.getShotCount(query);
        List<Map<String, Object>> list = storyboardService.getShotList(query);
        Map<String,Object> shotSize = null;
        if(list != null && list.size() > 0) {
            Map<String, Object> firstAsset = list.get(0);
            shotSize = storyboardService.getShotSize(firstAsset);
            if(shotSize != null) {
                item.putAll(shotSize);
            }
            for (Map<String, Object> asset : list) {
                String object = MapUtils.getString(asset,"object");
                AnalyzerSogang analyzerSogang = new AnalyzerSogang(object);
                asset.put("detect", analyzerSogang);
            }
        }

        ItemResponse<Map<String, Object>> itemResponse = new ItemResponse<>();
        itemResponse.setItem(item);

        ListResponse<Map<String, Object>> listResponse = new ListResponse<>();
        listResponse.setTotal(total);
        listResponse.setOffset(query.getOffset());
        listResponse.setLimit(query.getLimit());
        listResponse.setList(list);

        modelMap.addAttribute("itemResponse",itemResponse);
        modelMap.addAttribute("listResponse",listResponse);
        modelMap.addAttribute("videoServerUrl", videoServerUrl);
        modelMap.addAttribute("shotServerUrl", shotServerUrl);

        return "storyboard/storyboard";
    }

    @RequestMapping(value = "/storyboard/play", method = RequestMethod.GET)
    public String storyboardPlay(ModelMap modelMap, HttpServletRequest request) throws Exception {

        int idx = RequestUtils.getParameterInt(request, "idx", 0);

        ContentQuery query = new ContentQuery();
        query.setIdx(idx);

        List<Map<String, Object>> list = storyboardService.getShotList(query);
        Map<String, Object> item = contentService.getContentItem(query);

        Map<String,Object> shotSize = null;
        ListResponse<Map<String, Object>> listResponse = new ListResponse<>();

        if(list != null && list.size() > 0) {
            Map<String, Object> firstAsset = list.get(0);
            shotSize = storyboardService.getShotSize(firstAsset);
            if(shotSize != null) {
                item.putAll(shotSize);
            }
            for (Map<String, Object> asset : list) {
                String object = MapUtils.getString(asset,"object");
                AnalyzerSogang analyzerSogang = new AnalyzerSogang(object);
                asset.put("detect", analyzerSogang);
            }
        }
        listResponse.setList(list);

        ItemResponse<Map<String, Object>> itemResponse = new ItemResponse<>();
        itemResponse.setItem(item);

        modelMap.addAttribute("itemResponse",itemResponse);
        modelMap.addAttribute("listResponse",listResponse);
        modelMap.addAttribute("videoServerUrl", videoServerUrl);
        modelMap.addAttribute("shotServerUrl", shotServerUrl);
        return "storyboard/storyboard_play";
    }

}