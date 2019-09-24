#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SZ    = 1024;
const int RLT_SZ    = 4;
const int OPSZ      = 4;  

void error_handling(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int sock;
    char opmsg[BUF_SZ];
    int result, opnd_cnt, i;
    struct sockaddr_in serv_adr;

    if(argc != 3){
        printf("Usage : %s <IP> <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
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
        puts("Connected.............");
    
    fputs("Operand count: ", stdout);
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;

    for(i=0; i<opnd_cnt; i++){
        printf("Operand %d: ", i+1);
        scanf("%d", (int*)&opmsg[i * OPSZ + 1]);
    }
    
    fgetc(stdin);    // for erase remaining '\n'
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]);
    write(sock, opmsg, opnd_cnt * OPSZ + 2);

    read(sock, &result, RLT_SZ);

    printf("Operation result: %d\n", result);
    close(sock);

    return 0;
}