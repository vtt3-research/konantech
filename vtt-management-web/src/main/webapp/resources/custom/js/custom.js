
function toast(heading, text, icon, hideAfter) {
    $.toast({
        heading: heading,
        text: text,
        icon: icon,
        showHideTransition: "slide",
        hideAfter: hideAfter
    });
}

var MSG = {
    alert: function (heading, text, icon, hideAfter) {
        $.toast({
            heading: heading,
            text: text,
            icon: icon,
            showHideTransition: "slide",
            hideAfter: hideAfter
        });
    },

    reload: function (title, contentMSG, isReload, location) {
        $.SmartMessageBox({
            title: title,
            content: contentMSG,
            buttons: '[' + returnmsg("ok")  +']'
        }, function (ButtonPressed) {
            if (ButtonPressed === returnmsg("ok")) {
                if (isReload) {
                    if (location == '' || location == null) {
                        window.location.reload();
                    } else {
                        window.location.href = location;
                    }
                }
            }
        });
    },

    reloads: function (title, contentMSG, location1, location2) {
        $.SmartMessageBox({
            title: title,
            content: contentMSG,
            buttons: '[' + returnmsg("ok") + '][' + returnmsg("add") + ']'
        }, function (ButtonPressed) {
            if (ButtonPressed === returnmsg("add")) {
                window.location.href = location1;
            }

            if (ButtonPressed === returnmsg("ok")) {
                window.location.href = location2;
            }
        });
    },

    confirm: function (title, contentMSG, callback) {
        $.SmartMessageBox({
            title: title,
            content: contentMSG,
            buttons: '[' + returnmsg("cancel") + '][' + returnmsg("approve") + ']'
        }, function (ButtonPressed) {
            if (ButtonPressed === returnmsg("approve")) {
                if (callback) {
                    callback.apply();
                }
            }
            if (ButtonPressed === returnmsg("cancel")) {
                $.toast({
                    heading: returnmsg("cancel"),
                    text: returnmsg("cancel.msg"),
                    icon: "info",
                    showHideTransition: "slide",
                    hideAfter: 5000
                });
            }
        });
    },

    warning : function(contentMSG) {
        $.SmartMessageBox({
            title : returnmsg("caution"),
            content : contentMSG,
            buttons : '[' + returnmsg("ok")  +']'
        }, function(ButtonPressed) {
        });
    },

    error : function(contentMSG) {
        $.SmartMessageBox({
            title : returnmsg("error"),
            content : contentMSG,
            buttons : '[' + returnmsg("ok")  +']'
        }, function(ButtonPressed) {
        });
    },

    callback : function(heading, contentMSG, callback) {
        $.SmartMessageBox({
            title: heading,
            content: contentMSG,
            buttons: '[' + returnmsg("ok")  +']'
        }, function (ButtonPressed) {
            if (ButtonPressed === returnmsg("ok")) {
                if(callback) {
                    callback.apply();
                }
            }
        });
    }
};



var myPlayer1 = null;
// videojs.options.flash.swf = "/custom/js/plugins/video-js/video-js.swf";
var player = {
    init: function(id, type, streamUrl, imageUrl) {
        if (myPlayer1 != null) {
            try {
                myPlayer1.dispose();
            } catch (e){

            }
        }
        try {
            var jsplayer = videojs(id, '',function onPlayerReady() {
                myPlayer1 = this;
                if(type == 'video') {
                    myPlayer1.src({ type : "video/mp4", src : streamUrl });
                    $("#" + id + " video").attr("poster", imageUrl);
                }

                if(type == 'audio') {
                    myPlayer1.src({ type : "audio/mp3", src : streamUrl });
                    $("#" + id + " audio").attr("poster", imageUrl);
                }
            });
        } catch(e) {};
    }
}

var video = {
    setup: function (id, streamUrl ) {
        if (myPlayer1 != null) {
            try {
                myPlayer1.dispose();
            } catch (e){

            }
        }
        try {
            var jsplayer = videojs(id, '',function onPlayerReady() {
                myPlayer1 = this;
                techOrder : ["flash", "other supported tech"];
                myPlayer1.src({type: "video/mp4", src: streamUrl });
            });

            // jsplayer.on('timeupdate', function(){
            //     var currentTime = $("#" + id + " .vjs-duration").text(time2Code("video", jsplayer.currentTime(), 29.97));
            //     var durationTime = $("#" + id + " .vjs-remaining-time").text(time2Code("video", jsplayer.duration(), 29.97));
            //     $("#" + id + " .vjs-remaining-time").text(currentTime.text().replace("Current Time ", "") + " / " + durationTime.text().replace("Duration Time ", ""));
            // });

        } catch(e) {};
    },
    stop: function (id) {
        try {
            myPlayer1.pause();
        } catch (e) {
        }
    },
    seek: function (startframeindex) {
        var starttimesec = code2frame(startframeindex, 29.97);

        try {
            myPlayer1.currentTime(starttimesec);
        } catch (e) {
            console.log("@error_while_seek" + e.message);
        }
    }
};

var audio = {
    setup: function (id, streamUrl ) {
        if (myPlayer1 != null) {
            try {
                myPlayer1.dispose();
            } catch (e){

            }
        }
        try {
            var jsplayer = videojs(id).ready(function () {
                myPlayer1 = this;
                techOrder : ["flash", "other supported tech"];
                myPlayer1.src({type: "audio/mp3", src: streamUrl });
                // myPlayer1.controlBar.show();
                // myPlayer1.load();
                // myPlayer1.play();
            });
            var type = "audio";
            jsplayer.on('timeupdate', function(){
                var currentTime = $("#" + id + " .vjs-current-time").text(time2Code(type, jsplayer.currentTime(), 999));
                $("#" + id + " .vjs-duration").text(time2Code(type, jsplayer.duration(), 999));
                $("#" + id + " .vjs-remaining-time").text(currentTime.text());
            });

        } catch(e) {};
    },
    stop: function () {
        try {
            myPlayer1.pause();
        } catch (e) {
        }
    }
};

var calendar = {
    init: function (target_body) {
        $("input[icon_type][icon_type='calendar']").datepicker({
            dateFormat: 'yy.mm.dd',
            showOn: "button",
            buttonText: "날짜선택",
            buttonImage: "resources/css/jquery/calendar.gif",
            buttonImageOnly: true,
            changeMonth: true,
            changeYear: true,
            showButtonPanel: true,
            currentText: $.i18n.prop("calendar.today"),
            closeText: $.i18n.prop("menu.close"),
            prevText: $.i18n.prop("calendar.month.previous"),
            nextText: $.i18n.prop("calendar.month.next"),
            monthNamesShort: ['01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '11', '12'],
            dayNamesMin: [$.i18n.prop("calendar.day.sunday"), $.i18n.prop("calendar.day.monday"), $.i18n.prop("calendar.day.tuesday"), $.i18n.prop("calendar.day.wednesday"), $.i18n.prop("calendar.day.thursday"), $.i18n.prop("calendar.day.friday"), $.i18n.prop("calendar.day.saturday")],
            targetBody: target_body
        });
    },

    compareDate: function (start, end, split) {
        if (start && end) {
            var start_arr = start.split(split);
            var start_date = new Date(start_arr[0], start_arr[1] - 1, start_arr[2], 0, 0, 0);
            var end_arr = end.split(split);
            var end_date = new Date(end_arr[0], end_arr[1] - 1, end_arr[2], 23, 59, 59);
            if (start_date.getTime() > end_date.getTime()) {
                return false;
            }
        }
        return true;
    },
    betweenDay: function (start, end, split) {
        if (start && end) {
            var start_arr = start.split(split);
            var start_date = new Date(start_arr[0], start_arr[1] - 1, start_arr[2], 0, 0, 0);
            var end_arr = end.split(split);
            var end_date = new Date(end_arr[0], end_arr[1] - 1, end_arr[2], 0, 0, 0);

            return (end_date.getTime() - start_date.getTime()) / 1000 / 60 / 60 / 24;
        }
        return -1;
    },
    convertDateToString: function (date) {
        return date.getFullYear() + "." + this.fillBlanks(parseInt(date.getMonth() + 1)) + "." + this.fillBlanks(date.getDate());
    },

    fillBlanks: function (number) {
        if (number > 0 && number < 10)
            return "0" + number;
        else
            return number;
    }
};


function time2Code(type, time, fps) {
    var h = Math.floor(time/3600)%24;
    var m = Math.floor(time/60)%60;
    var s = Math.floor(time%60);
    var f = Math.floor(((time%1)*fps).toFixed(3));
    if(type == "audio"){
        return (h<10?"0"+h:h)+":"+(m<10?

                "0"+m:m)+":"+(s<10?"0"+s:s) +"."+(f<10?"0"+f:f);
    }else{
        return (h<10?"0"+h:h)+":"+(m<10?"0"+m:m)+":"+(s<10?"0"+s:s) +";"+(f<10?"0"+f:f);
    }
}

function code2frame( code, fps ) {
    var t = code.replace(";", ":").split(":");
    var h = parseInt(t[0],10);
    var m = parseInt(t[1],10);
    var s = parseInt(t[2],10);
    var f = parseInt(t[3],10);
    return (h*3600)+(m*60)+s+(f/fps);
}

function paginationTotalPage(itemTotal, limit) {
    var total_page = 0;
    if (itemTotal > 0 && limit > 0) {
        if ((itemTotal % limit) == 0) {
            total_page = (itemTotal / limit);
        } else {
            total_page = ( itemTotal / limit) + 1;
        }
    } else {
        total_page = 0;
    }
    return total_page;
}

function close() {
    $.toast().reset('all');
}

$(document).ready(function() {
    $("#tree_parent").height($(window).height());
});

$(window).resize(function() {
    $("#tree_parent").height($(window).height());
});

