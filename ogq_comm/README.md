# (주)코난테크놀로지 OGQ 연동 서버

OGQ 서비스와, 비디오 등록 관리 서버와 연동하는 서버 입니다.
JAVA 프로젝트로, Gradle 프로젝트로 오픈하면 됩니다.

## 서비스 주소

> 1. [VTT 클라이언트 데모] 접속 [ http://183.110.246.21:8080/ogq/ ]
> 1. 로그인 -> admin / admin 로긴


개발환경
>- Intellij IDEA 환경
>- gradle 프로젝트 열기
>- lombok 라이브러리 사용 ex) @Data
>- Proferences > Build, Excution, Depoloyment > Compiler > Annotation Processors > Enable annotation processing (체크)

빌드
```
$ gradle clean
$ gradle build -x test
$ ls ./build/libs/ogq-comm-1.0.war
  ./build/libs/ogq-comm-1.0.war
$
```

DATABASE
> 1. source > doc > sql > *.sql 파일로 테이블 생성, 기본데이타 구조 외
