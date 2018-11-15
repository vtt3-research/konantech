
package com.konantech.spring.domain.section;

import lombok.Data;

import java.util.List;

@Data
public class Section {
    public String fileName;
    public String registedName;
    public List<QaResult> qaResults = null;
}
