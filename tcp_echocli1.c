tcp_echocli1.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 127

int main(int argc, char *argv[]){
        int s,nbyte;
        struct sockaddr_in servaddr;
        char buf[ MAXLINE+1 ];

        if(argc != 3){
                printf("Usage: %s ip_address\n", argv[0]);
                exit(0);
        }

        //소켓생성
        if((s=socket(PF_INET, SOCK_STREAM, 0)) <0 ){
                perror("socket fail");
                exit(0);
        }

        //서버의 소켓주소 구조체 servaddr을 \0으로 초기화
        bzero((char *)&servaddr, sizeof(servaddr));

        //servaddr의 주소지정
        servaddr.sin_family=AF_INET;
        inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
        servaddr.sin_port=htons(atoi(argv[2]));

        //연결요청
        if(connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) <0 ){
                perror("connect fail");
                exit(0);
        }

        printf("입력: ");
        if(fgets(buf, sizeof(buf), stdin) ==NULL)
                exit(0);
        nbyte=strlen(buf);

        //에코서버로 메세지 송신
        if(write(s,buf,nbyte) <0){
                printf("write error\n");
                exit(0);
        }

        //수신된 에코 데이터 화면출력
        printf("수신: ");
        if((nbyte=read(s,buf,MAXLINE))< 0){
                perror("read fail");
                exit(0);
        }

        buf[nbyte]=0;
        printf("%s", buf);
        close(s);
        return 0;
}
