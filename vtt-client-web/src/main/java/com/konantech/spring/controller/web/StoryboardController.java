package com.konantech.spring.controller.web;

import com.konantech.spring.domain.content.AnalyzerSogang;
import com.konantech.spring.domain.content.ContentField;
import com.konantech.spring.domain.content.ContentQuery;
import com.konantech.spring.domain.response.ItemResponse;
import com.konantech.spring.domain.response.ListResponse;
import com.konantech.spring.domain.storyboard.ShotTB;
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

        ContentField item = contentService.getContentItem(query);
        int total = storyboardService.getShotCount(query);
        List<ShotTB> list = storyboardService.getShotList(query);
        Map<String,Object> shotSize = null;
        if(list != null && list.size() > 0) {
            ShotTB firstAsset = list.get(0);
            shotSize = storyboardService.getShotSize(firstAsset);
            if(shotSize != null) {
                item.setShotWidth(MapUtils.getIntValue(shotSize,"shotWidth"));
                item.setShotHeight(MapUtils.getIntValue(shotSize,"shotHeight"));
            }
            for (ShotTB asset : list) {
                String object = asset.getObject();
                AnalyzerSogang analyzerSogang = new AnalyzerSogang(object);
                asset.setDetect(analyzerSogang);
            }
        }

        ItemResponse<ContentField> itemResponse = new ItemResponse<>();
        itemResponse.setItem(item);

        ListResponse listResponse = new ListResponse<ShotTB>();
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

        List<ShotTB> list = storyboardService.getShotList(query);
        ContentField item = contentService.getContentItem(query);

        Map<String,Object> shotSize = null;

        if(list != null && list.size() > 0) {
            ShotTB firstAsset = list.get(0);
            shotSize = storyboardService.getShotSize(firstAsset);
            if(shotSize != null) {
                item.setShotWidth(MapUtils.getIntValue(shotSize,"shotWidth"));
                item.setShotHeight(MapUtils.getIntValue(shotSize,"shotHeight"));
            }
            for (ShotTB asset : list) {
                String object = asset.getObject();
                AnalyzerSogang analyzerSogang = new AnalyzerSogang(object);
                asset.setDetect(analyzerSogang);
            }
        }

        ListResponse listResponse = new ListResponse<ShotTB>();
        listResponse.setList(list);

        ItemResponse<ContentField> itemResponse = new ItemResponse<>();
        itemResponse.setItem(item);

        modelMap.addAttribute("itemResponse",itemResponse);
        modelMap.addAttribute("listResponse",listResponse);
        modelMap.addAttribute("videoServerUrl", videoServerUrl);
        modelMap.addAttribute("shotServerUrl", shotServerUrl);
        return "storyboard/storyboard_play";
    }

}