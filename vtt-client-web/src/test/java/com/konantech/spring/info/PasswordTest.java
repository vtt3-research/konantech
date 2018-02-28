package com.konantech.spring.info;

import com.konantech.spring.security.SHAPasswordEncoder;
import org.junit.Test;
import org.springframework.security.authentication.encoding.MessageDigestPasswordEncoder;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.security.crypto.password.StandardPasswordEncoder;

public class PasswordTest {

    private MessageDigestPasswordEncoder sha256PasswordEncoder;



    @Test
    public void passwordTest() {
        String str = "admin";
        PasswordEncoder passwordEncoder1 = new BCryptPasswordEncoder();
        PasswordEncoder passwordEncoder2 = new StandardPasswordEncoder();
        SHAPasswordEncoder passwordEncoder3 = new SHAPasswordEncoder(512);

        System.out.println(passwordEncoder1.encode(str));
        System.out.println(passwordEncoder2.encode(str));
        System.out.println(passwordEncoder3.encode(str));
    }
}
