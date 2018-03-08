package com.konantech.spring.service;

import com.konantech.spring.CommonTests;
import org.junit.Test;
import org.springframework.beans.factory.annotation.Autowired;


public class AuthServiceTest extends CommonTests {

    @Autowired
    AuthService authService;

    @Autowired
    MetaService metaService;

    @Test
    public void setCacheAll() throws Exception {
        System.out.println(authService);
        System.out.println(metaService);

        System.out.println(metaService.getMetaList("konan"));
//        authService.getAuthorities("konan");
    }

}