package com.konantech.spring.controller.rest;


import com.konantech.spring.exception.FieldException;
import com.konantech.spring.response.AuthenticationResponse;
import com.konantech.spring.security.AuthenticationRequest;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.ResponseEntity;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import javax.servlet.http.HttpServletRequest;
import javax.validation.Valid;

@RestController
public class AuthController {

    private static Logger log = LoggerFactory.getLogger(AuthController.class);

    @RequestMapping(value = "/v2/login", method = RequestMethod.POST)
    public ResponseEntity<?> authLogin(@Valid @RequestBody AuthenticationRequest authenticationRequest, BindingResult result, HttpServletRequest request ) throws Exception {

        if (result.hasErrors()) {
            throw new FieldException(result);
        }

        String token = "__token__";
        return ResponseEntity.ok(new AuthenticationResponse(token));
    }

    @RequestMapping(value = "/v2/login/refresh", method = RequestMethod.GET)
    public ResponseEntity<?> authRefresh(HttpServletRequest request) {
        String token = "__token__";
        return ResponseEntity.ok(new AuthenticationResponse(token));
    }

}
