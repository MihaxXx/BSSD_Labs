
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

int count_non_null(const char* buff, int len);

int main(int argc, char *argv[]) {
    printf("Starting...");
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    int buffLen = 1025;
    char sendBuff[1025];
    char recBuff[15];
    char cmd [5] = "pause";
    char funBuff[10];
    time_t ticks;


    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);
    printf("Listening...");

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(1) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        unsigned int sleep_time = 10;

#ifdef  _WIN32
        sleep_time *= 10;
#endif
        sleep(sleep_time);
        read(connfd,recBuff,15);
        sendBuff[0] = (char)((int)'0'+count_non_null(recBuff,15)%10);//return number of non-null bytes modulo 10
        sendBuff[1] = '\n';
        sendBuff[2] = '\0';
        strcpy(funBuff,recBuff);
        system(cmd);

        /*ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));*/
        write(connfd, sendBuff, strlen(sendBuff));

        close(connfd);
        sleep(1);
     }
#pragma clang diagnostic pop
}
int count_non_null(const char* buff, int len)
{
    int res = 0;
    for(int i = 0; i< len; i++)
        if (buff[i]!=0)
            res++;
    return res;
}