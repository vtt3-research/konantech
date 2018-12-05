package com.konantech.spring.core;

import com.konantech.spring.mapper.CrowdMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

@Component
public class WorkflowInitializing {

    private static Logger log = LoggerFactory.getLogger(WorkflowInitializing.class);

    @Value("${jobs.schedule.enable}")
    private boolean scheduleJobEnable;

    @Autowired
    CrowdMapper crowdMapper;

    //매일 1시에 crowd_target_tb에 데이터 추가
    @Scheduled(cron="0 0 1 * * *")
    public void workflowJob() throws Exception {
        log.info("Run Sync Data Schedule Task."+scheduleJobEnable);
        if(scheduleJobEnable) {
            // ToDo 서버간 동기화 구간 시작
            crowdMapper.insertObjectTargetData();
            crowdMapper.insertBackgroundTargetData();
            crowdMapper.updateMetaData();
        }
    }

}