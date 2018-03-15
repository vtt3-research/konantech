package com.konantech.spring.core;

import com.konantech.spring.domain.ItemData;
import com.konantech.spring.service.OgqService;
import com.konantech.spring.util.RestUtils;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.collections.map.HashedMap;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.InitializingBean;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Profile;
import org.springframework.http.ResponseEntity;
import org.springframework.scheduling.annotation.EnableScheduling;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import java.util.List;
import java.util.Map;

@Component
@Profile("!test")
@EnableScheduling
public class OgqBatch implements InitializingBean {

    private static Logger log = LoggerFactory.getLogger(OgqBatch.class);

    @Autowired
    OgqService ogqService;

    public void afterPropertiesSet() throws Exception {

    }

    @Value("${darc4.restUrl}")
    public String restUrl;

    @Value("${darc4.token}")
    public String token;

    @Scheduled(fixedDelay = 5000)
    public void darc4insert() throws Exception {

        System.out.println("i");
        Map<String, Object> param = new HashedMap();
        param.put("status", 0);
        List<ItemData> list = ogqService.itemList(param);
        for (ItemData item : list) {
            try {
                ResponseEntity responseEntity = RestUtils.darc4add(item);
                if (responseEntity != null) {
                    Map<String, Object> darc4items = (Map<String, Object>) ((Map) responseEntity.getBody()).get("items");
                    int videoid = MapUtils.getIntValue(darc4items, "videoid");
                    item.setVideoId(videoid);
                    item.setStatus(1);
                    RestUtils.darc4catalog(videoid);
                    ogqService.itemUpdate(item);
                } else {
                    item.setStatus(-2);
                    ogqService.itemUpdate(item);
                }
            } catch (Exception ignore) {
                item.setStatus(-5);
                ogqService.itemUpdate(item);
            }
        }
    }


    @Scheduled(fixedDelay = 5000)
    public void callback() throws Exception {

        System.out.println("c");
        Map<String, Object> param = new HashedMap();
        param.put("status", 1);
        List<ItemData> list = ogqService.itemList(param);
        for (ItemData item : list) {
            try {
                ResponseEntity responseEntity = ogqService.callback(item);
                if (responseEntity != null) {
                    if (responseEntity.getStatusCodeValue() == 200) {
                        item.setStatus(2);
                        ogqService.itemUpdate(item);
                    } else {
                        item.setStatus(responseEntity.getStatusCodeValue());
                        ogqService.itemUpdate(item);
                    }
                }
            } catch (Exception ignore) {
                item.setStatus(-3);
                ogqService.itemUpdate(item);
            }
        }
    }

}