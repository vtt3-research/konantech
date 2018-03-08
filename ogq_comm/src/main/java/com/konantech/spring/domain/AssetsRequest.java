package com.konantech.spring.domain;

import lombok.Data;
import org.hibernate.validator.constraints.NotEmpty;
import org.hibernate.validator.constraints.URL;

@Data
public class AssetsRequest {
    @NotEmpty
    private String request_id;
    @NotEmpty @URL(message = "비디오 URL을 입력하세요")
    private String video_url;
    @NotEmpty @URL(message = "callback URL을 입력하세요")
    private String callback_url;

    public AssetsRequest(){
    }

}
