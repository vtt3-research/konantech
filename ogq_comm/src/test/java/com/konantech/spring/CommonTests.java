package com.konantech.spring;

import com.konantech.spring.core.OgqBatch;
import com.konantech.spring.response.ListResponse;
import org.apache.commons.lang3.builder.ReflectionToStringBuilder;
import org.apache.commons.lang3.builder.ToStringStyle;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.FilterType;
import org.springframework.scheduling.annotation.EnableScheduling;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.test.context.ActiveProfiles;
import org.springframework.test.context.junit4.SpringRunner;

import java.util.List;
import java.util.Map;

@RunWith(SpringRunner.class)
@ActiveProfiles("test")
@SpringBootTest
public class CommonTests {

    private Logger log = LoggerFactory.getLogger(this.getClass());

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


    public void dumpListResponse(ListResponse result) {

        System.out.println();
        System.out.println("limit = " + result.getLimit());
        System.out.println("offset = " + result.getOffset());
        System.out.println("totalCount = " + result.getTotal());
        System.out.println("================================================");
        for (Map<String, Object> asset : (List<Map<String, Object>>) result.getList().get("items")) {
            printObj(asset);
        }
        System.out.println();
    }

}

