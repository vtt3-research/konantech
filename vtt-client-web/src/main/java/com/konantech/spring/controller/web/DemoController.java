package com.konantech.spring.controller.web;

import com.konantech.spring.exception.ApiException;
import io.swagger.annotations.ApiImplicitParam;
import io.swagger.annotations.ApiImplicitParams;
import io.swagger.annotations.ApiOperation;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.Map;

@Controller
public class DemoController {

    @RequestMapping(value = "/demo", method = RequestMethod.GET)
    public String custom() {
        return "demo/demo_page";
    }


}