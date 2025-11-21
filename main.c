#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

// I want to impleent streams(SSE), headers and async as well
int main() {
  const int PORT = 8080;
  const int LISTEN_BACKLOG = 50;
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    // perror basically logs out the values in the errno, errno holds the error
    // value after a syscall
    perror("socket connection failed");
    exit(EXIT_FAILURE);
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
    exit(EXIT_FAILURE);
  }

  printf("Bind success\n");

  int listen_result = listen(fd, LISTEN_BACKLOG);

  if (listen_result < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }

  printf("Waiting for conn at http://localhost:%i \n", PORT);

  struct sockaddr_in client_address;
  socklen_t client_address_len = sizeof(client_address);

  int client_fd =
      accept(fd, (struct sockaddr *)&client_address, &client_address_len);
  if (client_fd < 0) {
    perror("Accept failed");
    exit(EXIT_FAILURE);
  }

  printf("accept success\n");

  char buffer[1024] = {0};

  int req_result = recv(client_fd, buffer, sizeof(buffer), 0);

  if (req_result < 0) {
    perror("Request parsing failed\n");
    exit(EXIT_FAILURE);
  }

  printf("Request from the frontend: %s", buffer);
}
