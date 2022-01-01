#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "util.h"


int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    connect(sockfd, (sockaddr*)&serv_addr,sizeof(serv_addr));
    while (true) {
        char buf[1024];
        bzero(&buf,sizeof(buf));
        scanf("%s", buf);
        // send buffer data to the server socket,return the size of data already sent
        ssize_t write_bytes = write(sockfd, buf, sizeof(buf)); 
        if(write_bytes == -1) { //error suitation
            printf("message from server: %s\n", buf);
        } 
        bzero(&buf, sizeof(buf));
        // read data from server socket to buffer, return the size of data
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf)); 
        
        if(read_bytes > 0) {
            printf("message from server %s\n", buf);
        } else if(read_bytes == 0) { // EOF
            printf("server socket disconnected!\n");
            break;
        } else if(read_bytes == -1) { //error returned 
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    return 0;
}

