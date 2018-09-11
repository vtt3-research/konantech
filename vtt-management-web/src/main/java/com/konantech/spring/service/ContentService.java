package com.konantech.spring.service;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.konantech.spring.domain.content.ContentField;
import com.konantech.spring.domain.content.ContentQuery;
import com.konantech.spring.domain.content.VideoFile;
import com.konantech.spring.domain.response.ItemResponse;
import com.konantech.spring.domain.workflow.WorkflowRequest;
import com.konantech.spring.exception.NotFoundException;
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
import org.springframework.stereotype.Repository;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.multipart.MultipartFile;

import javax.servlet.http.HttpServletRequest;
import java.io.*;
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

    @Value("${darc.volumewin}")
    public String volumewin;

    @Value("${darc.volumeetc}")
    public String volumeetc;

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
                for (int pos = 0; pos < jsonArray.size(); pos++) {
                    JSONObject o1 = (JSONObject) jsonArray.get(pos);
                    int width = MapUtils.getIntValue(o1,"width");
                    int height = MapUtils.getIntValue(o1,"height");
                    if(width > 0) {
                        JSONObject tags = (JSONObject) o1.get("tags");
                        String ratate = null;
                        if(tags != null) {
                            ratate = MapUtils.getString(tags, "rotate");
                        }
                        if(ratate != null && Integer.parseInt(ratate) == 90) {
                            item.setWidth(height);
                            item.setHeight(width);
                        } else {
                            item.setWidth(width);
                            item.setHeight(height);
                        }
                        break;
                    }
                }
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
        File uFile = multipartFile(videoFile);
        /* 업로드 마지막일때 */

        if (videoFile.getChunks() == 0 || videoFile.getChunks() == videoFile.getChunk() + 1) {


            SimpleDateFormat yyyy = new SimpleDateFormat("yyyy");
            SimpleDateFormat yyyyMMdd = new SimpleDateFormat("yyyy/MM/dd");

            String title = videoFile.getTitle();
            String content = videoFile.getContent();
            MultipartFile file = videoFile.getFile();

            Map update = new LinkedHashMap<String, Object>();

            Map req = new LinkedHashMap<String, Object>();
            req.put("title", title);
            req.put("content", content);
            this.putContentItem(req);
            int idx = MapUtils.getIntValue(req,"idx");
            String objectid = String.format("OV%s%08d", yyyy.format(new Date()), idx);

            try {
                if(file != null) {
                    String ext = FilenameUtils.getExtension(file.getOriginalFilename());
                    String assetfilepath = yyyyMMdd.format(new Date()) + "/" + idx;
                    String assetfilename = objectid + "." + ext;
                    String filepath = FilenameUtils.normalize(videoFolder + "/" + assetfilepath + "/" + assetfilename);
                    File descFile = new File(filepath);

                    if (descFile.isDirectory()) {
                        throw new NotFoundException("파일명을 확인 하세요 ( " + descFile.getAbsolutePath() + " )");
                    }
                    if (descFile.exists()) {
                        throw new NotFoundException("대상 파일이 존재 합니다 ( " + descFile.getAbsolutePath() + " )");
                    }
                    File dir = descFile.getParentFile();
                    if (!dir.exists()) {
                        if (!dir.mkdirs()) {
                            throw new NotFoundException("Permission denied, ( " + dir.getAbsolutePath() + " )");
                        }
                    }
                    /* file move */
                    FileUtils.moveFile(uFile, descFile);



                    if(StringUtils.isEmpty(videoFile.getOrifilename())) {
                        videoFile.setOrifilename(file.getOriginalFilename());
                    }
                    String mediainfo = fFmpegUtil.getMediaInfo(filepath);
                    update.put("idx", idx);
                    update.put("objectid", objectid);
                    update.put("assetfilepath", assetfilepath);
                    update.put("assetfilename", assetfilename);
                    update.put("assetfilesize", FileUtils.sizeOf(new File(filepath)));
                    update.put("orifilename", videoFile.getOrifilename());
                    update.put("mediainfo", mediainfo);

                } else {

                    update.put("idx", idx);
                    update.put("objectid", objectid);
                    update.put("assetfilepath", videoFile.getFilepath());
                    update.put("genrepath", "/");
                    update.put("orifilename", videoFile.getOrifilename());

                }

                update.put("volumewin", volumewin);
                update.put("volumeetc", volumeetc);

                this.updateContentItem(update);

            } catch (IOException e) {
                throw new Exception(e.getMessage(), e);
            }
        }
        ItemResponse<ContentField> itemResponse = new ItemResponse<>();
        return itemResponse;
    }

    public void deleteContent(HttpServletRequest req, String ids) throws Exception {

        String[] idx_split = StringUtils.split(ids,"|");
        int idx;
        for (String s : idx_split) {
            idx = Integer.parseInt(s);
            contentMapper.deleteMetaInfo(idx);
            contentMapper.deleteSectionInfo(idx);
            contentMapper.deleteRepImg(idx);
            contentMapper.deleteContentItem(idx);
        }
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
            request.setAssetname("vttm_video_tb");
            request.setAssetid(idx);
            request.setSubtype(0);
            List<Integer> workflowIds = workflowService.createWorkflowHis(
                    req.getSession(),
                    request.getWorkflowname(),
                    request.getAssetname(),
                    request.getAssetid(),
                    request.getSubtype());

        } else if (StringUtils.equals(cname, "catalog")) {

            customQueryMapper.selectItem("UPDATE VTTM_VIDEO_TB SET CATALOGSTATUS=5000 WHERE IDX=" + idx);


           customQueryMapper.selectItem("UPDATE VTTM_VIDEO_TB SET CATALOGSTATUS=0 WHERE IDX=" + idx);
//            customQueryMapper.selectItem("DELETE FROM VTTM_COMPJOBQUEUE_TB");
//            customQueryMapper.selectItem("DELETE FROM VTTM_WORKFLOWHIS_TB");

            // 임시1
            WorkflowRequest request = new WorkflowRequest();
            request.setWorkflowname("ca");
            request.setAssetname("VTTM_video_tb");
            request.setAssetid(idx);
            request.setSubtype(0);
            List<Integer> workflowIds = workflowService.createWorkflowHis(
                    req.getSession(),
                    request.getWorkflowname(),
                    request.getAssetname(),
                    request.getAssetid(),
                    request.getSubtype());
//
//            customQueryMapper.selectItem("DELETE FROM VTTM_COMPJOBQUEUE_TB WHERE JOBNAME != 'cataloging'"); //불필요 큐제거
//            customQueryMapper.selectItem("DELETE FROM VTTM_WORKFLOWHIS_TB WHERE CURRJOB != 'cataloging'"); //불필요 워크플로우 제거
//            customQueryMapper.selectItem("UPDATE VTTM_WORKFLOWHIS_TB SET STATUS = 0"); //워크플로우 상태 초기화
//            customQueryMapper.selectItem("UPDATE VTTM_COMPSERVER_TB SET STATUS = 0"); //SET 콤포넌트 상태 초기화
//            customQueryMapper.selectItem("UPDATE VTTM_COMPJOBQUEUE_TB SET STATUS = 0"); //큐 작업 초기화
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

    private File multipartFile(VideoFile uploadFile) throws Exception {
        SimpleDateFormat yyyyMMdd = new SimpleDateFormat("yyyy/MM/dd");
        String baseFolder = FilenameUtils.normalize(videoFolder + File.separator + yyyyMMdd.format(new Date())+ File.separator + uploadFile.getUuid());
        File base = new File(baseFolder);
        File uParent = base.getParentFile();
        if (!uParent.exists()) {
            if (!uParent.mkdirs()) {
                throw new NotFoundException("Permission denied, ( " + uParent.getAbsolutePath() + " )");
            }
        }

        MultipartFile multipartFile = uploadFile.getFile();
        if (multipartFile != null && !multipartFile.isEmpty()) {
            long fileSize = multipartFile.getSize();
            if (fileSize > 0) {
                InputStream inputStream = null;
                OutputStream outputStream = null;
                try {
                    inputStream = multipartFile.getInputStream();
                    outputStream = new FileOutputStream(base, (uploadFile.getChunk() == 0) ? false : true);
                    byte[] buffer = new byte[8192];
                    int readBytes;
                    while ((readBytes = inputStream.read(buffer, 0, 8192)) != -1) {
                        outputStream.write(buffer, 0, readBytes);
                    }
                } catch (Exception ex) {
                    throw ex;
                } finally {
                    if (outputStream != null) {
                        outputStream.close();
                    }
                    if (inputStream != null) {
                        inputStream.close();
                    }
                }
            }
        }

        return base;
    }





}