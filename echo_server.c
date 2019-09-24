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
    In server code, 
    require one argument : port number

    First step, server ON
    Following client ON
*/
int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;       // socket handle variable
    char message[BUF_SZ];       // message buffer
    int str_len, i;

    struct sockaddr_in serv_adr, clnt_adr;      // socket information structure
    socklen_t clnt_adr_sz;

    if(argc != 2){      // Input control
        printf("Usage : %s <port> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* 
    PF_INET : IPv4, 
    SOCK_STREAM : connection oriented 
    So, use TCP protocol
    */
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);    // Create server socket
    if(serv_sock == -1)
        error_handling("socket() error!");

    /* Input information of server socket. */
    memset(&serv_adr, 0, sizeof(serv_adr));     // because of zero init member
    serv_adr.sin_family = AF_INET;      // IPv4
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);   // use internal network
    serv_adr.sin_port = htons(atoi(argv[1]));   // set port(second arg)

    if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error!");
    
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error!");
    
    clnt_adr_sz = sizeof(clnt_adr);

    for(i=0; i<5; i++){
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1)
            error_handling("accept() error!");
        else
                printf("Connected client %d \n", i+1);
        
        while((str_len = read(clnt_sock, message, BUF_SZ)) != 0)
            write(clnt_sock, message, str_len);
            
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}