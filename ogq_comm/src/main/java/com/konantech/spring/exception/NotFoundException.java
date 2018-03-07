package com.konantech.spring.exception;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonProperty;
import com.konantech.spring.constants.ErrorCode;

@JsonIgnoreProperties({"stackTrace", "cause", "localizedMessage", "suppressed", "suppressedExceptions"})
public class NotFoundException extends Exception {
    private int code;

    public NotFoundException() {
        this(null);
    }

    public NotFoundException(String message) {
        super(message);
        this.code = ErrorCode.ERR_8005_FILE_NOT_FOUND;
    }

    @JsonProperty("code")
    public int getCode() {
        return code;
    }

    @JsonProperty("i18n")
    @JsonInclude(JsonInclude.Include.NON_EMPTY)
    public String getDetailMessage() {
        return super.getMessage();
    }
}
