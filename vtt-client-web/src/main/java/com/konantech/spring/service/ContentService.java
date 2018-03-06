package com.konantech.spring.service;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.konantech.spring.domain.content.ContentField;
import com.konantech.spring.domain.content.ContentQuery;
import com.konantech.spring.domain.content.VideoFile;
import com.konantech.spring.domain.response.ItemResponse;
import com.konantech.spring.domain.workflow.WorkflowRequest;
import com.konantech.spring.mapper.ContentMapper;
import com.konantech.spring.mapper.CustomQueryMapper;
import com.konantech.spring.mapper.WorkflowMapper;
import com.konantech.spring.util.FFmpegUtil;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.lang3.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.multipart.MultipartFile;

import javax.servlet.http.HttpServletRequest;
import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;
import java.util.*;

@Service
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
public class ContentService {

    @Autowired
    private ContentMapper contentMapper;

    @Autowired
    private CustomQueryMapper customQueryMapper;

    @Autowired
    private WorkflowService workflowService;

    @Autowired
    WorkflowMapper workflowMapper;

    @Autowired
    private FFmpegUtil fFmpegUtil;

    @Value("${cmd.ffprobe}")
    private String ffprobeCmd;

    @Value("${darc.videoFolder}")
    public String videoFolder;

    public int getContentCount(ContentQuery param) {
        return contentMapper.getContentCount(param);
    }

    public List<Map<String,Object>> getContentList(ContentQuery n) {
        return contentMapper.getContentList(n);
    }

    public ContentField getContentItem(ContentQuery n) throws Exception {

        ContentField item = contentMapper.getContentItem(n);
        String mediainfo = item.getMediainfo();
        if(mediainfo != null) {
            ObjectMapper mapper = new ObjectMapper();
            HashMap<String, Object> map = mapper.readValue(mediainfo, new HashMap<String, Object>().getClass());
            JSONArray jsonArray = JSONArray.fromObject(map.get("streams"));
            if (jsonArray != null && jsonArray.size() > 0) {
                JSONObject o1 = (JSONObject) jsonArray.get(0);
                int width = (int) o1.get("width");
                int height = (int) o1.get("height");
                item.setWidth(width);
                item.setHeight(height);
            }
        }
        return item;
    }

    public int putContentItem(Map<String, Object> request) {
        return contentMapper.putContentItem(request);
    }
    public int updateContentItem(Map<String, Object> request) {
        return contentMapper.updateContentItem(request);
    }


    public ItemResponse<ContentField> upload(VideoFile videoFile) throws Exception {

        SimpleDateFormat yyyy = new SimpleDateFormat("yyyy");
        SimpleDateFormat yyyyMMdd = new SimpleDateFormat("yyyy/MM/dd");
        String descFolder = videoFolder;
        String baseFolder = FilenameUtils.normalize(descFolder + "/" + yyyyMMdd.format(new Date()));
        File base = new File(baseFolder);
        if (!base.exists()) {
            if(!base.mkdirs()) {
                throw new Exception("Permission denied, ( " + base.getAbsolutePath() + " )" );
            }
        }
        String title = videoFile.getTitle();
        String content = videoFile.getContent();
        MultipartFile file = videoFile.getFile();

        Map req = new LinkedHashMap<String, Object>();
        req.put("title", title);
        req.put("content", content);
        this.putContentItem(req);

        Map update = new LinkedHashMap<String, Object>();

        int idx = MapUtils.getIntValue(req,"idx");
        String objectid = String.format("OV%s%08d", yyyy.format(new Date()), idx);

        try {
            if(file != null) {
                String ext = FilenameUtils.getExtension(file.getOriginalFilename());
                String assetfilepath = yyyyMMdd.format(new Date()) + "/" + idx;
                String assetfilename = objectid + "." + ext;

                String descFile = FilenameUtils.normalize(descFolder + "/" + assetfilepath + "/" + assetfilename);
                File parent = new File(descFile).getParentFile();
                if (!parent.exists()) {
                    if(!parent.mkdirs()) {
                        throw new Exception("Permission denied, ( " + parent.getAbsolutePath() + " )" );
                    }
                }

                byte[] bytes = file.getBytes();
                Path path = Paths.get(descFile);
                Files.write(path, bytes);

                if(StringUtils.isEmpty(videoFile.getOrifilename())) {
                    videoFile.setOrifilename(file.getOriginalFilename());
                }
                String mediainfo = fFmpegUtil.getMediaInfo(descFile);
                update.put("idx", idx);
                update.put("objectid", objectid);
                update.put("assetfilepath", assetfilepath);
                update.put("assetfilename", assetfilename);
                update.put("assetfilesize", FileUtils.sizeOf(new File(descFile)));
                update.put("orifilename", videoFile.getOrifilename());
                update.put("mediainfo", mediainfo);

            } else {

                update.put("idx", idx);
                update.put("objectid", objectid);
                update.put("assetfilepath", videoFile.getFilepath());
                update.put("genrepath", "/");
                update.put("orifilename", videoFile.getOrifilename());

            }
            this.updateContentItem(update);

        } catch (IOException e) {
            throw new Exception(e.getMessage(), e);
        }

        ContentQuery query = new ContentQuery();
        query.setIdx(idx);
        ItemResponse<ContentField> itemResponse = new ItemResponse<>();
        itemResponse.setItem(this.getContentItem(query));
        return itemResponse;
    }

    public void retry(HttpServletRequest req, String cname,String ids) throws Exception {

        String[] idx_split = StringUtils.split(ids,"|");
        if(StringUtils.equals(cname,"catalog") || StringUtils.equals(cname,"transcoding")) {
            int idx;
            for (String s : idx_split) {
                idx = Integer.parseInt(s);
                queryResetTemp1(req, cname, idx);
            }

        } else {
            throw new Exception("catalog 또는 transcoding 이 아닙니다");
        }
    }


    /* 임시 */
    public void queryResetTemp1(HttpServletRequest req, String cname, int idx) throws Exception {
        if (StringUtils.equals(cname, "transcoding")) {
            customQueryMapper.selectItem("UPDATE DEMO_VIDEO_TB SET TRANSCODINGSTATUS=5000 WHERE IDX=" + idx);
//            customQueryMapper.selectItem("UPDATE DEMO_VIDEO_TB SET TRANSCODINGSTATUS=0 WHERE IDX=" + idx);
//            customQueryMapper.selectItem("DELETE FROM DEMO_COMPJOBQUEUE_TB");
//            customQueryMapper.selectItem("DELETE FROM DEMO_WORKFLOWHIS_TB");

            // 임시1
            WorkflowRequest request = new WorkflowRequest();
            request.setWorkflowname("tc_video");
            request.setAssetname("demo_video_tb");
            request.setAssetid(idx);
            request.setSubtype(0);
            List<Integer> workflowIds = workflowService.createWorkflowHis(
                    req.getSession(),
                    request.getWorkflowname(),
                    request.getAssetname(),
                    request.getAssetid(),
                    request.getSubtype());

        } else if (StringUtils.equals(cname, "catalog")) {

            customQueryMapper.selectItem("UPDATE DEMO_VIDEO_TB SET CATALOGSTATUS=5000 WHERE IDX=" + idx);


//            customQueryMapper.selectItem("UPDATE DEMO_VIDEO_TB SET CATALOGSTATUS=0 WHERE IDX=" + idx);
//            customQueryMapper.selectItem("DELETE FROM DEMO_COMPJOBQUEUE_TB");
//            customQueryMapper.selectItem("DELETE FROM DEMO_WORKFLOWHIS_TB");

            // 임시1
            WorkflowRequest request = new WorkflowRequest();
            request.setWorkflowname("ca");
            request.setAssetname("demo_video_tb");
            request.setAssetid(idx);
            request.setSubtype(0);
            List<Integer> workflowIds = workflowService.createWorkflowHis(
                    req.getSession(),
                    request.getWorkflowname(),
                    request.getAssetname(),
                    request.getAssetid(),
                    request.getSubtype());
//
//            customQueryMapper.selectItem("DELETE FROM DEMO_COMPJOBQUEUE_TB WHERE JOBNAME != 'cataloging'"); //불필요 큐제거
//            customQueryMapper.selectItem("DELETE FROM DEMO_WORKFLOWHIS_TB WHERE CURRJOB != 'cataloging'"); //불필요 워크플로우 제거
//            customQueryMapper.selectItem("UPDATE DEMO_WORKFLOWHIS_TB SET STATUS = 0"); //워크플로우 상태 초기화
//            customQueryMapper.selectItem("UPDATE DEMO_COMPSERVER_TB SET STATUS = 0"); //SET 콤포넌트 상태 초기화
//            customQueryMapper.selectItem("UPDATE DEMO_COMPJOBQUEUE_TB SET STATUS = 0"); //큐 작업 초기화
//
//            // ToDo 서버간 동기화 구간 시작
//            CompJobQueueTB compJobQueueTB = workflowMapper.selectAllocJob();
//            CompServerTB compServerTB = null;
//            if (compJobQueueTB != null) {
//                compServerTB = workflowMapper.selectAllocServer(compJobQueueTB);
//            }
//            if (compJobQueueTB != null && compServerTB != null) {
//                workflowService.updateStatusProc(compJobQueueTB, compServerTB);
//            }
//            workflowService.allocJob(compJobQueueTB, compServerTB);
        }



    }





}