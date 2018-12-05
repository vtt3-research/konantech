package com.konan.auth;

import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.authentication.AuthenticationProvider;
import org.springframework.security.authentication.BadCredentialsException;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.stereotype.Component;

import com.konan.constants.PropertiesConstants;
import com.konan.constants.Roles;
import com.konan.service.LoginAuthService;
import com.konan.utility.CommonUtil;
import com.konan.utility.MessageUtil;

@Component
public class CustomAuthenticationProvider implements AuthenticationProvider {

	@Autowired private LoginAuthService loginAuthService;
	
	@Override
	public Authentication authenticate(Authentication authentication) throws AuthenticationException {
		
		//throw new BadCredentialsException(MessageUtil.getValue(PropertiesConstants.MESSAGE_LOGIN_ERROR_CHECKIDPASSWORD));
		
		String LOGIN_ID = (String) authentication.getPrincipal();
		String LOGIN_PW = (String) authentication.getCredentials();
		
		if (!CommonUtil.isEmpty(LOGIN_ID) && !CommonUtil.isEmpty(LOGIN_PW)) {
			String LOGIN_TYPE = loginAuthService.selectLoginType(LOGIN_ID, LOGIN_PW);
			if(!CommonUtil.isEmpty(LOGIN_TYPE)){
				List<GrantedAuthority> roles = new ArrayList<GrantedAuthority>();
				if(LOGIN_TYPE.equals("Y")){
					roles.add(new SimpleGrantedAuthority(Roles.ADMIN));
				}else {
					roles.add(new SimpleGrantedAuthority(Roles.USER));
				}
				
				UsernamePasswordAuthenticationToken result = new UsernamePasswordAuthenticationToken(LOGIN_ID, LOGIN_PW, roles);			
					
				
				return result;
			}else{
				throw new BadCredentialsException(MessageUtil.getValue(PropertiesConstants.MESSAGE_LOGIN_ERROR_CHECKIDPASSWORD));
			}
		}else{
			throw new BadCredentialsException(MessageUtil.getValue(PropertiesConstants.MESSAGE_LOGIN_ERROR_INPUTIDPASSWORD));			
		}		
				
	}

	@Override
	public boolean supports(Class<?> authentication) {
		return authentication.equals(UsernamePasswordAuthenticationToken.class);
	}
	

}
