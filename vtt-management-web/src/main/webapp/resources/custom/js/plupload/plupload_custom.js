var plupload_filters = []; //not init
var plupload_fileName = [];
var plupload_fileSize = [];
var plupload_subDirect = [];
var plupload_timeout = 0;

function write_html5_init() {

    plupload_fileName = [];
    plupload_fileSize = [];
    plupload_subDirect = [];
    plupload_timeout = 0;

    $("#uploader").plupload({
        runtimes : 'html5',
        container : 'uploader',
        url : "/enroll/upload",
        max_file_size : '100gb',
        chunk_size: '1mb',
        rename: true,
        sortable: true,
            dragdrop: true,
            views: {
                list: true,
                thumbs: true,
                active: 'thumbs'
        },
        preinit : {
            Init: function(up, info) {
                plupload_log('[Init]', 'Info:', info, 'Features:', up.features);
            },
            UploadFile: function(up, file) {
                plupload_log('[UploadFile]', file);
                // You can override settings before the file is uploaded
                // up.setOption('url', 'upload.php?id=' + file.id);
                // up.setOption('multipart_params', {param1 : 'value1', param2 : 'value2'});
                up.setOption('multipart_params', {fileName : file.name });
            }
        },
        // Post init events, bound after the internal events
        init : {
            PostInit: function() {
                // Called after initialization is finished and internal event handlers bound
                plupload_log('[PostInit]');
            },

            Browse: function(up) {
                // Called when file picker is clicked
                plupload_log('[Browse]');
            },

            Refresh: function(up) {
                // Called when the position or dimensions of the picker change
                if(up.total.size > 0) {
                    $(".plupload_droptext").hide();
                } else {
                    $(".plupload_droptext").show();
                }
                plupload_log('[Refresh]');
            },

            StateChanged: function(up) {
                // Called when the state of the queue is changed
                plupload_log('[StateChanged]', up.state == plupload.STARTED ? "STARTED" : "STOPPED");
            },

            QueueChanged: function(up) {
                // Called when queue is changed by adding or removing files
                plupload_log('[QueueChanged]');
            },

            OptionChanged: function(up, name, value, oldValue) {
                // Called when one of the configuration options is changed
                plupload_log('[OptionChanged]', 'Option Name: ', name, 'Value: ', value, 'Old Value: ', oldValue);
            },

            BeforeUpload: function(up, file) {
                // Called right before the upload for a given file starts, can be used to cancel it if required
                plupload_log('[BeforeUpload]', 'File: ', file);
            },

            UploadProgress: function(up, file) {
                // Called while file is being uploaded
                plupload_log('[UploadProgress]', 'File:', file, "Total:", up.total);
                if( up.total.percent == 100 ) {
                    clearTimeout(plupload_timeout);
                    plupload_timeout = setTimeout(function() {
                        try {
                            //html5_uncompress_msg();
                        } catch (e) {};
                    }, 5000);
                }
            },

            FileFiltered: function(up, file) {
                // Called when file successfully files all the filters
                plupload_log('[FileFiltered]', 'File:', file);
            },

            FilesAdded: function(up, files) {
                // Called when files are added to queue
                plupload_log('[FilesAdded]');
                plupload.each(files, function(file) {
                    plupload_log('  File:', file);
                });
            },

            FilesRemoved: function(up, files) {
                // Called when files are removed from queue
                plupload_log('[FilesRemoved]');
                plupload.each(files, function(file) {
                    plupload_log('  File:', file);
                });
            },

            FileUploaded: function(up, file, info) {
                plupload_log('[FileUploaded] File:', file, "Info:", info);

                var res = JSON.parse(info.response);
                var status = res.status;
                var message = res.message;

                if(status.indexOf("500") > 0) {
                    uploadFailed(plupload_fileName, file.name, message.substring(0, 100));

                    up.stop();

                    up.splice();

                    up.refresh();
                    console.log("*REFRESH! > FileUploaded");
                } else {
                    plupload_fileName.push(file.name);

                    clearTimeout(plupload_timeout);
                }
            },

            ChunkUploaded: function(up, file, info) {
                // Called when file chunk has finished uploading
                plupload_log('[ChunkUploaded] File:', file, "Info:", info);
            },

            UploadComplete: function(up, files) {
                // Called when all files are either uploaded or failed
                plupload_log('[UploadComplete]');

                uploadComplete();
            },

            Destroy: function(up) {
                // Called when uploader is destroyed
                plupload_log('[Destroy] ');
            },

            Error: function(up, args) {
                // Called when error occurs
                plupload_log('[Error] ', args);

                uploadFailed(plupload_fileName, args.file.name, args);

                up.stop();

                up.splice();

                up.refresh();
                console.log("*REFRESH! > Error");
            }
        }
    });
}

function plupload_log() {
    var str = "";
    plupload.each(arguments, function(arg) {
        var row = "";
        if (typeof(arg) != "string") {
            plupload.each(arg, function(value, key) {
                // Convert items in File objects to human readable form
                if (arg instanceof plupload.File) {
                    // Convert status to human readable
                    switch (value) {
                        case plupload.QUEUED:
                            value = 'QUEUED';
                            break;

                        case plupload.UPLOADING:
                            value = 'UPLOADING';
                            break;

                        case plupload.FAILED:
                            value = 'FAILED';
                            break;

                        case plupload.DONE:
                            value = 'DONE';
                            break;
                    }
                }

                if (typeof(value) != "function") {
                    row += (row ? ', ' : '') + key + '=' + value;
                }
            });

            str += row + " ";
        } else {
            str += arg + " ";
        }
    });
    try {
        console.log(str);
    } catch (e) {}
}
/*

function uploadComplete() {
    MSG.reloads("콘텐츠 등록", "등록이 완료되었습니다.<br>추가 등록 시, '등록'버튼을 눌러주세요.<br>'확인'버튼을 누르면 콘텐츠 페이지로 전환됩니다.", "/enroll/list/", "/content/");
    return false;
}

function uploadFailed(successList, failList, failMsg) {
    var msg = "";
    var success = "";

    if(successList.length > 0) {
        success = "* 성공한 항목<br>[" + successList + "]<br><br>";
    }

    if(failMsg.length > 0) {
        msg = "* 실패 사유<br>" + failMsg + "<br><br>";
    }

    MSG.reload("콘텐츠 등록", "* 실패한 항목<br>[" + failList + "]<br><br>" + success + msg + "다시 시도해 주세요.", false, '');
    return false;
}*/
