package com.konantech.spring.controller.rest;

import com.konantech.spring.domain.AssetsResponse;
import com.konantech.spring.service.RedisService;
import io.swagger.annotations.ApiOperation;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

/**
 * Created by gsm on 2017-06-15.
 */

@RestController
public class ResponseController {

    @Autowired
    RedisService redisService;

//
//    @ApiOperation(value = "테스트", notes = "테스트" )
//    @RequestMapping(value = "/response1", method = {RequestMethod.PUT})
//    public ResponseEntity<?> test1(@RequestBody AssetsResponse assetsResponse) throws Exception {
//        return ResponseEntity.ok(assetsResponse);
//    }

//    @ApiOperation(value = "테스트", notes = "테스트" )
//    @RequestMapping(value = "/response", method = {RequestMethod.PUT})
//    public ResponseEntity<?> response(@RequestBody String assetsResponse) throws Exception {
//        String key = "responseTest";
//        redisService.setObject(key, assetsResponse, 60);
//        return ResponseEntity.ok(assetsResponse);
//    }
//
//    @ApiOperation(value = "테스트", notes = "테스트" )
//    @RequestMapping(value = "/responseTest", method = {RequestMethod.GET})
//    public ResponseEntity<?> responseTest() throws Exception {
//        String key = "responseTest";
//        return ResponseEntity.ok(redisService.getObject(key));
//
//    }
//
}
