#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SIZE = 1024;
const int RLT_SIZE = 4;
const int OPSZ = 4;

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
    char opmsg[BUF_SIZE];
    int result, opnd_cnt, i;
    struct sockaddr_in serv_adr;

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error");
    puts("Connected...........");

    fputs("Operand count: ", stdout);
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;

    for(i=0; i<opnd_cnt; i++){
        printf("Operand %d: ", i+1);
        scanf("%d", (int*)&opmsg[i*OPSZ+1]);
    }

    fgetc(stdin);       //erase \n
    fputs("Operator ", stdout);
    scanf("%c", &opmsg[opnd_cnt*OPSZ+1]); // operator info
    write(sock, opmsg, opnd_cnt*OPSZ+2);  // Send to four operands and operator
    read(sock, &result, RLT_SIZE);

    printf("Result : %d\n", result);
    close(sock);
    return 0;
}