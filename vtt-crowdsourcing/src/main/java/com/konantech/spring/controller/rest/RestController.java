package com.konantech.spring.controller.rest;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.konantech.spring.domain.visual.Visual;
import com.konantech.spring.mapper.CrowdMapper;
import com.konantech.spring.util.JSONUtils;
import com.konantech.spring.util.RequestUtils;
import com.konantech.spring.util.ResultJSON;
import io.swagger.annotations.ApiImplicitParam;
import io.swagger.annotations.ApiImplicitParams;
import io.swagger.annotations.ApiOperation;
import net.sf.json.JSON;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;

import javax.servlet.http.HttpServletRequest;
import java.util.*;

@org.springframework.web.bind.annotation.RestController
public class RestController {
    @Autowired
    CrowdMapper crowdMapper;

    @ApiOperation(value = "신뢰도 갱신", notes = "크라우드 소싱 결과 반영")
    @ApiImplicitParams({
            @ApiImplicitParam(name = "targetid", value = "targetid", required = false, dataType = "int", paramType = "path", defaultValue = "1"),
            @ApiImplicitParam(name = "score", value = "score", required = false, dataType = "String", paramType = "query", defaultValue = "1")
    })
    @RequestMapping(value = "/v2/metas/{targetid}", method = RequestMethod.PUT)
    public ResultJSON getContent(@PathVariable int targetid, @RequestParam String score, HttpServletRequest request){
        ResultJSON resultJSON = new ResultJSON();
        Map data = null;
        Map param = RequestUtils.getParameterMap(request);
        param.put("targetid",targetid);
        try {
            data = crowdMapper.getMetaData(param);
            String json = data.get("vtt_meta_json").toString();

            ArrayList<Map> list = (ArrayList<Map>) JSONUtils.jsonStringToObject(json, new ArrayList<>().getClass());
            ArrayList<Visual.VisualResult> resultList = new ArrayList<>();
            //같은 이미지, 같은 객체명, 같은 영역일때 target 데이터와 json 데이터가 일치한다고 판단
            for (Map map : list) {
                String mJson = JSONUtils.jsonStringFromObject(map);
                Visual.VisualResult visualResult = JSONUtils.jsonStringToObject(mJson, Visual.VisualResult.class);
                resultList.add(visualResult);
                if (visualResult.getImage().equals(data.get("image"))) {
                    for (Visual.VisualResult.VisualObject visualObject : visualResult.getObject()) {
                        if (visualObject.getObject_name().equals(data.get("code_name"))) {
                            String rect = (String) data.get("rect");
                            String mRect = visualObject.getObject_rect().getMin_x();
                            mRect += "," + visualObject.getObject_rect().getMin_y();
                            mRect += "," + visualObject.getObject_rect().getMax_x();
                            mRect += "," + visualObject.getObject_rect().getMax_y();
                            if (rect.equals(mRect)) {
                                visualObject.setScore(score);
                                break;
                            }
                        }
                    }
                }
            }
            ObjectMapper mapper = new ObjectMapper();
            String jsonResult = mapper.writerWithDefaultPrettyPrinter().writeValueAsString(resultList);
            data.put("vtt_meta_json", jsonResult);
            crowdMapper.putMeataData(data);
        }catch(Exception e){
            resultJSON.error("The tagging data could not be found.");
        }

        return resultJSON;
    }

}