package com.konantech.spring.service.impl;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.konantech.spring.domain.content.ContentField;
import com.konantech.spring.domain.content.ContentQuery;
import com.konantech.spring.domain.convertobj.ClientJson;
import com.konantech.spring.domain.convertobj.ConvertObj;
import com.konantech.spring.domain.visual.Visual;
import com.konantech.spring.domain.vtt.RepImgVo;
import com.konantech.spring.mapper.ContentMapper;
import com.konantech.spring.mapper.VisualMapper;
import com.konantech.spring.service.VisualService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.*;

@Service("visualService")
public class VisualServiceImpl implements VisualService {


    @Autowired
    private VisualMapper visualMapper;

    @Autowired
    private ContentMapper contentMapper;


    @Override
    public int getPutRepreImg(Map map) throws Exception {
        return visualMapper.getPutRepreImg(map);
    }

    @Override
    public int getCntRepreImg(Map map) throws Exception {
        return visualMapper.getCntRepreImg(map);
    }

    @Override
    public List<RepImgVo> getSelectRepImg(Map map) throws Exception {
        List<RepImgVo> repImgList = visualMapper.getSelectRepImg(map);

        ObjectMapper mapper = new ObjectMapper();
        String jsonInStr ="";
        ClientJson clientJson = null;
        for(RepImgVo vo : repImgList){
            String getJsonStr = vo.getRepJson();
            if(!Boolean.valueOf(vo.getSavedChk())){
                clientJson = new ClientJson();
                //서강대 json=>client.json 파싱
                ConvertObj convertObj = mapper.readValue(getJsonStr, ConvertObj.class);
                String cap = convertObj.getCap();
                for (ConvertObj.Result result : convertObj.getResults()) {
                    for (ConvertObj.Result.Module_result module_result : result.getModule_result()) {
                        ConvertObj.Result.Module_result.Label label = module_result.getLabel().get(0);

                        //위치
                        ClientJson.Rect rect = new ClientJson.Rect();
                        rect.setX(module_result.getPosition().getX());
                        rect.setY(module_result.getPosition().getY());
                        rect.setW(module_result.getPosition().getW());
                        rect.setH(module_result.getPosition().getH());

                        if (result.getModule_name().equals("object")) {
                            if (label.getDescription().equals("person")) {
                                ClientJson.PersonFull personFull = new ClientJson.PersonFull();
                                personFull.setFull_rect(rect);
                                personFull.setScore(label.getScore());
                                clientJson.getPersonFull().add(personFull);
                            } else {
                                ClientJson.Object object = new ClientJson.Object();
                                object.setObject_rect(rect);
                                object.setObject_name(label.getDescription());
                                object.setScore(label.getScore());
                                clientJson.getObject().add(object);
                            }
                        } else if (result.getModule_name().equals("friends.face")) {
                            ClientJson.Person person = new ClientJson.Person();
                            person.setRect(rect);
                            person.setPerson_name(label.getDescription());
                            person.setScore(label.getScore());
                            clientJson.getPerson().add(person);
                        } else if (result.getModule_name().equals("friends.place")) {
                            ClientJson.Place place = new ClientJson.Place();
                            place.setPlace(label.getDescription());
                            place.setScore(label.getScore());
                            clientJson.getPlaces().add(place);
                        }
                    }
                }
                clientJson.setCap(cap);
            }else{
                //visual json=>client.json 파싱
                List<Map<String,Object>> visual = mapper.readValue(getJsonStr,  ArrayList.class);
                for(Map mVisualResult : visual) {

                    getJsonStr = mapper.writerWithDefaultPrettyPrinter().writeValueAsString(mVisualResult);
                    Visual.VisualResult visualResult = mapper.readValue(getJsonStr,  Visual.VisualResult.class);

                    if(!visualResult.getPeriod_frame_num().equals(vo.getRepImgSeq())){
                        continue;
                    }
                    clientJson = new ClientJson();

                    clientJson.setRelate_between_obj(visualResult.getRelate_between_obj());
                    clientJson.setEmotional_behavior(visualResult.getEmotional_behavior());

                    ClientJson.Place place = new ClientJson.Place();
                    place.setPerson_name(visualResult.getRelated_person());
                    place.setPlace(visualResult.getPlace());
                    place.setSpot(visualResult.getSpot());
                    place.setScore(visualResult.getScore());
                    clientJson.getPlaces().add(place);

                    //persons
                    for (Map<String, List<Visual.VisualResult.Person>> personMap : visualResult.getPerson()) {
                        for (String name : personMap.keySet()) {

                            Visual.VisualResult.Person person = personMap.get(name).get(0);

                            //인물
                            ClientJson.Person clientPerson = new ClientJson.Person();
                            clientPerson.setPerson_name(name);
                            clientPerson.setBehavior(person.getBehavior());
                            clientPerson.setEmotion(person.getEmotion());
                            clientPerson.setScore(person.getScore());
                            //위치
                            Visual.VisualResult.Rect mFace = person.getFace_rect();
                            if(mFace!=null) {
                                ClientJson.Rect rect = new ClientJson.Rect();
                                rect.setX(mFace.getMin_x());
                                rect.setY(mFace.getMin_y());
                                rect.setMax_x(mFace.getMax_x());
                                rect.setMax_y(mFace.getMax_y());
                                clientPerson.setRect(rect);
                            }
                            clientJson.getPerson().add(clientPerson);

                            //인물 전체
                            //위치
                            Visual.VisualResult.Rect mFull = person.getFull_rect();
                            if(mFull!=null) {
                                ClientJson.PersonFull clientPersonFull = new ClientJson.PersonFull();
                                clientPersonFull.setPerson_name(name);
                                ClientJson.Rect rect = new ClientJson.Rect();
                                rect.setX(mFull.getMin_x());
                                rect.setY(mFull.getMin_y());
                                rect.setMax_x(mFull.getMax_x());
                                rect.setMax_y(mFull.getMax_y());
                                clientPersonFull.setFull_rect(rect);
                                clientPersonFull.setScore(person.getScore());
                                clientJson.getPersonFull().add(clientPersonFull);
                            }

                            for (Visual.VisualResult.Person.RelatedObject relObject : person.getRelated_object()) {

                                //객체
                                ClientJson.Object object = new ClientJson.Object();
                                object.setObject_name(relObject.getObject_name());
                                object.setPerson_name(name);
                                object.setPredicate(relObject.getPredicate());
                                object.setScore(relObject.getScore());
                                //위치
                                Visual.VisualResult.Rect mObject = relObject.getObject_rect();
                                if(mObject!=null) {
                                    ClientJson.Rect rect = new ClientJson.Rect();
                                    rect.setX(mObject.getMin_x());
                                    rect.setY(mObject.getMin_y());
                                    rect.setMax_x(mObject.getMax_x());
                                    rect.setMax_y(mObject.getMax_y());
                                    object.setObject_rect(rect);
                                }
                                clientJson.getObject().add(object);
                            }
                        }
                    }
                    //object
                    for (Visual.VisualResult.VisualObject visualObject : visualResult.getObject()) {

                        if(!visualObject.getObject_name().equals("person")) {
                            //객체
                            ClientJson.Object object = new ClientJson.Object();
                            object.setObject_name(visualObject.getObject_name());
                            object.setScore(visualObject.getScore());
                            //위치
                            Visual.VisualResult.Rect mRect = visualObject.getObject_rect();
                            if (mRect != null) {
                                ClientJson.Rect rect = new ClientJson.Rect();
                                rect.setX(mRect.getMin_x());
                                rect.setY(mRect.getMin_y());
                                rect.setMax_x(mRect.getMax_x());
                                rect.setMax_y(mRect.getMax_y());
                                object.setObject_rect(rect);
                            }
                            clientJson.getObject().add(object);
                        }else{
                            //인물 전체
                            ClientJson.PersonFull clientPersonFull = new ClientJson.PersonFull();
                            clientPersonFull.setScore(visualObject.getScore());
                            //위치
                            Visual.VisualResult.Rect mRect = visualObject.getObject_rect();
                            if (mRect != null) {
                                ClientJson.Rect rect = new ClientJson.Rect();
                                rect.setX(mRect.getMin_x());
                                rect.setY(mRect.getMin_y());
                                rect.setMax_x(mRect.getMax_x());
                                rect.setMax_y(mRect.getMax_y());
                                clientPersonFull.setFull_rect(rect);
                            }
                            clientJson.getPersonFull().add(clientPersonFull);
                        }
                    }
                }
            }
            jsonInStr = mapper.writerWithDefaultPrettyPrinter().writeValueAsString(clientJson);
            vo.setRepJson(jsonInStr);
        }
        return repImgList;
    }

    @Override
    public int getPutMetaInfo(Map paramMap, List list) throws Exception {
        ObjectMapper mapper = new ObjectMapper();
        String repImgSeq = (String)paramMap.get("repImgSeq");
        String jsonInString = "";

        LinkedHashMap<String,Object> metaInfoList=getSelectMetaInfo(paramMap);
        if(metaInfoList != null&&metaInfoList.get("vtt_meta_json") != null){
            String jsonString =metaInfoList.get("vtt_meta_json").toString();
            List<Map> visualPojo = mapper.readValue(jsonString, ArrayList.class);
            boolean newVisualInfo = true;
            Object temp = null;
            for (Map mVisual : visualPojo) {
                String oldVisualJson = mapper.writerWithDefaultPrettyPrinter().writeValueAsString(mVisual);
                Visual.VisualResult oldVIsualResult = mapper.readValue(oldVisualJson, Visual.VisualResult.class);
                list.add(oldVIsualResult);
                if(repImgSeq.equals(oldVIsualResult.getPeriod_frame_num())){
                    newVisualInfo = false;
                    temp = oldVIsualResult;
                }
            }
            if(!newVisualInfo){
                list.remove(temp);
            }
        }
        Collections.sort(list, (o1, o2) -> {
            String so1 = ((Visual.VisualResult)o1).getPeriod_frame_num();
            String so2 = ((Visual.VisualResult)o2).getPeriod_frame_num();
            return so1.compareTo(so2);
        });
        jsonInString = mapper.writerWithDefaultPrettyPrinter().writeValueAsString(list);
        paramMap.put("vtt_meta_json", jsonInString);
        return visualMapper.getPutMetaInfo(paramMap);
    }
    @Override
    public int getPutMetaInfo(Map paramMap) throws Exception {
        return visualMapper.getPutMetaInfo(paramMap);
    }

    @Override
    public LinkedHashMap<String, Object> getSelectMetaInfo(Map<String, Object> map) throws Exception {
        return visualMapper.getSelectMetaInfo(map);
    }

    @Override
    public Map getRepImgInfo(Map map)  throws Exception {
        return visualMapper.getRepImgInfo(map);
    }

    @Override
    public int getPutSecInfo(Map<String, Object> map) throws Exception {
        return visualMapper.getPutSecInfo(map);
    }

    @Override
    public List<LinkedHashMap<String,Object>> getSecInfo(Map<String, Object> map) throws Exception {
        return visualMapper.getSecInfo(map);
    }

    @Override
    public String getJsonData(Map<String, Object> map) throws Exception {
        List<String> metaData = visualMapper.getJsonData(map);
        ObjectMapper mapper = new ObjectMapper();
        List<Visual.VisualResult> list = new ArrayList<>();
        for(String json : metaData){

            List<Map> visualResults = mapper.readValue(json, ArrayList.class);
            for(Map mVisualResult : visualResults){
                String sVisualJson = mapper.writerWithDefaultPrettyPrinter().writeValueAsString(mVisualResult);
                Visual.VisualResult visualResult = mapper.readValue(sVisualJson, Visual.VisualResult.class);
                list.add(visualResult);
            }
        }
        ContentQuery param = new ContentQuery();
        param.setIdx(Integer.parseInt((String)map.get("idx")));
        ContentField video = contentMapper.getContentItem(param);
        Visual result = new Visual();
        result.setFile_name(video.getOrifilename());
        result.setRegisted_name(video.getTitle());
        result.setVisual_results(list);
        String json = mapper.writeValueAsString(result);
        return json;
    }

    @Override
    public int deleteRepImg(Map<String, Object> map) throws Exception{
        int result = -1;
        visualMapper.deleteRepImg(map);
        LinkedHashMap<String, Object> metaInfo = visualMapper.getSelectMetaInfo(map);
        String json = (String)metaInfo.get("vtt_meta_json");


        return result;
    }

    @Override
    public List<Map> getWorkerList(Map<String,Object> param) throws Exception{
        return visualMapper.getWorkerList(param);
    }
}
