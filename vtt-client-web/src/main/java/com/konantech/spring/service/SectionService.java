package com.konantech.spring.service;

import com.konantech.spring.domain.content.ContentField;
import com.konantech.spring.domain.content.ContentQuery;
import com.konantech.spring.domain.section.Section;
import com.konantech.spring.domain.storyboard.ShotTB;
import com.konantech.spring.mapper.ContentMapper;
import com.konantech.spring.mapper.SectionMapper;
import com.konantech.spring.util.FFmpegUtil;
import com.konantech.spring.util.JSONUtils;
import com.konantech.spring.util.MapUtil;
import com.konantech.spring.util.RequestUtils;
import org.apache.commons.collections.MapUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import javax.servlet.http.HttpServletRequest;
import java.security.Principal;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Service
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
public class SectionService {

    @Autowired
    private SectionMapper sectionMapper;

    @Autowired
    private ContentMapper contentMapper;

    public List<Map> getSectionList(HttpServletRequest request, Principal principal) {
        String idx = RequestUtils.getParameter(request, "idx");
        HashMap<String,String> param = new HashMap<>();
        param.put("userid"     ,principal.getName());
        param.put("idx"     ,idx);
        List<Map> list = sectionMapper.getSectionList(param);
        for(Map map : list){
            float rate = Float.parseFloat(RequestUtils.getParameter(request,"rate","1 "));
            float startframeindex = MapUtils.getFloatValue(map,"startframeindex");
            float endframeindex = MapUtils.getFloatValue(map,"endframeindex");
            map.put("startframeindex",startframeindex/rate);
            map.put("endframeindex",endframeindex/rate);
        }
        return list;
    }

    public int putQaSection(HttpServletRequest request, Principal principal) throws Exception{

        String videoid = RequestUtils.getParameter(request, "idx");
        String[] sectionid = RequestUtils.getParameterValues(request,"sectionid");
        String[] startshotid = RequestUtils.getParameterValues(request,"startshotid");
        String[] endshotid = RequestUtils.getParameterValues(request,"endshotid");
        String[] sectionname = RequestUtils.getParameterValues(request,"sectionname");
        String[] delflag = RequestUtils.getParameterValues(request,"delflag");
        int result = 0;
        for(int i = 0; i<sectionid.length; i++){
            HashMap<String,String> param = new HashMap<>();
            param.put("sectionid"  ,sectionid[i]);
            param.put("videoid"    ,videoid);
            param.put("startshotid",startshotid[i]);
            param.put("endshotid"  ,endshotid[i]);
            param.put("sectionname",sectionname[i]);
            param.put("userid"     ,principal.getName());
            if(delflag[i].equals("true")){
                if (!sectionid[i].equals(""))
                    result += sectionMapper.deleteSectionItems(Integer.parseInt(sectionid[i]));
            }else {
                if (sectionid[i].equals(""))
                    result += sectionMapper.putSectionItem(param);
                else
                    result += sectionMapper.setSectionItem(param);
            }
        }
        return result;
    }

    public List<Map> getDepictionList(HttpServletRequest request, Principal principal) {
        String sectionid = RequestUtils.getParameter(request, "sectionid");
        HashMap<String,String> param = new HashMap<>();
        param.put("userid"     ,principal.getName());
        param.put("sectionid"     ,sectionid);
        return sectionMapper.getDepictionList(param);
    }

    public int putDepiction(HttpServletRequest request, Principal principal) throws Exception{
        String sectionid = RequestUtils.getParameter(request, "sectionid");
        String[] depictionid = RequestUtils.getParameterValues(request,"depictionid");
        String[] depiction = RequestUtils.getParameterValues(request,"depiction");
        int result = 0;
        for(int i = 0; i<depictionid.length; i++){
            HashMap<String,String> param = new HashMap<>();
            param.put("depictionid" ,depictionid[i]);
            param.put("sectionid"   ,sectionid);
            param.put("depiction"   ,depiction[i]);
            param.put("userid"      ,principal.getName());
            if (depictionid[i].equals(""))
                result += sectionMapper.putDepictionItem(param);
            else
                result += sectionMapper.setDepictionItem(param);
        }
        return result;
    }

    public List<Map> getQuestionList(HttpServletRequest request, Principal principal) {
        String sectionid = RequestUtils.getParameter(request, "sectionid");
        HashMap<String,String> param = new HashMap<>();
        param.put("userid"     ,principal.getName());
        param.put("sectionid"     ,sectionid);
        return sectionMapper.getQuestionList(param);
    }

    public int putQuestionList(HttpServletRequest request, Principal principal) throws Exception{
        String sectionid = RequestUtils.getParameter(request, "sectionid");
        String[] questionid = RequestUtils.getParameterValues(request,"questionid");
        String[] question = RequestUtils.getParameterValues(request,"question");
        String[] answer = RequestUtils.getParameterValues(request,"answer");
        String[] wrong_answer1 = RequestUtils.getParameterValues(request,"wrong_answer1");
        String[] wrong_answer2 = RequestUtils.getParameterValues(request,"wrong_answer2");
        String[] wrong_answer3 = RequestUtils.getParameterValues(request,"wrong_answer3");
        String[] wrong_answer4 = RequestUtils.getParameterValues(request,"wrong_answer4");
        int result = 0;
        for(int i = 0; i<questionid.length; i++){
            HashMap<String,String> param = new HashMap<>();
            param.put("questionid" ,questionid[i]);
            param.put("sectionid"   ,sectionid);
            param.put("question"   ,question[i]);
            param.put("answer"   ,answer[i]);
            param.put("wrong_answer1"   ,wrong_answer1[i]);
            param.put("wrong_answer2"   ,wrong_answer2[i]);
            param.put("wrong_answer3"   ,wrong_answer3[i]);
            param.put("wrong_answer4"   ,wrong_answer4[i]);
            param.put("userid"      ,principal.getName());
            if (questionid[i].equals(""))
                result += sectionMapper.putQuestionItem(param);
            else
                result += sectionMapper.setQuestionItem(param);
        }
        return result;
    }

    public List<Map> getRelationList(HttpServletRequest request, Principal principal) {
        String videoid = RequestUtils.getParameter(request, "idx");
        HashMap<String,String> param = new HashMap<>();
        param.put("userid"     ,principal.getName());
        param.put("videoid"     ,videoid);
        return sectionMapper.getRelationList(param);
    }

    public int putRelationItem(HttpServletRequest request, Principal principal) throws Exception{
        String videoid = RequestUtils.getParameter(request, "idx");
        String[] relationid = RequestUtils.getParameterValues(request,"relationid");
        String[] subject_sectionid = RequestUtils.getParameterValues(request,"subject_sectionid");
        String[] object_sectionid = RequestUtils.getParameterValues(request,"object_sectionid");
        String[] relationcode = RequestUtils.getParameterValues(request,"relationcode");
        String[] delflag = RequestUtils.getParameterValues(request,"delflag");
        int result = 0;
        for(int i = 0; i<relationid.length; i++){
            HashMap<String,String> param = new HashMap<>();
            param.put("relationid" ,relationid[i]);
            param.put("videoid"   ,videoid);
            param.put("subject_sectionid"   ,subject_sectionid[i]);
            param.put("object_sectionid"   ,object_sectionid[i]);
            param.put("relationcode"   ,relationcode[i]);
            param.put("userid"      ,principal.getName());
            if(delflag[i].equals("true")){
                if (!relationid[i].equals(""))
                    result += sectionMapper.deleteRelationItems(Integer.parseInt(relationid[i]));
            }else {
                if (relationid[i].equals(""))
                    result += sectionMapper.putRelationItem(param);
                else
                    result += sectionMapper.setRelationItem(param);
            }
        }
        return result;
    }


    public String getJsonData(Map paramMap) throws Exception{

        Section sectionData = sectionMapper.getSectionJson(paramMap);


        List<Section.QaResult> qaResultList = sectionMapper.getQaResultsJson(paramMap);
        for(Section.QaResult qaResult : qaResultList){

            paramMap.put("sectionid",qaResult.getPeriod_num());

            List<Section.QaResult.RelatedPeriodInfo> relationList = sectionMapper.getRelationJson(paramMap);
            qaResult.setRelated_period_info(relationList);

            List<Section.QaResult.DescriptionInfo> descriptionList = sectionMapper.getDepictionJson(paramMap);
            qaResult.setDescription_info(descriptionList);

            List<Section.QaResult.Qa> qaList = sectionMapper.getQaJson(paramMap);
            qaResult.setQa_info(qaList);


        }
        sectionData.setQa_results(qaResultList);

        String resultJson= JSONUtils.jsonStringFromObject(sectionData);

        return resultJson;
    }
}