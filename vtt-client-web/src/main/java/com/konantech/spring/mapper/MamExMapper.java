package com.konantech.spring.mapper;

import com.konantech.spring.domain.mamex.MamExCnfTb;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Param;

import java.util.List;

/**
 * Created by seheung on 2017. 4. 21..
 */
@Mapper
public interface MamExMapper {
    List<MamExCnfTb> selectMamEx(@Param("trname") String trName, @Param("pool") int pool);
    void updateMamEx(MamExCnfTb mamEx);
    void insertMamEx(@Param("uri") String URI, @Param("trname") String trName, @Param("pool") int pool);
    void deleteMamEx(@Param("uri") String URI);
}
