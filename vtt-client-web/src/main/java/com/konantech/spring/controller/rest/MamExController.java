package com.konantech.spring.controller.rest;

import com.konantech.spring.domain.mamex.MamExRequest;
import com.konantech.spring.entities.ErrorCode;
import com.konantech.spring.exception.ApiException;
import com.konantech.spring.exception.FieldException;
import com.konantech.spring.response.MapResponse;
import com.konantech.spring.response.ObjectResponse;
import com.konantech.spring.service.MamExService;
import io.swagger.annotations.ApiImplicitParam;
import io.swagger.annotations.ApiImplicitParams;
import io.swagger.annotations.ApiOperation;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.validation.BindingResult;
import org.springframework.web.bind.annotation.*;

import javax.servlet.http.HttpServletRequest;
import javax.validation.Valid;

@RestController
public class MamExController {

    @Autowired
    MamExService mamexService;

    private static final String callMamEx = "{  <br/>" +
            "   \"workinfo\":{  <br/>" +
            "   },<br/>" +
            "   \"assetinfo\":{  <br/>" +
            "      \"type\":\"shot\",<br/>" +
            "      \"shotinfo\":{  <br/>" +
            "         \"assetid\":112,<br/>" +
            "         \"sequencetype\":\"start\",<br/>" +
            "         \"starttimecode\":\"00:00:10:11\",<br/>" +
            "         \"endtimecode\":\"00:00:11:11\",<br/>" +
            "         \"keypositiontimecode\":\"00:00:10:25\",<br/>" +
            "         \"startframeindex\":111,<br/>" +
            "         \"endframeindex\":122,<br/>" +
            "         \"keypositionframeindex\":1117,<br/>" +
            "         \"facerect\":[  <br/>" +
            "            \"11 12 13 14\",<br/>" +
            "            \"11 12 13 14\",<br/>" +
            "            \"11 12 13 14\"<br/>" +
            "         ],<br/>" +
            "         \"storage\":\"X\",<br/>" +
            "         \"filepath\":\"/data/2010/12/23/11111.jpg\"<br/>" +
            "      }<br/>" +
            "   }<br/>" +
            "}";
    @ApiOperation(value = "mamEx 호출", notes = callMamEx )
    @ApiImplicitParams({
            @ApiImplicitParam(name = "trname", value = "trname", required = true, dataType = "string", paramType = "path", defaultValue = "ex_update_jobresult"),
            @ApiImplicitParam(name = "pool", value = "pool", required = true, dataType = "string", paramType = "path", defaultValue = "1")
    })
    @RequestMapping(value = "/v2/mamEx/{trname}/{pool}", method = {RequestMethod.PUT})
    public ResponseEntity<?> callMamEx(@PathVariable String trname, @PathVariable int pool, @RequestBody String request) throws Exception {
        try {
            mamexService.callMamEx(trname, pool, request);

            MapResponse mapResponse = new MapResponse();
            mapResponse.setResult("success");
            return new ObjectResponse(mapResponse);

        } catch (Exception e) {
            throw new ApiException(ErrorCode.ERR_7010_COMPSERVER_ERROR, "callMamEx 오류 입니다 ( " + e.getMessage() + " )");
        }
    }

    private static final String createMamEx = "{</br>" +
            "        \"uri\":\"http://127.0.0.1:9999\",</br>" +
            "            \"pool\": 1,</br>" +
            "            \"trnames\": [\"ex_test_test\"]</br>" +
            "    }</br>";
    @ApiOperation(value = "mamEx 생성", notes = createMamEx )
    @RequestMapping(value = "/v2/mamEx", method = {RequestMethod.POST})
    public ResponseEntity<?> createMamEx(@Valid @RequestBody MamExRequest request, BindingResult result, HttpServletRequest httpServletRequest) throws Exception {
        if (result.hasErrors()) {
            throw new FieldException(result);
        }
        try {
            mamexService.createMamEx(
                    request.getUri(),
                    request.getPool(),
                    request.getTrnames());

            MapResponse mapResponse = new MapResponse();
            mapResponse.setResult("success");
            return new ObjectResponse(mapResponse);
        } catch (Exception e) {
            throw new ApiException(ErrorCode.ERR_7010_COMPSERVER_ERROR, "createMamEx 오류 입니다 ( " + e.getMessage() + " )");
        }
    }

}
