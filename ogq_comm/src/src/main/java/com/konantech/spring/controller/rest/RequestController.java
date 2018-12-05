package com.konantech.spring.controller.rest;

import com.konantech.spring.constants.ErrorCode;
import com.konantech.spring.domain.AssetsRequest;
import com.konantech.spring.domain.AssetsResponse;
import com.konantech.spring.domain.DarcStatus;
import com.konantech.spring.domain.ItemData;
import com.konantech.spring.exception.KonantechException;
import com.konantech.spring.response.ObjectResponse;
import com.konantech.spring.exception.FieldException;
import com.konantech.spring.service.OgqService;
import com.konantech.spring.util.RestUtils;
import com.konantech.spring.util.ThreadUtil;
import io.swagger.annotations.ApiImplicitParam;
import io.swagger.annotations.ApiImplicitParams;
import io.swagger.annotations.ApiOperation;
import org.apache.commons.collections.MapUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletRequest;
import javax.validation.Valid;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * Created by gsm on 2017-06-15.
 */

@RestController
public class RequestController {

    @Autowired
    private OgqService ogqService;

    @ApiOperation(value = "요청 API", notes = "예제</br>{</br>" +
                "  \"callback_url\": \"http://bgh.ogqcorp.com/video_tagging/callback/1sdv23d4234\",</br>" +
                "  \"request_id\": \"1sdv23d4234\",</br>" +
                "  \"video_url\": \"http://s3.amazon.com/media/image/2017-06/te33033.mp4\"</br>" +
                "}", response = AssetsRequest.class)
        @ApiImplicitParams({
                @ApiImplicitParam(name = "assetsRequest", value = "파라미터 JSON", required = false, dataType = "AssetsRequest", paramType = "body")
        })
        @RequestMapping(value = "/videoTagging", method = {RequestMethod.PUT})
            public ObjectResponse<Object> callVideoTagging(@Valid @RequestBody AssetsRequest assetsRequest, BindingResult result, HttpServletRequest request) throws Exception {

                if (result.hasErrors()) {
            throw new FieldException(result);
        }

        int uuid = 0;
        ItemData itemData = new ItemData();
        itemData.setRequestId(assetsRequest.getRequest_id());
        itemData.setVideoUrl(assetsRequest.getVideo_url());
        itemData.setCallbackUrl(assetsRequest.getCallback_url());
        itemData.setCreateDate(new Date());
        itemData.setUpdateDate(new Date());
        itemData.setStatus(0);
        itemData.setIsUsed(1);
        if(ogqService.itemInsert( itemData ) > 0) {
            uuid = itemData.getIdx();
        }

        /*
        //1. video_url 정보를 아카이브에 전송
        System.out.println("Go to sleep~~~~");

        try {
            ThreadUtil threadUtil = new ThreadUtil();
            threadUtil.doArchive(vUrl, cUrl);
            threadUtil.finish();
        }catch (Exception e){
            e.printStackTrace();
        }

        */

        String port = (request.getServerPort() == 80) ? "" : ":" + request.getServerPort();
        String url = "http://" + request.getServerName() + port + request.getContextPath();

        Map<String, Object> map = new LinkedHashMap<>();
        map.put("callback_url", assetsRequest.getCallback_url());
        map.put("uuid", uuid );
        map.put("check_url", url + "/status/"+ uuid);
        return new ObjectResponse<>(map);
    }

    @ApiOperation(value = "결과(상태)")
    @ApiImplicitParams({
            @ApiImplicitParam(name = "idx", value = "UUID", required = false, dataType = "string", paramType = "path", defaultValue="1")
    })
    @RequestMapping(value = "/v2/status/{idx}", method = {RequestMethod.GET})
    public ObjectResponse<?> status(HttpServletRequest request, @PathVariable int idx) throws Exception {
        try {
            Map<String, Object> info = ogqService.getDarc4Info(idx);
            DarcStatus status = ogqService.getStatus(info);
            if(status.getCatalogstatus() == 3000) {
                AssetsResponse assetsResponse = ogqService.getShotTgas(MapUtils.getIntValue(info,"idx"));
                return new ObjectResponse<>(assetsResponse);
            } else {
                return new ObjectResponse<>(status);
            }
        } catch (Exception e) {
            throw new KonantechException(500, "status 오류 입니다 ( " + e.getMessage() + " )");
        }
    }

    @ApiOperation(value = "콜백(재시도)")
    @ApiImplicitParams({
            @ApiImplicitParam(name = "idx", value = "UUID", required = false, dataType = "string", paramType = "path", defaultValue="1")
    })
    @RequestMapping(value = "/v2/callback/retry/{idx}", method = {RequestMethod.GET})
    public ObjectResponse<?> callbackRetry(HttpServletRequest request, @PathVariable int idx) throws Exception {
        try {
            ItemData info = ogqService.itemInfo(idx);
            Map<String, Object> map = new LinkedHashMap<>();
            map.put("callback_url", info.getCallbackUrl());
            ResponseEntity responseEntity = ogqService.callback(info);
            if (responseEntity != null && responseEntity.getStatusCodeValue() == 200) {
                map.put("result", "SUCCESS");
            } else {
                map.put("result", "FAIL");
            }
            return new ObjectResponse<>(map);
        } catch (Exception e) {
            throw new KonantechException(500, "callback 호출 오류 입니다 ( " + e.getMessage() + " )");
        }
    }

}
