# (주)코난테크놀로지 VTT 비디오 메타데이터 매니지먼트 시스템 데모

비디오 메타데이터 매니지먼트 시스템 설계 데모

JAVA 프로젝트로, Gradle 프로젝트로 오픈하면 됩니다.

## 서비스 주소

> 1. [VTT 클라이언트 데모] 접속 [ http://183.110.246.21:8080/vtt-management/ ]
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
$ ls ./build/libs/vtt-management-web-1.0.war
  ./build/libs/vtt-management-web-1.0.war
$
```

DATABASE
> 1. source > doc > sql > *.sql 파일로 테이블 생성, 기본데이타 구조외
> 1.  