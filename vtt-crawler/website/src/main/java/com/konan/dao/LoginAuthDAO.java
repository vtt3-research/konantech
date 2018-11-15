package com.konan.dao;

import org.apache.ibatis.annotations.Param;
import org.springframework.stereotype.Repository;

@Repository
public interface LoginAuthDAO {

	public String selectLoginType(@Param("LOGIN_ID") String LOGIN_ID, @Param("LOGIN_PW") String LOGIN_PW);
	
}
