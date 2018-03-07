
1. 인증
 POST /v2/login 을 통해 아이디, 패스워드 입력후 token 값 받음


2. 호출 
PUT /videoTagging 호출할때 파리미터 + token을 header에 포함하여 호출
ex)
{
  "callback_url": "http://183.110.246.21:8080/ogq/response",
  "request_id": "0234567890",
  "video_url": "http://video.url"
}

3. 결과
callback_url => 실제 콜백받을 URL을 쓰면 10초후 콜백을 호출함
임시로 /response 를 호출하면 내부적으로 콜백을 받으면 받은 파라미터릴 캐시서버(redis) 60초간 보관함
=>
GET /responseTest -> cache서버(redis)의 내용을 화면에 계속 보여줌)

새로고침으로 웹에서 호출하면 
GET /responseTest 를 계속 호출하면 10초후 결과가 보이는걸 알수있음


===============================

** 전체가 모두 dummy 데이타입니다 api 인터페이스도 확정이 아닙니다 좀더 세부화 시킬 예정입니다
전체적으로 이와같은 내용으로 결과를 드릴터이니 구조만 참고해주세요



