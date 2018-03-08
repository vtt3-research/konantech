var locale_lang = {
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
}

var ranges_lang = {
  '전체': [new Date(1900, 0, 1), new Date(2100, 0, 1)],
  '-1년': [ moment().subtract(1, 'years'), moment() ],
  '-1개월': [ moment().subtract(1, 'months'), moment() ],
  '-1주일': [ moment().subtract(1, 'weeks'), moment() ],
  '오늘': [moment(), moment()]
}

var detail_locale_kr = {
    "format": "YYYY-MM-DD hh:mm",
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
}