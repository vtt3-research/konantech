package com.konantech.spring.controller.rest;

import com.konantech.spring.exception.KonantechException;
import com.konantech.spring.response.ObjectResponse;
import com.konantech.spring.service.MetaService;
import com.konantech.spring.util.RequestUtils;
import io.swagger.annotations.ApiImplicitParam;
import io.swagger.annotations.ApiImplicitParams;
import io.swagger.annotations.ApiOperation;
import org.apache.commons.lang3.ObjectUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;
import springfox.documentation.annotations.ApiIgnore;

import javax.servlet.http.HttpServletRequest;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

@RestController
public class MetaController {


    @Autowired
    MetaService metaService;

    private static final String putMeta = "[  \n" +
            "   {  \n" +
            "      \"id\":\"0001\",\n" +
            "      \"type\":\"donut\",\n" +
            "      \"name\":\"Cake\",\n" +
            "      \"ppu\":0.55,\n" +
            "      \"batters\":{  \n" +
            "         \"batter\":[  \n" +
            "            {  \n" +
            "               \"id\":\"1001\",\n" +
            "               \"type\":\"Regular\"\n" +
            "            },\n" +
            "            {  \n" +
            "               \"id\":\"1002\",\n" +
            "               \"type\":\"Chocolate\"\n" +
            "            },\n" +
            "            {  \n" +
            "               \"id\":\"1003\",\n" +
            "               \"type\":\"Blueberry\"\n" +
            "            },\n" +
            "            {  \n" +
            "               \"id\":\"1004\",\n" +
            "               \"type\":\"Devil's Food\"\n" +
            "            }\n" +
            "         ]\n" +
            "      },\n" +
            "      \"topping\":[  \n" +
            "         {  \n" +
            "            \"id\":\"5001\",\n" +
            "            \"type\":\"None\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5002\",\n" +
            "            \"type\":\"Glazed\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5005\",\n" +
            "            \"type\":\"Sugar\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5007\",\n" +
            "            \"type\":\"Powdered Sugar\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5006\",\n" +
            "            \"type\":\"Chocolate with Sprinkles\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5003\",\n" +
            "            \"type\":\"Chocolate\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5004\",\n" +
            "            \"type\":\"Maple\"\n" +
            "         }\n" +
            "      ]\n" +
            "   },\n" +
            "   {  \n" +
            "      \"id\":\"0002\",\n" +
            "      \"type\":\"donut\",\n" +
            "      \"name\":\"Raised\",\n" +
            "      \"ppu\":0.55,\n" +
            "      \"batters\":{  \n" +
            "         \"batter\":[  \n" +
            "            {  \n" +
            "               \"id\":\"1001\",\n" +
            "               \"type\":\"Regular\"\n" +
            "            }\n" +
            "         ]\n" +
            "      },\n" +
            "      \"topping\":[  \n" +
            "         {  \n" +
            "            \"id\":\"5001\",\n" +
            "            \"type\":\"None\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5002\",\n" +
            "            \"type\":\"Glazed\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5005\",\n" +
            "            \"type\":\"Sugar\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5003\",\n" +
            "            \"type\":\"Chocolate\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5004\",\n" +
            "            \"type\":\"Maple\"\n" +
            "         }\n" +
            "      ]\n" +
            "   },\n" +
            "   {  \n" +
            "      \"id\":\"0003\",\n" +
            "      \"type\":\"donut\",\n" +
            "      \"name\":\"Old Fashioned\",\n" +
            "      \"ppu\":0.55,\n" +
            "      \"batters\":{  \n" +
            "         \"batter\":[  \n" +
            "            {  \n" +
            "               \"id\":\"1001\",\n" +
            "               \"type\":\"Regular\"\n" +
            "            },\n" +
            "            {  \n" +
            "               \"id\":\"1002\",\n" +
            "               \"type\":\"Chocolate\"\n" +
            "            }\n" +
            "         ]\n" +
            "      },\n" +
            "      \"topping\":[  \n" +
            "         {  \n" +
            "            \"id\":\"5001\",\n" +
            "            \"type\":\"None\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5002\",\n" +
            "            \"type\":\"Glazed\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5003\",\n" +
            "            \"type\":\"Chocolate\"\n" +
            "         },\n" +
            "         {  \n" +
            "            \"id\":\"5004\",\n" +
            "            \"type\":\"Maple\"\n" +
            "         }\n" +
            "      ]\n" +
            "   }\n" +
            "]";
    @ApiOperation(value = "메타 등록", notes = putMeta )
    @ApiImplicitParams({
            @ApiImplicitParam(name = "title", value = "제목", required = true, dataType = "string", paramType = "query", defaultValue = "제목"),
            @ApiImplicitParam(name = "content", value = "내용", required = true, dataType = "string", paramType = "query", defaultValue =  putMeta )
    })

    @RequestMapping(value = "/v2/putMeta", method = {RequestMethod.POST})
    public ResponseEntity<?> putMeta(HttpServletRequest request) throws Exception {
        try {
            Map<String, Object> param = new HashMap<>();
            param.put("title", RequestUtils.getParameter(request, "title"));
            param.put("content", RequestUtils.getParameter(request, "content"));

            return new ObjectResponse<>(metaService.putMeta(param));
        } catch (Exception e) {
            throw new KonantechException( 500, "putMeta 오류 입니다 ( " + e.getMessage() + " )");
        }
    }

    private static final String getMeta = "";
    @ApiOperation(value = "메타 조회", notes = getMeta )
    @ApiImplicitParams({
            @ApiImplicitParam(name = "idx", value = "IDX", required = true, dataType = "int", paramType = "path", defaultValue = "1"),
    })
    @RequestMapping(value = "/v2/getMeta/{idx}", method = {RequestMethod.GET})
    public ResponseEntity<?> getMeta( @PathVariable int idx) throws Exception {
        try {
            return new ObjectResponse<>(metaService.getMetaIdx(idx));
        } catch (Exception e) {
            throw new KonantechException( 500, "getMeta 오류 입니다 ( " + e.getMessage() + " )");
        }
    }


    private static final String getMetas = "메타 리스트 요청";
    @ApiOperation(value = "메타 목록", notes = getMetas )
    @ApiImplicitParams({
            @ApiImplicitParam(name = "cond", value = "JSON 조건", required = true, dataType = "string", paramType = "query", defaultValue = "{\"a\": 1}")
    })
    @RequestMapping(value = "/v2/metas", method = RequestMethod.GET)
    public ObjectResponse getMetas(@ApiIgnore HttpServletRequest httpServletRequest) throws Exception {
        try {
            String cond = RequestUtils.getParameter(httpServletRequest,"cond");
            return new ObjectResponse<>(metaService.getMetaList(cond));
        } catch (Exception e) {
            throw new KonantechException( 500, "getMetas 오류 입니다 ( " + e.getMessage() + " )");
        }
    }

    private static final String updateMetas = "메타 업데이트";
    @ApiOperation(value = "메타 업데이트", notes = getMetas )
    @ApiImplicitParams({
            @ApiImplicitParam(name = "cond", value = "JSON 조건", required = true, dataType = "string", paramType = "query", defaultValue = "{\"a\": 1}")
    })
    @RequestMapping(value = "/v2/updateMeta", method = RequestMethod.PUT)
    public ObjectResponse geupdateMetatMetas(@ApiIgnore HttpServletRequest httpServletRequest) throws Exception {
        try {
            String cond = RequestUtils.getParameter(httpServletRequest,"cond");
            return new ObjectResponse<>(metaService.getMetaList(cond));
        } catch (Exception e) {
            throw new KonantechException( 500, "getMetas 오류 입니다 ( " + e.getMessage() + " )");
        }
    }
    private static final String deleteMetas = "메타 삭제";
    @ApiOperation(value = "메타 삭제", notes = getMetas )
    @ApiImplicitParams({
            @ApiImplicitParam(name = "idx", value = "IDX", required = true, dataType = "int", paramType = "path", defaultValue = "1"),
    })
    @RequestMapping(value = "/v2/deleteMetas/{idx}", method = {RequestMethod.DELETE})
    public ObjectResponse deleteMetas(@ApiIgnore HttpServletRequest httpServletRequest) throws Exception {
        try {
            String cond = RequestUtils.getParameter(httpServletRequest,"cond");
            return new ObjectResponse<>(metaService.getMetaList(cond));
        } catch (Exception e) {
            throw new KonantechException( 500, "deleteMetas 오류 입니다 ( " + e.getMessage() + " )");
        }
    }



}