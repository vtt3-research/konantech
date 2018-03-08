package com.konantech.spring.service;

import com.konantech.spring.domain.user.SecurityUser;
import com.konantech.spring.mapper.AuthMapper;
import com.konantech.spring.security.SHAPasswordEncoder;
import org.apache.commons.collections.MapUtils;
import org.apache.commons.collections.map.HashedMap;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import java.util.Collection;
import java.util.List;
import java.util.Map;

/**
 * Created by Violet on 2017-01-17.
 */

@Service
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
public class AuthService implements UserDetailsService {

    @Autowired
    private AuthMapper authMapper;

    private SHAPasswordEncoder passwordEncoder = new SHAPasswordEncoder(512);

    @Override
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {

        Map<String, Object> user = authMapper.readUserByName(username);
        SecurityUser securityUser = new SecurityUser(
                MapUtils.getString(user,"username"),
                MapUtils.getString(user,"password"),
                getAuthorities(username));
        return securityUser;
    }

    public Collection<GrantedAuthority> getAuthorities(String username) {
        return authMapper.readAuthority(username);
    }

    public PasswordEncoder passwordEncoder() {
        return this.passwordEncoder;
    }

    public Map<String, Object> currentUserInsert(String username, String userip ) {
        Map<String, Object> userInfo = authMapper.readUserByName(username);
        Map<String, Object> current = new HashedMap();
        current.put("userid", userInfo.get("userid"));
        current.put("userip", userip);
        if(authMapper.insertCurrentUser(current) > 0) {
            int agentid = MapUtils.getIntValue(current, "agentid");
            userInfo.put("agentid", agentid);
        }
        return userInfo;
    }


    public List<Map<String, Object>> getUserList() {
        return authMapper.getUserList();
    }
}
