#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SIZE = 30;

/* Control error. And exit */
void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(EXIT_FAILURE);
}

/* One argument
    First : port number
 */
int main(int argc, char *argv[])
{
    int serv_sock;      // socket handle
    char message[BUF_SIZE];
    int str_len;
    socklen_t clnt_adr_sz;

    // socket info - require server and client
    struct sockaddr_in serv_adr, clnt_adr;
    
    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);     // create socket(IPv4, UDP)
    // In addition, TCP is SOCK_STREAM

    if(serv_sock == -1)
        error_handling("socket() error");
    
    memset(&serv_adr, 0, sizeof(serv_adr));     // Init for specific variable of structure
    serv_adr.sin_family = AF_INET;      // Ipv4
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);   // internal network(127.0.0.1)
    serv_adr.sin_port = htons(atoi(argv[1]));       // port(string -> int) 

    // saved socket handle called serv_sock
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    while(1){
        clnt_adr_sz = sizeof(clnt_adr);

        //receive from client
            //server socket handle, message, message size, option, client info(structure), client info size 
            //return length of message(+NULL character)
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        
        //send to client
            // server socket handle, message, message size, client info, client info size
        sendto(serv_sock, message, str_len, 0, (struct sockaddr*)&clnt_adr, clnt_adr_sz);
    }
    close(serv_sock);
    return 0;
}