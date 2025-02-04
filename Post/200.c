#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#include "webport.h"


/* 
Credit 
https://bruinsslot.jp/post/simple-http-webserver-in-c/#basics

*/


char htmltext(void) {
    char response[] = 
                "HTTP/1.0 200 OK\r\n"
                "Server: webserver-c\r\n"
                "Content-type: text/html\r\n\r\n"
                "<html>\r\n"
                "<head>\r\n"
                "<h1>I Love Femboys!</h1>\r\n"
                "<p1>This is my shitty webserver that I created with C</p1>\r\n"
                "</head>\r\n"
                "</html>\r\n";
    return response;
}

int main() {

    char buffer[BUFFER_SIZE];

    // Initial setup for HTTP 200
    


    int sockfoward = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfoward == -1) {
        perror("WebSever (socket)");
        return 1;
    }
    printf("Socket Created\n");

    struct sockaddr_in host_addr;
    int hostaddrlength = sizeof(host_addr);

    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(WEBPORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    struct sockaddr_in client_addr;
    int clientaddrlength = sizeof(client_addr);


    if (bind(sockfoward, (struct sockaddr *)&host_addr, hostaddrlength) != 0) {
        perror("Webserver: (bind)");
        return 1;
    }
    printf("Server Bound");

    //Listens to any Socket 
    if (listen(sockfoward, SOMAXCONN) != 0) {
        perror("Webserver (listen)\n");
        return 1;
    }
    printf("Server Listen\n");
    
    // Proccess where the server sends data to the clien. 
    while (1) {

        //Accepts sockets 
        int newsocketforward = accept(sockfoward, (struct sockaddr*)&host_addr, (socklen_t *)&hostaddrlength);

        if (newsocketforward < 0) {
            perror("webserver (accept)\n");
            continue;
        }
        printf("connection accpeted\n");

        int sockin = getsockname(newsocketforward, (struct sockaddr *)&client_addr, (socklen_t *) &clientaddrlength);
   

        if (sockin < 0) {
            perror("Webserver (getsockname)");
            continue;
        }

        int readval = read(newsocketforward, buffer, BUFFER_SIZE);
        if (readval < 0) {
            perror("Webserver read");       
            continue;    
        }

        char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
        sscanf(buffer, "%s %s %s", method, uri, version);
        printf("[%s:%u] %s %s %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), method, version, uri);

        int writeval = write(newsocketforward, htmltext(), strlen(htmltext()));
        if (writeval < 0) {
            perror("Webserver write");
            continue;
        }
        close(newsocketforward);
    }
    return 0;


}