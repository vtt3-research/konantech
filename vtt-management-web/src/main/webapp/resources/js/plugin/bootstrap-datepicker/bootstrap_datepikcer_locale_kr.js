var locale_kr = {
  "format": "YYYY-MM-DD",
  "separator": " ~ ",
  "applyLabel": '<i class="fa fa-calendar-plus-o" aria-hidden="true"></i> 지정 날짜 검색',
  "cancelLabel": '<i class="fa fa-times" aria-hidden="true"></i> 취소',
  "fromLabel": "시작",
  "toLabel": "종료",
  "customRangeLabel": "사용자 정의",
  "daysOfWeek": [
    "일", "월", "화", "수", "목", "금", "토"
  ],
  "monthNames": [
    "1월", "2월", "3월", "4월", "5월", "6월", "7월", "8월", "9월", "10월", "11월", "12월"
  ]
  ,
  "firstDay": 1
}

var locale_single_kr = {
    "format": "YYYY-MM-DD HH:mm:ss",
    "separator": " ~ ",
    "applyLabel": '<i class="fa fa-calendar-plus-o" aria-hidden="true"></i> 지정 날짜 검색',
    "cancelLabel": '<i class="fa fa-times" aria-hidden="true"></i> 취소',
    "fromLabel": "시작",
    "toLabel": "종료",
    "customRangeLabel": "사용자 정의",
    "daysOfWeek": [
        "일", "월", "화", "수", "목", "금", "토"
    ],
    "monthNames": [
        "1월", "2월", "3월", "4월", "5월", "6월", "7월", "8월", "9월", "10월", "11월", "12월"
    ]
    ,
    "firstDay": 1
}

var ranges_kr = {
  '- 1년': [ moment().subtract(1, 'years'), moment() ],
  '- 1개월': [ moment().subtract(1, 'months'), moment() ],
  '- 1주일': [ moment().subtract(1, 'weeks'), moment() ],
  '오늘': [moment(), moment()],
  '+ 1주일': [moment(), moment().add(1, 'weeks')],
  '+ 1개월': [moment(), moment().add(1, 'months')],
  '+ 1년': [moment(), moment().add(1, 'years') ]
}
