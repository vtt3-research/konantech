package com.konan.controller;


import java.io.File;

import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContextAware;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.servlet.ModelAndView;

@Controller
public class DownloadController implements ApplicationContextAware{
 
    @RequestMapping("download")
    public ModelAndView download(@RequestParam("fileName")String fileName){
         
        String fullPath = "/usr/share/tomcat/webapps/crawler/" + fileName;
         
        File file = new File(fullPath);
         
        return new ModelAndView("downloadView", "downloadFile", file);
    }

	@Override
	public void setApplicationContext(org.springframework.context.ApplicationContext applicationContext)
			throws BeansException {
		// TODO Auto-generated method stub
		
	}
     
}