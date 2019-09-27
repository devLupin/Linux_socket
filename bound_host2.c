/*
    bound_host1.c에 데이터를 전송하는 예제
    총 3회의 sendto 함수 호출을 통해서 문자열 데이터를 전송한다.
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
    int sock;
    char msg1[] = "Hi!";
    char msg2[] = "I'm another UDP host!";
    char msg3[] = "Nice to meet you";

    struct sockaddr_in your_adr;
    socklen_t your_adr_sz;

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        error_handling("socket() error");
    

}