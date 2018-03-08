/*url : webSocket receive url
message : json*/
var ws = null;

function webSocketConnect(url, message, callback) {

    console.log("--- FROM MING ---" + ws);
    console.log(message);

    $(window).bind("beforeunload", function() {
        ws.close();
    });

    if(ws == null) {
        ws = new WebSocket(url);
    } else {
        ws.send(message);
    }

    ws.onopen = function () {
        ws.send(message);
        console.log("WS OPEN > " + ws.bufferedAmount);
    };

    ws.onmessage = function (onmessage) {
        callback(JSON.parse(onmessage.data));
    };

    ws.onerror = function () {
        var error = new Object();
        error.command = "error";
        error.result = "fail";

        callback(error);
    };

    ws.onclose = function (event) {
        console.log("WS CLOSE > " + event.code);
        ws.close();
        ws = null;
    };
}