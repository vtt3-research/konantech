package com.konantech.spring.service;

import org.springframework.core.io.FileSystemResource;
import org.springframework.core.io.Resource;
import org.springframework.http.*;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.util.LinkedMultiValueMap;
import org.springframework.util.MultiValueMap;
import org.springframework.web.client.RestTemplate;

import java.util.LinkedHashMap;
import java.util.Map;

@Service
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
public class RestService {
    public LinkedHashMap getImgMetaInfo(String imgPath) throws Exception {
        RestTemplate restTemplate = new RestTemplate();

        String url ="http://10.10.18.183:8000/analyzer/";

        HttpHeaders headers = new HttpHeaders();

        //헤더 내임, 값 셋팅.
        String headerName = "";
        String headerValue = "";
        headers.setContentType(MediaType.MULTIPART_FORM_DATA);

        headers.add(headerName, headerValue);

        MultiValueMap<String, Object> parameters = new LinkedMultiValueMap<>();

        Resource resource = new FileSystemResource(imgPath);
        parameters.add("Content-Type", "multipart/form-data");
        parameters.add("image", resource);
        parameters.add("modules", "friends");

        HttpEntity<MultiValueMap<String, Object>> requestEntity = new HttpEntity<>(parameters, headers);

        ResponseEntity<LinkedHashMap> result = restTemplate.exchange(url, HttpMethod.POST, requestEntity, LinkedHashMap.class);

        //System.out.println("result :" +result.getBody());
        LinkedHashMap mResult = result.getBody();
        mResult.put("cap",getImgQaInfo(imgPath).get("cap"));
        return mResult;
    }


    public LinkedHashMap getImgQaInfo(String imgPath) throws Exception {
        RestTemplate restTemplate = new RestTemplate();

        String url ="http://10.10.18.193:3333/cap";

        HttpHeaders headers = new HttpHeaders();

        //헤더 내임, 값 셋팅.
        String headerName = "";
        String headerValue = "";
        headers.setContentType(MediaType.MULTIPART_FORM_DATA);

        headers.add(headerName, headerValue);

        MultiValueMap<String, Object> parameters = new LinkedMultiValueMap<>();

        Resource resource = new FileSystemResource(imgPath);
        parameters.add("Content-Type", "multipart/form-data");
        parameters.add("image", resource);
        parameters.add("modules", "friends");

        HttpEntity<MultiValueMap<String, Object>> requestEntity = new HttpEntity<>(parameters, headers);

        ResponseEntity<LinkedHashMap> result = restTemplate.exchange(url, HttpMethod.POST, requestEntity, LinkedHashMap.class);

        //System.out.println("result :" +result.getBody());
        return result.getBody();
    }
}
