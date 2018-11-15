package com.konan.controller;


import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;

@Controller
@RequestMapping("/home")
public class HomeController {

	@RequestMapping(value = "/login", method = RequestMethod.GET)
	public String login(
			HttpServletRequest request,
			HttpServletResponse response,			
			Model model) throws Exception {
		
		return "/home/login";
	}	
}
