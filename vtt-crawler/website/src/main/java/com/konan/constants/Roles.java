package com.konan.constants;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.context.SecurityContextHolder;

/**
 * 		사이트에서 사용될 Role name을 Key map으로 정의하여 사용한다.
 * 			- 변경 또는 추가될 경우 여기만 변경 하면 사이트 전체 적용
 */
public final class Roles {

	public static final String USER				= "ROLE_USER";
	public static final String ADMIN			= "ROLE_ADMIN";
		

	@SuppressWarnings("unchecked")
	public static boolean isSessionHasRole(String role){
		List<GrantedAuthority> sessionRoles = (List<GrantedAuthority>)SecurityContextHolder.getContext().getAuthentication().getAuthorities();
		return sessionRoles.contains(new SimpleGrantedAuthority(role));
	}
	
	@SuppressWarnings("unchecked")
	public static boolean isSessionHasAnyRole(String... roles){
		List<GrantedAuthority> sessionRoles = (List<GrantedAuthority>)SecurityContextHolder.getContext().getAuthentication().getAuthorities();
		for(String role : roles){
			if(sessionRoles.contains(new SimpleGrantedAuthority(role))) return true;
		}
		return false;
	}
	
	@SuppressWarnings("rawtypes")
	public static List<GrantedAuthority> convertUserAuth(List<HashMap<String, Object>> authInfoList){
		List<GrantedAuthority> roles = new ArrayList<GrantedAuthority>();
		
		for(HashMap<String, Object> authInfo : authInfoList){
			String VIEW_NAME = authInfo.get("VIEW_URL").toString();
			
			Set set = authInfo.keySet();
			Iterator iterator = set.iterator();
			while(iterator.hasNext()){
				String key = (String)iterator.next();
				if(key.contains("_YN")){
					boolean isHasAuth = Boolean.parseBoolean(authInfo.get(key).toString());
					if(isHasAuth){
						roles.add(new SimpleGrantedAuthority("ROLE_AUTH_"+VIEW_NAME.toUpperCase()+"_"+key.replace("_YN", "").toUpperCase()));
					}
				}
			}
		}
		
		return roles;
	}
}
