#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>

const int BUF_SIZE = 100;

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(void)
{
    int fd = open("data.txt", O_RDONLY);
    int buf[BUF_SIZE];

    if(fd == -1)
        error_handling("open() error!");
    printf("file discripter : %d \n", fd);

    if(read(fd, buf, sizeof(buf)) == -1)
        error_handling("read() error!");

    printf("file data : %s\n", buf);
    close(fd);

    return 0;
}