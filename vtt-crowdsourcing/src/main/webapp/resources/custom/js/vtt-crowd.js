
var MSG = {
    alert: function (text, callback) {
        bootbox.alert({
            message: text,
            callback: function () {
                if(callback) {
                    callback.apply();
                }
            }
        })
    },
    confirm: function (text, callback) {
        bootbox.confirm({
            message: text,
            buttons: {
                confirm: {
                    label: 'Yes',
                    className: 'btn-success'
                },
                cancel: {
                    label: 'No',
                    className: 'btn-danger'
                }
            },
            callback: function (result) {
                if(result && callback) {
                    callback.apply();
                }
            }
        });
    }
};




function returnmsg(param) {
    if (param == "cancel") {
        return "취소";
    } else if (param == "cancel.msg") {
        return "취소 되었습니다";
    } else if (param == "approve") {
        return "승인";
    } else if (param == "ok") {
        return "확인";
    } else if (param == "add") {
        return "등록";
    } else if (param == "caution") {
        return "주의";
    } else if (param == "error") {
        return "오류";
    } else if (param == "intro") {
        return "인트로";
    } else if (param == "normal") {
        return "본편";
    } else if (param == "middlecm") {
        return "광고중";
    } else if (param == "ending") {
        return "엔딩";
    }
}


function toast(heading, text, icon, hideAfter) {
    $.toast({
        heading: heading,
        text: text,
        icon: icon,
        showHideTransition: "slide",
        hideAfter: hideAfter
    });
}
var reload = function(){
 location.reload();
}
var callback = function(){
    parent.postMessage("vtt_callback","*");
}