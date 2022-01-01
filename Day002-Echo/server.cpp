#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "util.h"


int main() {
    // TCP Socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // client connect server by using server's IP address and port
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr)); // bzero is included in <string.h> or <cstring>

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(sockfd, SOMAXCONN); //SOMAXCONN is a Macro means 128 （System suggest maximum value）

    // receive a connection from client
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));

    // accept function will block current program until a client socket is received
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len); 
    // printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

    while(true) {
        char buf[1024]; // define buffer area
        bzero(&buf, sizeof(buf)); //empty the buffer area
        ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf)); // return the size of data already read
        if(read_bytes > 0) {
            printf("message from client fd %d: %s\n", clnt_sockfd, buf);
            write(clnt_sockfd, buf, sizeof(buf)); // 相同的数据写回到客户端
        } else if(read_bytes == 0) {
            printf("client fd %d disconnected\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        } else if(read_bytes == -1) {
            close(clnt_sockfd);
            errif(true, "socket read error");
        }
    }

    return 0;
}




