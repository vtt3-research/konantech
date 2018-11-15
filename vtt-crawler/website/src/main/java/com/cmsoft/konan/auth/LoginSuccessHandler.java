package com.cmsoft.konan.auth;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.springframework.security.core.Authentication;
import org.springframework.security.web.authentication.SavedRequestAwareAuthenticationSuccessHandler;
import org.springframework.stereotype.Component;

import com.cmsoft.konan.constants.Roles;

@Component
public class LoginSuccessHandler extends SavedRequestAwareAuthenticationSuccessHandler {
	
	public LoginSuccessHandler() {		
		setDefaultTargetUrl("/user/collectionList");
	}
	
	@Override
	public void onAuthenticationSuccess(HttpServletRequest request, HttpServletResponse response, Authentication authentication) throws ServletException, IOException {
		
		HttpSession session = request.getSession(false);
		
		if(session == null){
			response.sendRedirect("/home/login");
		}else{
			String targetURL = getDefaultTargetUrl();
			if(Roles.isSessionHasAnyRole(Roles.ADMIN)){									
				targetURL = "/admin/collectionList";				
			}

			String LOGIN_ID = (String) authentication.getPrincipal();
			session.setAttribute("USER_ID", LOGIN_ID);
			
			getRedirectStrategy().sendRedirect(request, response, targetURL);
			return;
		}
		
	}

}
