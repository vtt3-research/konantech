package com.konan.auth;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.method.configuration.EnableGlobalMethodSecurity;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.builders.WebSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;

@Configuration
@EnableWebSecurity
@EnableGlobalMethodSecurity(securedEnabled = true, prePostEnabled = true)
public class SecurityConfiguration extends WebSecurityConfigurerAdapter {
	
	@Autowired
	private CustomAuthenticationProvider customAuthenticationProvider;
	
	@Autowired
	private LoginSuccessHandler loginSuccessHandler;
	
	@Autowired
	public void configureGlobal(AuthenticationManagerBuilder auth) {
		auth.authenticationProvider(customAuthenticationProvider);
	}
	
	@Bean(name="authenticationManager")
	public AuthenticationManager authenticationManagerBean() throws Exception {
		return super.authenticationManagerBean();
	}	
		
	@Override
	public void configure(WebSecurity web) throws Exception {
		web.ignoring().antMatchers("/resources/**/*");
		web.ignoring().antMatchers("/**/*.html");
	}
	
	@Override
	protected void configure(HttpSecurity http) throws Exception {
		http.headers().frameOptions().disable();
		
		http.csrf().disable()
			.formLogin()
				.loginPage("/home/login")
				.loginProcessingUrl("/home/loginProcess")
				.usernameParameter("userId")
				.passwordParameter("userPassword")
				.successHandler(loginSuccessHandler)
				.permitAll()
			.and()
			.logout()
				.logoutUrl("/logout")
				.logoutSuccessUrl("/home/login")
				.invalidateHttpSession(true)
				.deleteCookies("JSESSIONID","SPRING_SECURITY_REMEMBER_ME_COOKIE")
			.and()
			.authorizeRequests()
				.antMatchers("/", "/home/**").permitAll()
				.antMatchers("/user/**").access("hasAnyRole('USER')")
				.antMatchers("/admin/**").access("hasAnyRole('ADMIN')")
				.anyRequest().authenticated()
			.and()			
			.httpBasic()
		;
	}	
}
