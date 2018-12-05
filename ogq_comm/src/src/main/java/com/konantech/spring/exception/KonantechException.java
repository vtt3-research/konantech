package com.konantech.spring.exception;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.annotation.JsonProperty;

@JsonIgnoreProperties({"stackTrace", "cause", "localizedMessage", "suppressed", "suppressedExceptions"})
public class KonantechException extends Exception {
    private int code;

    public KonantechException() {
    }

    public KonantechException(int code) {
        this(code, null);
    }

    public KonantechException(int code, String message) {
        super(message);
        this.code = code;
    }

    @JsonProperty("code")
    public int getCode() {
        return code;
    }

}