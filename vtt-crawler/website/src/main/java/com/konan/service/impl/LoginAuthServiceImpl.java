package com.konan.service.impl;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.konan.dao.LoginAuthDAO;
import com.konan.service.LoginAuthService;

@Service
public class LoginAuthServiceImpl implements LoginAuthService {

	@Autowired private LoginAuthDAO loginAuthDAO;

	@Override
	public String selectLoginType(String LOGIN_ID, String LOGIN_PW) {
		return loginAuthDAO.selectLoginType(LOGIN_ID, LOGIN_PW);
	}

}
