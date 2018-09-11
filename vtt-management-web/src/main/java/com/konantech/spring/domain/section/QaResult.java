
package com.konantech.spring.domain.section;

import lombok.Data;

import java.util.List;

@Data
public class QaResult {
    public List<String> visualPeriodNum = null;
    public String periodNum;
    public String startTime;
    public String endTime;
    public List<DescriptionInfo> descriptionInfo = null;
    public List<QaInfo> qaInfo = null;
    public List<RelatedPeriodInfo> relatedPeriodInfo = null;
}
