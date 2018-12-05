package com.konantech.spring.config;

import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import springfox.documentation.builders.PathSelectors;
import springfox.documentation.builders.RequestHandlerSelectors;
import springfox.documentation.service.ApiInfo;
import springfox.documentation.service.ApiKey;
import springfox.documentation.service.Contact;
import springfox.documentation.spi.DocumentationType;
import springfox.documentation.spring.web.plugins.Docket;
import springfox.documentation.swagger2.annotations.EnableSwagger2;

import static com.google.common.collect.Lists.newArrayList;

@Configuration
@EnableSwagger2
public class SwaggerConfig {
    @Bean
    public Docket api() {
        return new Docket(DocumentationType.SWAGGER_2)
                .select()
                .apis(RequestHandlerSelectors.any())
                .apis(RequestHandlerSelectors.basePackage("com.konantech.spring.controller.rest"))
                .paths(PathSelectors.any())
                .build()
                .apiInfo(apiInfo())
                .securitySchemes(newArrayList(authorization()));

    }

    private ApiKey authorization() {
        return new ApiKey("x-auth-token", "api-key", "header");
    }

    private ApiInfo apiInfo() {

        Contact DEFAULT_CONTACT = new Contact("", "", "");
        return  new ApiInfo("OGQ & KONAN 비디오 태킹 인터페이스", "Api 에 대한 상세 페이지 입니다.", "1.0", "urn:tos",
                DEFAULT_CONTACT, "Konantech VTGI Interface 1.0", "http://www.konantech.com");
    }


}
