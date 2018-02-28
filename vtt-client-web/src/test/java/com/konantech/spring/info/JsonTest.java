package com.konantech.spring.info;

import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.type.CollectionType;
import com.konantech.spring.domain.workflow.CompJobParamProperty;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;
import org.junit.Test;

import java.io.IOException;
import java.util.HashMap;
import java.util.List;

public class JsonTest {



    @Test
    public void test1() throws IOException {

        String jsonString = "{\"paramlist\": [{ \"type\": \"mainkey\", \"field\": \"idx\" }] }";
        ObjectMapper mapper = new ObjectMapper();
        HashMap<String, Object> map = mapper.readValue(jsonString, new HashMap<String, Object>().getClass());
        String json = mapper.writeValueAsString(map.get("paramlist"));
        CollectionType javaType = mapper.getTypeFactory().constructCollectionType(List.class, CompJobParamProperty.class);
        List<CompJobParamProperty> list = mapper.readValue(json, javaType );
        System.out.println(list);
        for( CompJobParamProperty l : list ) {
            System.out.println(l);
        }

    }
    @Test
    public void test2() throws IOException {

        String jsonString = "{\"paramlist\": [{ \"type\": \"mainkey\", \"field\": \"idx\" }] }";
        ObjectMapper mapper = new ObjectMapper();
        HashMap<String, Object> map = mapper.readValue(jsonString, new HashMap<String, Object>().getClass());
        String json = mapper.writeValueAsString(map.get("paramlist"));
        List<CompJobParamProperty> list = mapper.readValue(json, new TypeReference<List<CompJobParamProperty>>() { });
        System.out.println(list);
        for( CompJobParamProperty l : list ) {
            System.out.println(l);
        }

    }


    @Test
    public void test3() throws Exception {

        String jsonString = "{\"streams\": [{\"refs\": 1, \"tags\": {\"language\": \"und\", \"handler_name\": \"ISO Media file produced by Google Inc.\", \"creation_time\": \"2017-02-20T05:06:19.000000Z\"}, \"index\": 0, \"level\": 31, \"width\": 1280, \"height\": 720, \"is_avc\": \"true\", \"pix_fmt\": \"yuv420p\", \"profile\": \"Main\", \"bit_rate\": \"1681574\", \"duration\": \"239.989744\", \"codec_tag\": \"0x31637661\", \"nb_frames\": \"5754\", \"start_pts\": 0, \"time_base\": \"1/90000\", \"codec_name\": \"h264\", \"codec_type\": \"video\", \"start_time\": \"0.000000\", \"coded_width\": 1280, \"color_range\": \"tv\", \"color_space\": \"bt709\", \"disposition\": {\"dub\": 0, \"forced\": 0, \"lyrics\": 0, \"comment\": 0, \"default\": 1, \"karaoke\": 0, \"original\": 0, \"attached_pic\": 0, \"clean_effects\": 0, \"visual_impaired\": 0, \"hearing_impaired\": 0, \"timed_thumbnails\": 0}, \"duration_ts\": 21599077, \"coded_height\": 720, \"has_b_frames\": 1, \"r_frame_rate\": \"24000/1001\", \"avg_frame_rate\": \"517860000/21599077\", \"color_transfer\": \"bt709\", \"chroma_location\": \"left\", \"codec_long_name\": \"H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10\", \"codec_time_base\": \"21599077/1035720000\", \"color_primaries\": \"bt709\", \"nal_length_size\": \"4\", \"codec_tag_string\": \"avc1\", \"bits_per_raw_sample\": \"8\", \"sample_aspect_ratio\": \"1:1\", \"display_aspect_ratio\": \"16:9\"}, {\"tags\": {\"language\": \"und\", \"handler_name\": \"ISO Media file produced by Google Inc.\", \"creation_time\": \"2017-02-20T05:06:19.000000Z\"}, \"index\": 1, \"profile\": \"LC\", \"bit_rate\": \"125595\", \"channels\": 2, \"duration\": \"240.001451\", \"codec_tag\": \"0x6134706d\", \"nb_frames\": \"10336\", \"start_pts\": 0, \"time_base\": \"1/44100\", \"codec_name\": \"aac\", \"codec_type\": \"audio\", \"sample_fmt\": \"fltp\", \"start_time\": \"0.000000\", \"disposition\": {\"dub\": 0, \"forced\": 0, \"lyrics\": 0, \"comment\": 0, \"default\": 1, \"karaoke\": 0, \"original\": 0, \"attached_pic\": 0, \"clean_effects\": 0, \"visual_impaired\": 0, \"hearing_impaired\": 0, \"timed_thumbnails\": 0}, \"duration_ts\": 10584064, \"sample_rate\": \"44100\", \"r_frame_rate\": \"0/0\", \"avg_frame_rate\": \"0/0\", \"channel_layout\": \"stereo\", \"bits_per_sample\": 0, \"codec_long_name\": \"AAC (Advanced Audio Coding)\", \"codec_time_base\": \"1/44100\", \"codec_tag_string\": \"mp4a\"}]}";
        ObjectMapper mapper = new ObjectMapper();
        HashMap<String, Object> map = mapper.readValue(jsonString, new HashMap<String, Object>().getClass());
        JSONArray jsonArray = JSONArray.fromObject(map.get("streams"));
        if(jsonArray != null && jsonArray.size() > 0) {
            JSONObject o1 = (JSONObject) jsonArray.get(0);
            int width = (int) o1.get("width");
            int height = (int) o1.get("height");
            System.out.println(width);
            System.out.println(height);
        }




    }



}
