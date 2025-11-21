#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

// what is the diff between this and the port var?
#define LISTEN_BACKLOG 50

// I want to impleent streams(SSE), headers and async as well
int main() {
  const int PORT = 8080;
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    // perror basically logs out the values in the errno, errno holds the error
    // value after a syscall
    perror("socket connection failed");
    exit(-1);
  }

  printf("Socket connection success\n");

  struct sockaddr_in server_address;

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int bind_result =
      bind(fd, (struct sockaddr *)&server_address, sizeof(server_address));

  if (bind_result < 0) {
    perror("Bind failed");
    exit(-1);
  }

  printf("Bind success\n");

  int listen_result = listen(fd, LISTEN_BACKLOG);

  // is this how error handling is done in big c apps? there must be a better
  // way surely
  if (listen_result < 0) {
    perror("listen failed");
    exit(-1);
  }

  printf("Listen success");
}
