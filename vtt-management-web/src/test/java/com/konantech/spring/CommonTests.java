package com.konantech.spring;

import org.apache.commons.lang3.builder.ReflectionToStringBuilder;
import org.apache.commons.lang3.builder.ToStringStyle;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.context.junit4.SpringRunner;

import java.util.List;
import java.util.Map;

@RunWith(SpringRunner.class)
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

}

