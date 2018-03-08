var locale_lang = {};
var ranges_lang = {};

var locale_kr = {
  "format": "YYYY-MM-DD",
  "separator": " ～ ",
  "applyLabel": "확인",
  "cancelLabel": "취소",
  "fromLabel": "시작",
  "toLabel": "종료",
  "customRangeLabel": "사용자정의",
  "daysOfWeek": [
    "일", "월", "화", "수", "목", "금", "토"
  ],
  "monthNames": [
    "1월", "2월", "3월", "4월", "5월", "6월", "7월", "8월", "9월", "10월", "11월", "12월"
  ]
  ,
  "firstDay": 0
};

var locale_en = {
    "format": "YYYY-MM-DD",
    "separator": " ~ ",
    "applyLabel": "Ok",
    "cancelLabel": "Cancel",
    "fromLabel": "Start",
    "toLabel": "End",
    "customRangeLabel": "Custom",
    "daysOfWeek": [
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    ],
    "monthNames": [
        "1Month", "2Month", "3Month", "4Month", "5Month", "6Month", "7Month", "8Month", "9Month", "10Month", "11Month", "12Month"
    ]
    ,
    "firstDay": 1
};

var ranges_kr = {
  '전체': [new Date(1900, 0, 1), new Date(2100, 0, 1)],
  '-1년': [ moment().subtract(1, 'years'), moment() ],
  '-1개월': [ moment().subtract(1, 'months'), moment() ],
  '-1주일': [ moment().subtract(1, 'weeks'), moment() ],
  '오늘': [moment(), moment()]
};

var ranges_en = {
    'All': [new Date(1900, 0, 1), new Date(2100, 0, 1)],
    '-1Year': [ moment().subtract(1, 'years'), moment() ],
    '-1Month': [ moment().subtract(1, 'months'), moment() ],
    '-1Week': [ moment().subtract(1, 'weeks'), moment() ],
    'Today': [moment(), moment()]
}







