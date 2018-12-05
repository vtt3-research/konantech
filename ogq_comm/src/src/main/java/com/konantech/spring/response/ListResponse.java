package com.konantech.spring.response;

import com.fasterxml.jackson.annotation.JsonAnyGetter;
import com.fasterxml.jackson.annotation.JsonIgnore;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ListResponse<T> {
    @JsonIgnore
    private String listTitle;

    @JsonIgnore
    private Map<String, List<T>> list = new HashMap<>();

    private int limit;
    private int offset;
    private int total;

    public ListResponse() {
        this("items");
    }

    public ListResponse(String listTitle) {
        this.listTitle  = listTitle;
        this.list       = new HashMap<>(1);
    }

    public void setContent(List<T> content) {
        this.list.put(listTitle, content);
    }

    @JsonAnyGetter
    public Map<String, List<T>> getList() {
        return list;
    }

    public String getListTitle() {
        return listTitle;
    }

    public void setListTitle(String listTitle) {
        this.listTitle = listTitle;
    }

    public int getLimit() {
        return limit;
    }

    public void setLimit(int limit) {
        this.limit = limit;
    }

    public int getOffset() {
        return offset;
    }

    public void setOffset(int offset) {
        this.offset = offset;
    }

    public int getTotal() {
        return total;
    }

    public void setTotal(int total) {
        this.total = total;
    }
}
