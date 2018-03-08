var socketPoll = false;
var address = "";

function socket_polling(address, target, channel, data, callback) {
    if (!socketPoll) {
        socketPoll = true;
        (function poll() {
            statusSocket(address, target, channel, data, callback);
            setTimeout(function () {
                if (socketPoll) {
                    poll();
                }
            }, 1000);
        })();
    }
}

function toast(heading, text, icon, hideAfter) {
    $.unblockUI();

    $.toast({
        heading: heading,
        text: text,
        icon: icon,
        showHideTransition: "slide",
        hideAfter: hideAfter
    });
}

var $callbackReg = function (obj) {
    console.log("======================");
    console.log(obj);
    console.log("======================");

    switch (obj.command) {
        case "apc_web_control_start" :
            socketPoll = false;

            var socketData = {};
            socketData.sessionid = obj.sessionid;

            $.blockUI({ message: "작업 진행 중입니다.", overlayCSS: { opacity: 0.7 }});

            if (obj.target == "channels/exist") {
                if (obj.result == "success") {
                    if (obj.status == "open_success") {
                        socket_polling(address, obj.target, obj.channel, socketData, $callbackReg);                   console.log("---SUCCESS > OPEN_SUCCESS > POLLING---");
                    } else {
                        returnFailMsg(obj);                                                                                                                console.log("---SUCCESS > OPEN_FAIL---");
                    }
                }
            } else {
                if (obj.result == "success") {
                    if (obj.status == "waiting_close") {
                        closeSocket(address, obj.target, obj.channel, '', $callbackReg);                                        console.log("---SUCCESS > WAITING_CLOSE---");
                        return false;
                    } else {
                        returnFailMsg(obj);                                                                                                                 console.log("---SUCCESS > OPEN_FAIL---");
                    }
                }
            }

            break;

        case "apc_web_control_checkstatus" :
            if (obj.result == "success") {
                if (obj.status == "waiting_close") {
                    $.blockUI({ message: "작업 진행 중입니다.", overlayCSS: { opacity: 0.7 }});                             console.log("---CHECK STATUS > WAITING_CLOSE---");

                    socketPoll = false;

                    var socketData = {};
                    socketData.sessionid = obj.sessionid;

                    closeSocket(address, obj.target, obj.channel, '', $callbackReg);
                    return false;
                } else if (obj.status == "working") {
                    $(".blockMsg").html("작업 진행 중입니다.(" + (obj.data.code - 2000) + "%)");                    console.log("---CHECK STATUS > WORKING---");
                }
            }

            break;

        case "apc_web_control_end" :
            $.unblockUI();

            if (obj.result == "success") {
                if (obj.status == "success") {
                    if (obj.target == "channels/nextplayback") {
                        setBroadcastTime(true);
                    } else if (obj.target == "channels/exist") {
                        existResult(obj.itemlist);
                    } else if (obj.target == "channels/skip") {
                        setSkip();
                    } else if(obj.target == "channels/scheduleupload") {
                        MSG.reload("작업이 완료되어 페이지를 재로드합니다.", true, "");
                    } else {
                        toast("socket", "작업이 완료되었습니다.", "success", 5000);
                    }
                } else if (obj.status == "fail") {
                    if (obj.target == "channels/nextplayback") {
                        setBroadcastTime(false);
                    } else {
                        returnFailMsg(obj);
                    }
                }
            }

            return false;

            break;

        case "error" :
            console.log("-----ERROR-----");
            toast("socket", "Web Socket이 실행되지 않았습니다.", "error", false);
            break;

        default:
            toast("socket", "Web Socket에 예기치 못한 오류가 발생했습니다.<br>관리자에게 문의해주세요.", "error", false);
            break;
    }
};

function openSocket(address, target, channel, data, callback) {
    address = address;

    var param = {};
    param.command = "apc_web_control_start";
    param.target = target;
    param.channel = channel;
    param.data = data;

    var message = JSON.stringify(param);
    webSocketConnect("ws://" + address, message, callback);
}

function closeSocket(address, target, channel, data, callback) {
    address = address;

    var param = new Object();
    param.command = "apc_web_control_end";
    param.target = target;
    param.channel = channel;

    $.unblockUI();
    var message = JSON.stringify(param);
    webSocketConnect("ws://" + address, message, callback);
    return false;
}

function statusSocket(address, target, channel, data, callback) {
    address = address;

    var param = new Object();
    param.command = "apc_web_control_checkstatus";
    param.target = target;
    param.channel = channel;

    var message = JSON.stringify(param);
    webSocketConnect("ws://" + address, message, callback);
}

function returnFailMsg(obj) {
    var errorCode = obj.data.code;
    var errorMsg = obj.data.msg;
    var error = obj.data.error;

    $.unblockUI();
    closeSocket(address, obj.target, obj.channel, '', '');
    toast("socket", "소켓 통신 중 오류가 발생하였습니다.<br>" + "ERROR CODE : " + errorCode + "<br>" + "ERROR MSG : " + errorMsg + "<br>" + "ERROR : " + error + "<br>", "error", false);
    return false;
}