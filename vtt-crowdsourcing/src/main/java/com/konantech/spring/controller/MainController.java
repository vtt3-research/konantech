package com.konantech.spring.controller;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

import javax.servlet.http.HttpServletRequest;

@Controller
public class MainController {

    @RequestMapping(value = "/main", method = RequestMethod.GET)
    public String defaultActon(ModelMap modelMap, HttpServletRequest request) {
        String type = request.getParameter("type");

        modelMap.addAttribute("type", type);
        return "main/main";
    }


}


