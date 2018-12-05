package com.konantech.spring.service;

import com.konantech.spring.mapper.CrowdMapper;
import com.konantech.spring.util.RequestUtils;
import com.konantech.spring.util.ResultJSON;
import org.apache.commons.collections.map.HashedMap;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import javax.servlet.http.HttpServletRequest;
import java.text.SimpleDateFormat;
import java.util.*;

@Service
public class CrowdService{

    @Autowired
    CrowdMapper crowdMapper;

    @Value("${darc.shotServerUrl}")
    private String shotServerUrl;
    private Logger log = LoggerFactory.getLogger(CrowdService.class);

    public List getObjectImageList(HttpServletRequest request) throws Exception{
        Map param = RequestUtils.getParameterMap(request);
        param.put("type","object");
        //정답객체
        Map<String,String> inTargetMap = crowdMapper.getInObjectCode(param);
        //오답객체
        Map<String,String> outTargetMap = crowdMapper.getOutObjectCode(inTargetMap);

        //불확실한 다른 객체 2개
        inTargetMap.put("target","target");
        List<Map> targetMap = crowdMapper.getTargetData(inTargetMap);

        //확실한 다른객체 1개
        inTargetMap.put("target","answer");
        targetMap.addAll(crowdMapper.getTargetData(inTargetMap));

        //확실한 동일각체 6개
        outTargetMap.put("target","wrong");
        targetMap.addAll(crowdMapper.getTargetData(outTargetMap));

        //rect 좌표 계산 및 이미지 url 셋팅
        targetMap = setImageRect(targetMap);
        return targetMap;
    }

    public List getBackgroundImageList(HttpServletRequest request) throws Exception{
        Map param = RequestUtils.getParameterMap(request);
        param.put("type","background");
        //정답객체
        Map<String,String> inTargetMap = crowdMapper.getInObjectCode(param);
        //오답객체
        Map<String,String> outTargetMap = crowdMapper.getOutObjectCode(inTargetMap);

        //불확실한 다른 객체 1개
        inTargetMap.put("target","target");
        List<Map> targetMap = crowdMapper.getTargetData(inTargetMap);

        //확실한 다른객체 1개
        inTargetMap.put("target","answer");
        targetMap.addAll(crowdMapper.getTargetData(inTargetMap));

        //확실한 동일각체 2개
        outTargetMap.put("target","wrong");
        targetMap.addAll(crowdMapper.getTargetData(outTargetMap));

        ArrayList<Map> imgList = new ArrayList();
        for(Map<String,Object> data : targetMap) {
            data.put("filepath",shotServerUrl+"/"+data.get("imgurl"));
            imgList.add(data);
        }
        long seed = System.nanoTime();
        Collections.shuffle(imgList, new Random(seed));
        return imgList;
    }

    public Map getDragImageList(HttpServletRequest request) throws Exception{
        Map param = RequestUtils.getParameterMap(request);
        //확실객체 1개 이상 객체코드 3개
        List<Map> inTargetMap = crowdMapper.getInDragObjectCode(param);
        String objcode = "";
        for(Map map : inTargetMap){
            if(!objcode.equals("")) objcode+=",";
            objcode += "'"+map.get("objectcode")+"'";
        }
        param.put("objectcode", objcode);

        //불확실객체 3개 이상인 객체코드 1개
        Map<String,String> outTargetMap = crowdMapper.getOutDragObjectCode(param);

        //객체 코드, 코드명 4개
        inTargetMap.add(outTargetMap);

        objcode+=",";
        objcode += "'"+outTargetMap.get("objectcode")+"'";
        param.put("objectcode", objcode);

        //확실객체 랜덤 3개
        param.put("target", "answer");
        List<Map> imgList = crowdMapper.getDragTarget(param);
        //불확실객체 랜덤 3개
        param.put("target", "target");
        imgList.addAll(crowdMapper.getDragTarget(param));

        //rect 좌표 계산 및 이미지 url 셋팅
        imgList = setImageRect(imgList);

        param.put("objectList",inTargetMap);
        param.put("targetList",imgList);
        return param;
    }

    public ResultJSON putCrowdTarget(HttpServletRequest request) throws Exception{
        ResultJSON resultJSON = new ResultJSON();
        Map param = RequestUtils.getParameterMap(request);
        String[] aTargetid = (String[])param.get("targetid");
        String[] aTarget = (String[])param.get("target");
        String[] aCheckid = (String[])param.get("ischeck");
        String trustid = (String)param.get("userid");
        String sitecode = (String)param.get("site");
        String objectcode = (String)param.get("objectcode");
        String ip = RequestUtils.getRemoteAddr(request);
        int allcnt = 0;
        int wrong = 0;
        ArrayList<Map> answerList = new ArrayList();
        SimpleDateFormat sdfYm = new SimpleDateFormat("yyyyMM");
        String yyyymm  = sdfYm.format(new Date());
        param.put("date", yyyymm);

        //선택로그테이블 생성
        if (crowdMapper.checkLogTb(param) != 1) {
            crowdMapper.createLogTb(param);
        }

        for(int i = 0; i<aTarget.length; i++){
            String target = aTarget[i];
            HashMap answer = new HashMap();
            answer.put("targetid",aTargetid[i]);
            answer.put("ischeck",aCheckid[i]);
            if(target.equals("target")){
                answerList.add(answer);
            }else if(target.equals("answer")){
                allcnt+=1;
                if(aCheckid[i].equals("false")){
                    wrong+=1;
                }
            }else if(target.equals("wrong")){
                allcnt+=1;
                if(aCheckid[i].equals("true")){
                    wrong+=1;
                }
            }

            //선택로그 등록
            if(aCheckid[i].equals("true")) {
                answer.put("trustid",trustid);
                answer.put("sitecode",sitecode);
                answer.put("objectcode",objectcode);
                answer.put("userip",ip);
                answer.put("date",yyyymm);
                crowdMapper.putCrowdLog(answer);
            }
        }

        //오답이 없을때 결과저장장
        if(wrong==0){
            for(Map answer : answerList){
                crowdMapper.setTargetData(answer);
            }
            Map<String, Object> resultMap = new HashedMap();
            resultMap.put("message", "Thank you for your cooperation.");
            resultJSON.setData(resultMap);
        }else{
            resultJSON.error("You selected wrong picture.");
        }

        //산뢰도 반영
        HashMap answer = new HashMap();
        answer.put("trustid",trustid);
        answer.put("allcnt",allcnt);
        answer.put("chkcnt",wrong);
        answer.put("sitecode",sitecode);
        answer.put("userip",ip);
        crowdMapper.putUserTrust(answer);
        return resultJSON;
    }


    public ResultJSON putCrowdTargetDrag(HttpServletRequest request) throws Exception{
        ResultJSON resultJSON = new ResultJSON();
        Map param = RequestUtils.getParameterMap(request);
        String[] aTarget = (String[])param.get("target");
        String[] aObjectcode = (String[])param.get("objectcode");
        String[] aTargetcode = (String[])param.get("targetcode");
        String[] aTargetid = (String[])param.get("targetid");

        String trustid = (String)param.get("userid");
        String sitecode = (String)param.get("site");
        String ip = RequestUtils.getRemoteAddr(request);

        int allcnt = 0;
        int wrong = 0;
        ArrayList<Map> answerList = new ArrayList();
        SimpleDateFormat sdfYm = new SimpleDateFormat("yyyyMM");
        String yyyymm  = sdfYm.format(new Date());
        param.put("date", yyyymm);


        //선택로그테이블 생성
        if (crowdMapper.checkLogTb(param) != 1) {
            crowdMapper.createLogTb(param);
        }

        for(int i=0; i< aTarget.length; i++){
            String target = aTarget[i];
            String objectcode = aObjectcode[i];
            String targetcode = aTargetcode[i];
            String targetid = aTargetid[i];

            HashMap answer = new HashMap();
            answer.put("trustid",trustid);
            answer.put("sitecode",sitecode);
            answer.put("userip",ip);
            answer.put("date",yyyymm);
            if(target.equals("target")){
                //체크
                if(objectcode.equals(targetcode)){
                    answer.put("ischeck","true");
                }else{
                    answer.put("ischeck","false");
                }
                answerList.add(answer);
            }else if(target.equals("answer")){
                allcnt++;
                //오답
                if(!objectcode.equals(targetcode)){
                    wrong++;
                }
            }
            //선택로그 등록
            answer.put("targetid",targetid);
            answer.put("objectcode",targetcode);
            crowdMapper.putCrowdLog(answer);
        }

        //오답이 없을때 결과저장장
        if(wrong==0) {
            for (Map data : answerList) {
                crowdMapper.setTargetData(data);
            }
            Map<String, Object> resultMap = new HashedMap();
            resultMap.put("message", "Thank you for your cooperation.");
            resultJSON.setData(resultMap);
        }else{
            resultJSON.error("You selected wrong picture.");
        }

        //신뢰도 반영
       HashMap answer = new HashMap();
        answer.put("trustid",trustid);
        answer.put("allcnt",allcnt);
        answer.put("chkcnt",wrong);
        answer.put("sitecode",sitecode);
        answer.put("userip",ip);
        crowdMapper.putUserTrust(answer);
        return resultJSON;
    }

    private List setImageRect(List<Map> imgList){

        for(Map<String,Object> data : imgList) {
            String[] point = ((String)data.get("rect")).split(",");
            data.put("x",(Integer.parseInt(point[0])-10));
            data.put("y",(Integer.parseInt(point[1])-10));
            int width = Integer.parseInt(point[2])-Integer.parseInt(point[0]);
            data.put("width",(width+20));
            int height = Integer.parseInt(point[3])-Integer.parseInt(point[1]);
            data.put("height",(height+20));
            data.put("filepath",shotServerUrl+"/"+data.get("imgurl"));
        }
        long seed = System.nanoTime();
        Collections.shuffle(imgList, new Random(seed));

        return imgList;
    }
}
