package com.konantech.spring.service;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.redis.core.RedisTemplate;
import org.springframework.stereotype.Component;

import java.util.concurrent.TimeUnit;

@Component
public class RedisService {

    @Autowired
    private RedisTemplate redisTemplate;

    private static Logger log = LoggerFactory.getLogger(RedisService.class);


    public boolean setObject(String key, Object value) {
        return setObject(key,value,-1);
    }

    public boolean setObject(String key, Object value, int second) {
        try {
            redisTemplate.opsForValue().set(key, value);
            if(second > 0) {
                redisTemplate.expire(key, second, TimeUnit.SECONDS);
            }
            log.debug("SET expire:" + redisTemplate.getExpire(key) + " ," + key + " ," + value);
            return true;
        } catch (Exception e) {
            log.error(e.getMessage(), e);
            return false;
        }
    }

    public Object getObject(String key) {
        try {
            Object value = redisTemplate.opsForValue().get(key);
            log.debug("GET expire:" + redisTemplate.getExpire(key) + " ," + key + " ," + value);
            return value;
        } catch (Exception e) {
            log.error(e.getMessage(), e);
            return null;
        }
    }

    public boolean delString(String key) {
        try {
            redisTemplate.delete(key);
            System.out.println("GET delete key = " + key );
            return true;
        } catch (Exception e) {
            log.error(e.getMessage(), e);
            return false;
        }
    }
}