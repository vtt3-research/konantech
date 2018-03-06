package com.konantech.spring.mapper;

import org.apache.ibatis.annotations.Mapper;
import org.springframework.security.core.GrantedAuthority;

import java.util.Collection;
import java.util.List;
import java.util.Map;


@Mapper
public interface AuthMapper {

    Map<String, Object> readUserByName(String username);

    Collection<GrantedAuthority> readAuthority(String username);

    int insertCurrentUser(Map<String, Object> data);

    List<Map<String, Object>> getUserList();

}
