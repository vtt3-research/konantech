package com.konantech.spring.util;

import com.konantech.spring.domain.AssetsResponse;
import com.konantech.spring.domain.ShotInfo;
import org.apache.commons.collections.map.HashedMap;
import org.apache.commons.lang3.StringUtils;
import org.springframework.http.*;
import org.springframework.util.LinkedMultiValueMap;
import org.springframework.util.MultiValueMap;
import org.springframework.web.client.HttpStatusCodeException;
import org.springframework.web.client.RestClientException;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.util.UriComponentsBuilder;

import java.net.URI;
import java.nio.channels.CompletionHandler;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created by H1608118 on 2016-09-30.
 */
public class ThreadUtil {

    private ExecutorService executorService;

    public  ThreadUtil() {
        executorService = Executors.newFixedThreadPool(1);
    }

    private CompletionHandler<Integer, Void> callback = new CompletionHandler<Integer, Void>() {

        @Override
        public void completed(Integer result, Void attachment) {
            System.out.println("completed() finish : " + result);
        }

        @Override
        public void failed(Throwable exc, Void attachment) {
            System.out.println("Thread Error : " + exc.toString());
        }
    };

    public void finish(){
        executorService.shutdown(); //현재 처리 중인 작업뿐만아니라 대기하고 있는 모든 작업을 처리한 뒤에 쓰레드 풀을 종료시킴.
    }

    public void doArchive(String vUrl, String cUrl) {
        Runnable task = new Runnable() {
            @Override
            public void run() {
                try {
                    //2. video_url로 접근....아카이브작업...
                    Thread.sleep(3000);

                    //Test 용 데이터....
                    AssetsResponse assetsResponse = new AssetsResponse();

                    List<String> tags = new ArrayList<>();
                    tags.add("recreation");
                    tags.add("fireworks");
                    tags.add("event");
                    tags.add("sea");
                    tags.add("day");

                    List<ShotInfo> shotInfos = new ArrayList<>();
                    for(int i = 0; i <= 10; i++){
//                        shotInfos.add(new ShotInfo(Long.valueOf(i), Long.valueOf(i), tags));
                    }

                    assetsResponse.setTags(tags);
                    assetsResponse.setShots(shotInfos);

                    Map<String, String> map = new HashedMap();

                    URI uri = UriComponentsBuilder.fromHttpUrl(cUrl).build().expand(map).encode().toUri();
                    ResponseEntity responseEntity = makeRestTemplate(uri, HttpMethod.PUT, assetsResponse);

                    Map<String, String> resultMap = new HashedMap();
                    int statusCode = Integer.parseInt(responseEntity.getStatusCode().toString());
                    if (statusCode == 200) {
                        resultMap = (Map) responseEntity.getBody();
                    } else {
                        HttpHeaders httpHeaders = responseEntity.getHeaders();
                        for (String key : httpHeaders.keySet()) {
                            resultMap.put(key, httpHeaders.get(key).toString());
                        }
                    }
                    System.out.println(resultMap);

                } catch (NumberFormatException e) {
                    callback.failed(e, null);
                } catch (Exception e) {
                    throw new Error("쓰레드 생성시 에러 ( "+ e.getMessage() + " )");
                }
            }
        };
        executorService.execute(task);
    }

    private ResponseEntity makeRestTemplate(URI uri, HttpMethod httpMethod, AssetsResponse assetsResponse) throws Exception {
        RestTemplate restTemplate = new RestTemplate();
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);

        HttpEntity<AssetsResponse> httpEntity = new HttpEntity<>(assetsResponse, headers);
        ResponseEntity responseEntity = null;

        try {
            responseEntity = restTemplate.exchange(uri, httpMethod, httpEntity, Map.class);
        } catch (HttpStatusCodeException e) {
            MultiValueMap<String, String> multiValueMap = new LinkedMultiValueMap();
            Map<String, String> map;
            if(StringUtils.isEmpty(e.getResponseBodyAsString())){
                multiValueMap.add("i18n", e.getLocalizedMessage());
                multiValueMap.add("status", e.getStatusCode().toString());
                multiValueMap.add("path", uri.getPath());
            }else{
                map = JSONUtils.jsonStringToMap(e.getResponseBodyAsString());

                for (String key : map.keySet()) {
                    multiValueMap.add(key, map.get(key));
                }
            }

            responseEntity = new ResponseEntity(multiValueMap, e.getStatusCode());
        } catch (RestClientException e) {
            e.getStackTrace();
        }

        return responseEntity;
    }


}
