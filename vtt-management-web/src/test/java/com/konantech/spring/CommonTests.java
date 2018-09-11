package com.konantech.spring;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.konantech.spring.domain.content.ContentQuery;
import com.konantech.spring.domain.storyboard.ShotTB;
import com.konantech.spring.domain.visual.Visual;
import com.konantech.spring.mapper.ContentMapper;
import com.konantech.spring.mapper.StoryboardMapper;
import com.konantech.spring.mapper.VisualMapper;
import com.konantech.spring.service.VisualService;
import org.apache.commons.lang3.builder.ReflectionToStringBuilder;
import org.apache.commons.lang3.builder.ToStringStyle;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.mock.web.MockHttpServletRequest;
import org.springframework.test.context.junit4.SpringRunner;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@RunWith(SpringRunner.class)
@SpringBootTest
public class CommonTests {

    private Logger log = LoggerFactory.getLogger(this.getClass());
    public MockHttpServletRequest req = new MockHttpServletRequest();

    @Autowired
    private VisualMapper visualMapper;
    @Test
    public void CommonTests() throws Exception {

    }

    public void printObj(Object obj) {

        if (obj instanceof Map) {
            System.out.println(obj);
        } else {
            System.out.println(ReflectionToStringBuilder.toString(obj, ToStringStyle.MULTI_LINE_STYLE));
        }

    }

    public void printList(List<?> list) {
        for (Object obj : list) {
            printObj(obj);
        }
    }

    @Autowired
    private VisualService visualService; //(신규) VTTM(메타정보 관리) 관련 서비스
    @Autowired
    private ContentMapper contentMapper;
    @Autowired
    StoryboardMapper storyboardMapper;

    @Test
    public void jsonPasing() throws Exception{
        String[] aFilePath = {
                "s01_ep07_tag2_visual_Final_for_merge.json"
                ,"s01_ep08_tag2_visual_Final_for_merge.json"
                ,"s01_ep09_tag2_visual_Final_for_merge.json"
                ,"s01_ep10_tag2_visual_Final_for_merge.json"
        };
        String[] aVideoId = {
                "658"
                ,"659"
                ,"660"
                ,"661"
        };
        for(int i = 0 ; i<aFilePath.length; i++) {
            String base = "/Volumes/konan/VTT/2차년도/02. 메타데이터 관련/VTT3_2차년도_메타데이터태깅/20180824_메타데이터_1차배포/VTT3_2차년도_메타데이터1차_배포_20180824_visual수정/";
            File jsonFile = new File(base+aFilePath[i]);
            ObjectMapper mapper = new ObjectMapper();
            Visual visual = mapper.readValue(jsonFile, Visual.class);
            ContentQuery vo = new ContentQuery();
            vo.setIdx(Integer.parseInt(aVideoId[i]));
            List<ShotTB> shotList = storyboardMapper.getShotList(vo);
            List<Visual.VisualResult> list = new ArrayList<>();
            for (Visual.VisualResult visualResult : visual.getVisual_results()) {
                List<Map<String, List<Visual.VisualResult.Person>>> oldPerson = visualResult.getPerson();
                List<Map<String, List<Visual.VisualResult.Person>>> newPerson = new ArrayList<>();
                Map<String, List<Visual.VisualResult.Person>> mPerson = new HashMap<>();
                newPerson.add(mPerson);
                for (String key : oldPerson.get(0).keySet()) {
                    List<Visual.VisualResult.Person> person = oldPerson.get(0).get(key);
                    String mKey = key.substring(0, 1).toUpperCase() + key.substring(1);
                    newPerson.get(0).put(mKey, person);
                }
                visualResult.setPerson(newPerson);

                if(list.size()!=0&&!list.get(0).getPeriod_num().equals(visualResult.getPeriod_num())){
                    String json = mapper.writerWithDefaultPrettyPrinter().writeValueAsString(list);
                    String shotId = "";
                    for(ShotTB shot : shotList){
                        if(shot.getAssetfilename().startsWith(list.get(0).getPeriod_num())){
                            shotId=shot.getShotid()+"";
                            break;
                        }
                    }

                    Map<String, String> paramMap = new HashMap<>();
                    paramMap.put("idx", aVideoId[i]);
                    paramMap.put("shotId", shotId);
                    paramMap.put("userId", "konan");
                    paramMap.put("vttMetaJson", json);
                    visualService.getPutMetaInfo(paramMap);
                    list.clear();
                }else if(visual.getVisual_results().indexOf(visualResult)+1==visual.getVisual_results().size()){
                    visualResult.setPeriod_frame_num((list.size()+1)+"");
                    list.add(visualResult);

                    String json = mapper.writerWithDefaultPrettyPrinter().writeValueAsString(list);
                    String shotId = "";
                    for(ShotTB shot : shotList){
                        if(shot.getAssetfilename().startsWith(list.get(0).getPeriod_num())){
                            shotId=shot.getShotid()+"";
                            break;
                        }
                    }

                    HashMap<String, String> paramMap = new HashMap<>();
                    paramMap.put("idx", aVideoId[i]);
                    paramMap.put("shotId", shotId);
                    paramMap.put("userId", "konan");
                    paramMap.put("vttMetaJson", json);
                        visualService.getPutMetaInfo(paramMap);

                }
                visualResult.setPeriod_frame_num((list.size()+1)+"");
                list.add(visualResult);

            }
            contentMapper.deleteRepImg(Integer.parseInt(aVideoId[i]));
            contentMapper.insertRepImg(Integer.parseInt(aVideoId[i]));
        }

    }
}

