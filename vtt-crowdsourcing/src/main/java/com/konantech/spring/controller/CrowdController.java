package com.konantech.spring.controller;

import com.konantech.spring.service.CrowdService;
import com.konantech.spring.util.RequestUtils;
import com.konantech.spring.util.ResultJSON;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

import javax.servlet.http.HttpServletRequest;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Controller
public class CrowdController {

    @Autowired
    CrowdService crowdService;

    @RequestMapping(value = "/crowd/{type}", method = RequestMethod.GET)
    public String crowd(ModelMap modelMap, HttpServletRequest request, @PathVariable String type) throws Exception{
        Map param = RequestUtils.getParameterMap(request);
        if(param.get("site")==null||param.get("userid")==null){
            return "main/main";
        }
        List<Map> imageList = null;
        List<Map> objectList = null;
        String jsp = "";
        if(type.equals("object")) {
            imageList = crowdService.getObjectImageList(request);
            jsp = "crowd_object";
        }else if(type.equals("background")) {
            imageList = crowdService.getBackgroundImageList(request);
            jsp = "crowd_background";
        }else if(type.equals("drag")) {
            param = crowdService.getDragImageList(request);
            imageList = (List<Map>)param.get("targetList");
            objectList = (List<Map>)param.get("objectList");
            jsp = "crowd_drag";
        }


        modelMap.addAttribute("imageList", imageList);
        modelMap.addAttribute("objectList", objectList);
        modelMap.addAttribute("param", param);
        return "crowd/"+jsp;
    }

    @RequestMapping(value = "/setCrowdData/{type}", method = RequestMethod.POST)
    @ResponseBody
    public ResultJSON setCrowdData(HttpServletRequest request, @PathVariable String type) throws Exception{
        ResultJSON resultJSON = null;
        if(!type.equals("drag")){
            resultJSON = crowdService.putCrowdTarget(request);
        }else{
            resultJSON = crowdService.putCrowdTargetDrag(request);
        }
        return resultJSON;
    }

}


