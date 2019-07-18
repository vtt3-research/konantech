package com.konantech.spring.controller.web;

import com.konantech.spring.service.*;
import com.konantech.spring.util.RequestUtils;
import org.apache.commons.collections.MapUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.w3c.dom.Attr;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import java.io.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

@Controller
public class FileController {

    @Autowired
    VisualService visualService;

    @Autowired
    SectionService sectionService;

    @Autowired
    ContentService contentService;

    @Autowired
    SoundService soundService;

    @Autowired
    FileService fileService;

    @RequestMapping(value = "/xmlFileDown", method = RequestMethod.GET)
    public void xmlFileDown(@RequestParam(value = "fileId", defaultValue = "file.xml") String fileId, HttpServletRequest request, HttpServletResponse response) throws Exception {
        request.setCharacterEncoding("UTF-8");

        //파일 업로드된 경로
        try {
            DocumentBuilderFactory docFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder docBuilder = docFactory.newDocumentBuilder();

            // 루트 엘리먼트
            Document doc = docBuilder.newDocument();
            Element rootElement = doc.createElement("company");
            doc.appendChild(rootElement);

            // staff 엘리먼트
            Element staff = doc.createElement("Staff");
            rootElement.appendChild(staff);

            // 속성값 정의
            Attr attr = doc.createAttribute("id");
            attr.setValue("1");
            staff.setAttributeNode(attr);

            // 속성값을 정의하는 더 쉬운 방법
            // staff.setAttribute("id", "1");

            // firstname 엘리먼트
            Element firstname = doc.createElement("firstname");
            firstname.appendChild(doc.createTextNode("Gildong"));
            staff.appendChild(firstname);

            // lastname 엘리먼트
            Element lastname = doc.createElement("lastname");
            lastname.appendChild(doc.createTextNode("Hong"));
            staff.appendChild(lastname);

            // nickname 엘리먼트
            Element nickname = doc.createElement("nickname");
            nickname.appendChild(doc.createTextNode("Mr.Hong"));
            staff.appendChild(nickname);

            // salary 엘리먼트
            Element salary = doc.createElement("salary");
            salary.appendChild(doc.createTextNode("100000"));
            staff.appendChild(salary);

            // XML 파일로 쓰기
            TransformerFactory transformerFactory = TransformerFactory.newInstance();
            Transformer transformer = transformerFactory.newTransformer();

            transformer.setOutputProperty(OutputKeys.ENCODING, "UTF - 8");
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");
            DOMSource source = new DOMSource(doc);

            String fileName = "file.xml";
            // 파일 객체 생성

            //실제 내보낼 파일명
            String oriFileName = "file.xml";


            File file = new File(fileName);
            // true 지정시 파일의 기존 내용에 이어서 작성
            FileWriter fw = new FileWriter(file, true);

            // 파일안에 문자열 쓰기
            //fw.write(txt);
            fw.flush();

            // 객체 닫기
            fw.close();


            //실제 내보낼 파일명
            InputStream in = null;
            OutputStream os = null;
            boolean skip = false;
            String client = "";

            //파일을 읽어 스트림에 담기
            try {
                in = new FileInputStream(file);
            } catch (FileNotFoundException fe) {
                skip = true;
            }

            client = request.getHeader("User-Agent");

            //파일 다운로드 헤더 지정
            response.reset();
            response.setContentType("application/octet-stream");
            response.setHeader("Content-Description", "JSP Generated Data");

            if (!skip) {
                // IE
                if (client.indexOf("MSIE") != -1) {
                    response.setHeader("Content-Disposition", "attachment; filename=\""
                            + java.net.URLEncoder.encode(oriFileName, "UTF-8").replaceAll("\\+", "\\ ") + "\"");
                    // IE 11 이상.
                } else if (client.indexOf("Trident") != -1) {
                    response.setHeader("Content-Disposition", "attachment; filename=\""
                            + java.net.URLEncoder.encode(oriFileName, "UTF-8").replaceAll("\\+", "\\ ") + "\"");
                } else {
                    // 한글 파일명 처리
                    response.setHeader("Content-Disposition",
                            "attachment; filename=\"" + new String(oriFileName.getBytes("UTF-8"), "ISO8859_1") + "\"");
                    response.setHeader("Content-Type", "application/octet-stream; charset=utf-8");
                }
                response.setHeader("Content-Length", "" + file.length());
                os = response.getOutputStream();
                byte b[] = new byte[(int) file.length()];
                int leng = 0;
                while ((leng = in.read(b)) > 0) {
                    os.write(b, 0, leng);
                }
            } else {
                response.setContentType("text/html;charset=UTF-8");
                System.out.println("<script language='javascript'>alert('파일을 찾을 수 없습니다');history.back();</script>");
            }
            in.close();
            os.close();
        } catch (Exception e) {
            System.out.println("ERROR : " + e.getMessage());
        }
    }

//    @ResponseBody
//    @RequestMapping(value = "/jsonFileDown", method = RequestMethod.GET)
//    public void jsonFileDown(HttpServletRequest request, HttpServletResponse response) throws Exception {
//        Map paramMap = RequestUtils.getParameterMap(request);
//        String visualJson = visualService.getJsonData(paramMap);
//        String qaJson = sectionService.getJsonData(paramMap);
//        String soundJson = soundService.getJsonData(paramMap);
//        List<Map> fileList = new ArrayList<>();
//        fileList.add(fileService.createJsonFileStream(paramMap,"visual",visualJson));
//        fileList.add(fileService.createJsonFileStream(paramMap,"qa",qaJson));
//        fileList.add(fileService.createJsonFileStream(paramMap,"sound",soundJson));
//
//        fileService.downloadZip(response,"json", fileList);
//    }

    @ResponseBody
    @RequestMapping(value = "/jsonFileDown", method = RequestMethod.GET)
    public void jsonFileDown(HttpServletRequest request, HttpServletResponse response) throws Exception {
        Map paramMap = RequestUtils.getParameterMap(request);

        List<Map> UserList = contentService.getFileUserList(paramMap);
        List<Map> fileList = new ArrayList<>();
        int visualDataCnt = 0;

        for(Map userData : UserList){

            paramMap.replace("userid", userData.get("user_id"));
            if(userData.get("info_type").equals("visual")){

                String visualJson = visualService.getJsonData(paramMap);
                fileList.add(fileService.createJsonFileStream(paramMap,"visual",visualJson));

                visualDataCnt++;

            }else if(userData.get("info_type").equals("sound")){

                String soundJson = soundService.getJsonData(paramMap);
                fileList.add(fileService.createJsonFileStream(paramMap,"sound",soundJson));

            }else if(userData.get("info_type").equals("section")){

                String qaJson = sectionService.getJsonData(paramMap);
                fileList.add(fileService.createJsonFileStream(paramMap,"qa",qaJson));
            }
        }

        String visualInfoJson = visualService.getShotJsonData(RequestUtils.getParameterMap(request));
        fileList.add(fileService.createJsonFileStream(RequestUtils.getParameterMap(request),"shotinfo",visualInfoJson));

        fileService.downloadZip(response,"json", fileList);
    }

    @ResponseBody
    @RequestMapping(value = "/imgFileDown", method = RequestMethod.GET)
    public void imgFileDown(HttpServletRequest request, HttpServletResponse response) throws Exception {
        Map paramMap = RequestUtils.getParameterMap(request);
        List<Map<String,Object>> list = contentService.getRepImgFile(request);
        List<Map> fileList = new ArrayList<>();
        for(Map map : list) {
            fileList.add(fileService.createImgFileStream(MapUtils.getString(map,"path")));
        }
        fileService.downloadZip(response,"img", fileList);
    }

}