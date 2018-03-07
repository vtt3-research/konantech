package com.konantech.spring.util;

import com.konantech.spring.domain.AssetsGetRequest;
import com.konantech.spring.domain.AssetsResponse;
import com.konantech.spring.domain.DarcCond;
import com.konantech.spring.domain.ItemData;
import org.apache.commons.collections.map.HashedMap;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.io.IOUtils;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.core.io.ByteArrayResource;
import org.springframework.http.*;
import org.springframework.stereotype.Component;
import org.springframework.util.LinkedMultiValueMap;
import org.springframework.util.MultiValueMap;
import org.springframework.web.client.HttpStatusCodeException;
import org.springframework.web.client.RestClientException;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.util.UriComponentsBuilder;
import org.springframework.web.util.UriUtils;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.URI;
import java.net.URL;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

@Component
public class RestUtils {

    private static String restUrl;
    private static String token;
    private static String tempFolder;

    public RestUtils(
            @Value("${darc4.restUrl}") String restUrl,
            @Value("${darc4.token}") String token,
            @Value("${darc4.tempFolder}") String tempFolder
            ) {
        this.restUrl = restUrl;
        this.token = token;
        this.tempFolder = tempFolder;
    }

    public static URI makeURI(String path, Map<String, String> map) {

        MultiValueMap<String, String> params = new LinkedMultiValueMap<>();
        for( String key : map.keySet() ){
            params.add(key, map.get(key));
        }
        URI uri = UriComponentsBuilder.fromUriString(restUrl + path).queryParams(params).build().toUri();
        return uri;
    }

    public static URI makeURI(String path) {
        URI uri = UriComponentsBuilder.fromHttpUrl(restUrl).path(path).build().toUri();
        return uri;
    }

    public static ResponseEntity makeRestTemplate(URI uri, HttpMethod httpMethod, Map<String, String> restResponse) throws Exception {
        RestTemplate restTemplate = new RestTemplate();
        HttpHeaders headers = new HttpHeaders();

        MultiValueMap<String, String> params = new LinkedMultiValueMap<>();
        for( String key : restResponse.keySet() ){
            params.add(key, restResponse.get(key));
        }

        HttpEntity<Map> httpEntity = new HttpEntity<>(restResponse, headers);
        ResponseEntity<Map> responseEntity = null;
        try {
            HttpEntity<MultiValueMap<String, String>> request = new HttpEntity<MultiValueMap<String, String>>(params, headers);
            responseEntity = restTemplate.exchange(uri, httpMethod, httpEntity, Map.class);
        } catch (HttpStatusCodeException e) {
            MultiValueMap<String, String> multiValueMap = new LinkedMultiValueMap();
            Map<String, String> map = JSONUtils.jsonStringToMap(e.getResponseBodyAsString());
            for (String key : map.keySet()) {
                multiValueMap.add(key, map.get(key));
            }
            responseEntity = new ResponseEntity(multiValueMap, e.getStatusCode());
        } catch (RestClientException e) {
            e.getStackTrace();
        }

        return responseEntity;
    }

    public static ResponseEntity makeRestTemplateQuery(URI uri, HttpMethod httpMethod, Map<String, String> restResponse) throws Exception {

        RestTemplate restTemplate = new RestTemplate();
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_FORM_URLENCODED);

        MultiValueMap<String, String> params = new LinkedMultiValueMap<>();
        for( String key : restResponse.keySet() ){
            params.add(key, restResponse.get(key));
        }
        HttpEntity<?> request = new HttpEntity<>(params, headers);
        ResponseEntity<?> responseEntity = restTemplate.exchange(uri, httpMethod, request, Map.class);
        return responseEntity;
    }


    public static ResponseEntity makeRestTemplate(URI uri, HttpMethod httpMethod, AssetsGetRequest assetsRequest) throws Exception {
        RestTemplate restTemplate = new RestTemplate();
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);

        HttpEntity<AssetsGetRequest> httpEntity = new HttpEntity<>(assetsRequest, headers);
        ResponseEntity responseEntity = null;

        try {
            responseEntity = restTemplate.exchange(uri, httpMethod, httpEntity, Map.class);
        } catch (HttpStatusCodeException e) {
            MultiValueMap<String, String> multiValueMap = new LinkedMultiValueMap();
            Map<String, String> map = JSONUtils.jsonStringToMap(e.getResponseBodyAsString());
            for (String key : map.keySet()) {
                multiValueMap.add(key, map.get(key));
            }
            responseEntity = new ResponseEntity(multiValueMap, e.getStatusCode());
        } catch (RestClientException e) {
            e.getStackTrace();
        }

        return responseEntity;
    }

    public static ResponseEntity makeRestTemplate(URI uri, HttpMethod httpMethod, AssetsResponse assetsResponse) throws Exception {
        RestTemplate restTemplate = new RestTemplate();
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);

        HttpEntity<AssetsResponse> httpEntity = new HttpEntity<>(assetsResponse, headers);
        ResponseEntity responseEntity = null;

        try {
            responseEntity = restTemplate.exchange(uri, httpMethod, httpEntity, Map.class);
        } catch (HttpStatusCodeException e) {
            MultiValueMap<String, String> multiValueMap = new LinkedMultiValueMap();
            Map<String, String> map = JSONUtils.jsonStringToMap(e.getResponseBodyAsString());
            for (String key : map.keySet()) {
                multiValueMap.add(key, map.get(key));
            }
            responseEntity = new ResponseEntity(multiValueMap, e.getStatusCode());
        } catch (RestClientException e) {
            e.getStackTrace();
        }

        return responseEntity;
    }


    public static ResponseEntity makeRestTemplateGET(URI uri) throws Exception {
        RestTemplate restTemplate = new RestTemplate();
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);

        HttpEntity<AssetsResponse> httpEntity = new HttpEntity<>(headers);
        ResponseEntity responseEntity = restTemplate.exchange(uri, HttpMethod.GET, httpEntity, Map.class);
        return responseEntity;
    }

    static public ResponseEntity darc4add(Map<String, String> stateParam) throws Exception  {
        URI uri = RestUtils.makeURI("/v2/insert" );
        return RestUtils.makeRestTemplateQuery(uri, HttpMethod.PUT , stateParam);
    }

    static public ResponseEntity darc4add(ItemData item) throws Exception  {

        String source = item.getVideoUrl();
        int idx = item.getIdx();
        Path fn = Paths.get(source);
        URL url = new URL(UriUtils.encodePath(source,"UTF-8"));
        BufferedInputStream bis = new BufferedInputStream(url.openStream());
        String ext = FilenameUtils.getExtension(source);

        String target = FilenameUtils.normalize(tempFolder + "/" + idx + "." + ext);
        File fileTarget = new File(target);
        if (!fileTarget.exists()) {
            fileTarget.delete();
        }
        FileOutputStream fis = new FileOutputStream(target);
        byte[] buffer = new byte[1024];
        int count;
        while ((count = bis.read(buffer, 0, 1024)) != -1) {
            fis.write(buffer, 0, count);
        }
        fis.close();
        bis.close();

        InputStream is = url.openStream();
        byte[] data = IOUtils.toByteArray(is);
        ByteArrayResource resource = new ByteArrayResource(data){
            @Override
            public String getFilename() throws IllegalStateException {
                return fn.getFileName().toString();
            }
        };

        RestTemplate restTemplate = new RestTemplate();
        String serverUrl = restUrl + "/v2/upload";
        URI uri = URI.create(serverUrl);

        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.MULTIPART_FORM_DATA);

        MultiValueMap map = new LinkedMultiValueMap<String, Object>();
        map.add("title", item.getRequestId());
        map.add("content", "OGQ");
        map.add("orifilename", fn.getFileName().toString());
        map.add("file", resource);

        HttpEntity<?> request = new HttpEntity<>(map, headers);
        ResponseEntity<?> responseEntity = restTemplate.exchange(uri, HttpMethod.POST, request, Map.class);

        return responseEntity;
    }

    static public ResponseEntity darc4workflow(int videoid)  {

        Map<String, String> map = new HashedMap();
        try {
            URI uri = RestUtils.makeURI("/v2/asset-retry/catalogstatus/ast_br_video/" + videoid );
            return RestUtils.makeRestTemplate(uri, HttpMethod.GET , map);
        } catch ( Exception e) {
            System.out.println(e.getMessage());
            return null;
        }
    }
    static public ResponseEntity darc4info(int videoid)  {

        Map<String, String> map = new HashedMap();
        try {
            URI uri = RestUtils.makeURI("/v2/content/" + videoid );
            return RestUtils.makeRestTemplate(uri, HttpMethod.GET , map);
        } catch ( Exception e) {
            System.out.println(e.getMessage());
            return null;
        }
    }

    static public ResponseEntity darc4shots(int videoid)  {

        Map<String, String> map = new HashedMap();
        try {
            URI uri = RestUtils.makeURI("/v2/shots/" + videoid );
            return RestUtils.makeRestTemplateGET(uri);
        } catch ( Exception e) {
            System.out.println(e.getMessage());
            return null;
        }
    }

    static public ResponseEntity darc4scenes(int videoid)  {

        AssetsGetRequest assetsGetRequest = new AssetsGetRequest();
        assetsGetRequest.setLimit(0);
        List<DarcCond> conds = new ArrayList<>();
        DarcCond cond = new DarcCond();
        cond.setCond("=");
        cond.setKey("videoid");
        cond.setValue(String.valueOf(videoid));
        conds.add(cond);
        assetsGetRequest.setWhere(conds);

        Map<String, String> map = new HashedMap();
        try {
            URI uri = RestUtils.makeURI("/v2/assets/ast_br_scene" );
            return RestUtils.makeRestTemplate(uri, HttpMethod.POST , assetsGetRequest);
        } catch ( Exception e) {
            System.out.println(e.getMessage());
            return null;
        }
    }


    static public ResponseEntity callback(String url, AssetsResponse assetsResponse)  {
        try {
            URI uri = UriComponentsBuilder.fromHttpUrl(url).build().toUri();
            return RestUtils.makeRestTemplate(uri, HttpMethod.POST , assetsResponse);
        } catch ( Exception e) {
            System.out.println(e.getMessage());
            return null;
        }
    }


}