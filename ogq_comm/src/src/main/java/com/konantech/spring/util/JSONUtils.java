package com.konantech.spring.util;

import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonSerializer;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.SerializerProvider;
import com.fasterxml.jackson.databind.module.SimpleModule;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class JSONUtils {

    public static String jsonStringFromObject(Object object) throws JsonProcessingException {
        ObjectMapper mapper = new ObjectMapper();
        return mapper.writeValueAsString(object);
    }

    public static String jsonStringFromObjectLowerCase(Object object) throws JsonProcessingException {
        ObjectMapper mapper = new ObjectMapper();

        SimpleModule simpleModule = new SimpleModule();
        simpleModule.addKeySerializer(String.class, new LowerCaseKeySerializer());
        mapper.registerModule(simpleModule);

        return mapper.writeValueAsString(object);
    }

    public static Map jsonStringToMap(String t) throws IOException {
        ObjectMapper mapper = new ObjectMapper();
        return mapper.readValue(t,HashMap.class);
    }

    public static Object jsonStringToObject(String t, Class<?> c) throws IOException {
        ObjectMapper mapper = new ObjectMapper();
        return mapper.readValue(t, c );
    }

    public static class LowerCaseKeySerializer extends JsonSerializer<String> {

        @Override
        public void serialize(String value, JsonGenerator gen, SerializerProvider serializers)
                throws IOException {

            String key = value.toLowerCase();
            gen.writeFieldName(key);
        }
    }
}
