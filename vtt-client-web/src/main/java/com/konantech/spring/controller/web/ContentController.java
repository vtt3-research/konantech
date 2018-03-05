package com.konantech.spring.controller.web;

import com.konantech.spring.domain.content.ContentQuery;
import com.konantech.spring.domain.response.ListResponse;
import com.konantech.spring.service.ContentService;
import com.konantech.spring.util.RequestUtils;
import net.sf.json.JSONObject;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.lang3.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
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
import java.text.SimpleDateFormat;
import java.util.*;

@Controller
public class ContentController {

    @Autowired
    private ContentService contentService;

    @RequestMapping(value = "/content/write", method = RequestMethod.GET)
    public String contentWrite(ModelMap modelMap, HttpServletRequest request) throws Exception {
        return "content/content_write";
    }

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
            contentService.upload(request, file);
        } catch (Exception e) {
            throw new Exception(e.getMessage(), e);
        }
        return "{\"success\":true}";
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
            contentService.retry(request, cname,idx);
        } catch (Exception e) {
            throw new Exception(e.getMessage(), e);
        }
        return "{\"success\":true}";
    }


}