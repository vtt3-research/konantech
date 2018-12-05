package com.konantech.spring.domain;

import com.fasterxml.jackson.databind.ObjectMapper;
import lombok.Data;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.lang3.StringUtils;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

@Data
public class SoGangParsing {

    private ArrayList<ObjectInfo> objectData;

    @Data
    public class ObjectInfo {
        String location;
        Info info;
    }

    @Data
    public class Info {
        String objectName;
        double accuracy;
    }


    public SoGangParsing(String jsonString) {

        try {
            ObjectMapper mapper = new ObjectMapper();
            HashMap<String, Object> map = mapper.readValue(jsonString, new HashMap<String, Object>().getClass());

            // object
            JSONArray jsonArrayObject = JSONArray.fromObject(map.get("objectData"));
            objectData = new ArrayList<>();
            for(Object obj : jsonArrayObject) {
                JSONArray jsonArray = (JSONArray) obj;
                Object obj1 = jsonArray.get(1);
                if ( obj1 instanceof JSONObject ) {
                    JSONArray rect = (JSONArray) jsonArray.get(0);
                    JSONObject name = (JSONObject) jsonArray.get(1);
                    objectData.add(getObjectInfo(rect, name));
                } else if ( obj1 instanceof JSONArray ) {
                    for (Object obj2 : (JSONArray) obj1) {
                        JSONArray rect = (JSONArray) ((JSONArray) obj1).get(0);
                        JSONObject name = (JSONObject) ((JSONArray) obj1).get(1);
                        objectData.add(getObjectInfo(rect, name));
                    }
                }
            }

        } catch (Exception ignore) {
            System.out.println(ignore.getMessage());
            //ignore
        }
    }


    private ObjectInfo getObjectInfo(JSONArray rect, JSONObject name  ) {
        ObjectInfo objectInfo = new ObjectInfo();
        List<Double> box = new ArrayList<>();
        for (double n : (List<Integer>) rect) {
            box.add(Double.parseDouble(String.format("%.02f", n)));
        }
        objectInfo.setLocation(StringUtils.join(box," "));

        for(Iterator<String> itr = name.keys(); itr.hasNext(); ) {
            Info i = new Info();
            i.setObjectName(itr.next());
            i.setAccuracy(Double.parseDouble(String.format("%.02f", MapUtils.getDouble(name,i.getObjectName()))));
            objectInfo.setInfo(i);
        }
        return objectInfo;
    }

}
