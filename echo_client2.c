#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SZ  1024

void error_handling(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);

    exit(EXIT_FAILURE);
}

/*
    In client code, 
    require two argument : 1st(IP addr), 2nd(port number)

    First step, server ON
    Following client ON
*/
int main(int argc, char *argv[])
{
    int sock;
    char msg[BUF_SZ] = {NULL};
    int str_len = -1, recv_len;
    struct sockaddr_in serv_adr;
    int recv_cnt;

    if(argc != 3){
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error!");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected............");

    while(1){
        fputs("Input message(Q to quit): ", stdout);
        fgets(msg, BUF_SZ, stdin);

        /*
        Escape condition called Application protocol.
        Define end of Data
        */
        if(!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
            break;
        
        str_len = write(sock, msg, strlen(msg));

        recv_len = 0;
        while(recv_len < str_len){
            recv_cnt = read(sock, &msg[recv_len], BUF_SZ - 1);
            if(recv_cnt == -1)
                error_handling("read() error!");
            else
                recv_len += recv_cnt;
        }
        msg[recv_len] = '\0';
        printf("Message from server : %s", msg);
    }
    close(sock);
    return 0;
}