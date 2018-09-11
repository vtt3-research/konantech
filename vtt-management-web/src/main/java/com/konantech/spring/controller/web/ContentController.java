package com.konantech.spring.controller.web;

import com.konantech.spring.domain.content.ContentQuery;
import com.konantech.spring.domain.content.VideoFile;
import com.konantech.spring.domain.response.ListResponse;
import com.konantech.spring.response.BasicResponse;
import com.konantech.spring.service.ContentService;
import com.konantech.spring.util.RequestUtils;
import net.sf.json.JSONObject;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.lang3.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import javax.servlet.http.HttpServletRequest;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.Normalizer;
import java.text.SimpleDateFormat;
import java.util.*;

@Controller
public class ContentController {

    @Autowired
    private ContentService contentService;

    @RequestMapping(value = "/content", method = RequestMethod.GET)
    public String content(ModelMap modelMap, HttpServletRequest request) throws Exception {
        Map<String, Object> queryMap = RequestUtils.getParameterMap(request);
        modelMap.addAttribute("queryMap", queryMap);
        return "content/content";
    }

   @RequestMapping(value = "/content/list")
    public String contentList(ModelMap modelMap, HttpServletRequest request) throws Exception {

       int offset = RequestUtils.getParameterInt(request, "offset", 0);
       int limit = RequestUtils.getParameterInt(request, "limit", 25);

       ContentQuery query = new ContentQuery();
       query.setLimit(limit);
       query.setOffset(offset);

       int total = contentService.getContentCount(query);
       List<Map<String, Object>> list = contentService.getContentList(query);

       ListResponse<Map<String, Object>> listResponse = new ListResponse<>();
       listResponse.setTotal(total);
       listResponse.setOffset(query.getOffset());
       listResponse.setLimit(query.getLimit());
       listResponse.setList(list);

       modelMap.addAttribute("listResponse",listResponse);
       return "content/content_list";
    }

    @RequestMapping(value = "/content/upload", method = RequestMethod.POST)
    public @ResponseBody Object contentUpload(ModelMap modelMap, HttpServletRequest request, @RequestParam("file") MultipartFile file) throws Exception {
        if (file.isEmpty()) {
            request.setAttribute("message", "Please select a file to upload");
            return "uploadStatus";
        }
        try {
            int chunks = RequestUtils.getParameterInt(request, "chunks", 0);
            int chunk = RequestUtils.getParameterInt(request, "chunk", 0);
            int size = RequestUtils.getParameterInt(request, "size", 0);
            String uuid = RequestUtils.getParameter(request, "uuid");
            String videotype = RequestUtils.getParameter(request, "videotype");
            String title = request.getParameter("title");
            String content = request.getParameter("content");
            String orifilename = Normalizer.normalize(file.getOriginalFilename(), Normalizer.Form.NFC); /* mac 한글 */

            if(StringUtils.isEmpty(title)) {
                title = orifilename;
            }

            VideoFile videoFile = new VideoFile();
            videoFile.setTitle(title);
            videoFile.setContent(content);
            videoFile.setFile(file);
            videoFile.setChunks(chunks);
            videoFile.setChunk(chunk);
            videoFile.setUuid(uuid);

            contentService.upload(videoFile);

            BasicResponse response = new BasicResponse();
            response.setResult("SUCCESS");
            response.setData("비디오를 등록했습니다");
            response.setTimestamp(System.currentTimeMillis());
            return new ResponseEntity<>(response, HttpStatus.OK);
        } catch (Exception e) {
            return new ResponseEntity<>(e, HttpStatus.BAD_REQUEST);
        }
    }


    @RequestMapping(value = "/content/retry/{cname}/{idx}", method = RequestMethod.GET)
    public @ResponseBody Object contentRetry(HttpServletRequest request, @PathVariable String cname, @PathVariable String idx) throws Exception {
        if (StringUtils.isEmpty(cname)) {
            throw new Exception("cname 을 입력하세요");
        }
        if (StringUtils.isEmpty(idx)) {
            throw new Exception("idx를 입력하세요");
        }
        try {
            contentService.retry(request, cname, idx);
        } catch (Exception e) {
            throw new Exception(e.getMessage(), e);
        }
        return "{\"success\":true}";
    }

    @RequestMapping(value = "/content/delete/{idx}", method = RequestMethod.GET)
    public @ResponseBody Object contentDelete(HttpServletRequest request, @PathVariable String idx) throws Exception {
        if (StringUtils.isEmpty(idx)) {
            throw new Exception("idx를 입력하세요");
        }
        try {
            contentService.deleteContent(request, idx);
        } catch (Exception e) {
            throw new Exception(e.getMessage(), e);
        }
        return "{\"success\":true}";
    }


}