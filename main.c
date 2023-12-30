#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <netinet/in.h>

void main(){
    // AF_INET is the domain for regular ipv4
    // SOCK_STREAM is what http uses
    // 0 is for defaults  
    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {
        AF_INET,
        0x901f, // hex for 8080 reversed 0x1f90
        0
    };
    // binding the socket to a certain address
    bind(s, &addr, sizeof(addr));

    // 10 => max number of parallel reqs before sending rejections
    listen(s, 10);

    int client_fd = accept(s, 0, 0);

    // accepting client rec data 
    char buffer[256] = {0};
    recv(client_fd, buffer, 256, 0);

    // sample req: 
    // GET /file.html ..... 

    // getting rid of first 5 spaces by moving the pointer up 5 times
    char *f = buffer + 5;
    *strchr(f, ' ') = 0; // changes the space to null terminated string
    int opened_fd = open(f, O_RDONLY);
    sendfile(client_fd, opened_fd, 0, 256);
    close(opened_fd);
    close(client_fd);

    printf("It ran succesfully");
}
