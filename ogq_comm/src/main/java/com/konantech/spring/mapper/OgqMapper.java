package com.konantech.spring.mapper;

import com.konantech.spring.domain.ItemData;
import org.apache.ibatis.annotations.Mapper;

import java.util.List;
import java.util.Map;


@Mapper
public interface OgqMapper {

    ItemData itemInfo(int idx);
    List<ItemData> itemList(Map<String, Object> param);
    int itemInsert(ItemData param);
    int itemUpdate(ItemData param);
    int itemDelete(int idx);

}
