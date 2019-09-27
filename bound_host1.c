/*
    TCP는 데이터의 경계가 존재하지 않는 프로토콜으로, 
    데이터 송수신 과정에서 호출하는 입출력함수의 호출횟수는 큰 의미가 없었다.

    UDP는 데이터의 경계가 존재하는 프로토콜으로,
    데이터 송수신 과정에서 호출하는 입출력 함수의 호출횟수가 큰 의미가 있다.
    때문에 입력함수와 출력함수의 호출횟수가 완벽히 일치해야 송신된 데이터 전부를 수신할 수 있다.

    Follow UDP send/recv exam
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SZ    = 1024;

void error_handling(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int sock, str_len, i;
    char message[BUF_SZ];
    struct sockaddr_in my_adr, your_adr;
    socklen_t adr_sz;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("sock() error");

    memset(&my_adr, 0, sizeof(my_adr));
    my_adr.sin_family = AF_INET;
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_adr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr*)&my_adr, sizeof(my_adr)) == -1)
        error_handling("bind() error");

    for(i=0; i<3; i++){
        sleep(5);   //delay 5 sec;

        adr_sz = sizeof(your_adr);
        str_len = recvfrom(sock, message, BUF_SZ, 0, (struct sockaddr*)&your_adr, &adr_sz);
        printf("Message %d: %s\n", i+1, message);
    }
    close(sock);
    return 0;
}