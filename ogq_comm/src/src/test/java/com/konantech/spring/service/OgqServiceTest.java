package com.konantech.spring.service;

import com.konantech.spring.CommonTests;
import com.konantech.spring.domain.ItemData;
import org.junit.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.transaction.annotation.Transactional;

import java.util.Date;
import java.util.List;

/**
 * Created by ossboard on 28/08/2017.
 */
public class OgqServiceTest extends CommonTests {

    @Autowired
    private OgqService ogqService;

    @Test
    public void itemList() throws Exception {
        List<ItemData> result = ogqService.itemList(null);
        printList(result);
    }

    @Test
    @Transactional
    public void itemAll() throws Exception {

        int result;
        ItemData itemData = new ItemData();
        itemData.setRequestId("1");
        itemData.setVideoUrl("2");
        itemData.setCallbackUrl("3");
        itemData.setCreateDate(new Date());
        itemData.setUpdateDate(new Date());
        itemData.setStatus(1);
        itemData.setIsUsed(1);
        result = ogqService.itemInsert( itemData );
        System.out.println("idx = " + itemData.getIdx());

        itemData.setVideoUrl("2-2");
        result = ogqService.itemUpdate(itemData);

        result = ogqService.itemDelete(itemData.getIdx());

    }

}