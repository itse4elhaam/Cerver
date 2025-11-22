#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

void handle_err(int result, const char *msg) {
  if (result == EXIT_FAILURE) {
    // perror basically logs out the values in the errno, errno holds the error
    // value after a syscall
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

void bind_server(int fd, const int port) {
  struct sockaddr_in server_address;

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int bind_result =
      bind(fd, (struct sockaddr *)&server_address, sizeof(server_address));

  handle_err(bind_result, "Bind failed");

  printf("Bind success\n");
}

void listen_for_reqs(int fd, const int port, const int listen_backlog) {
  int listen_result = listen(fd, listen_backlog);

  handle_err(listen_result, "listen failed");

  printf("Waiting for conn at http://localhost:%i \n", port);
}

int accept_reqs(int fd) {
  struct sockaddr_in client_address;
  socklen_t client_address_len = sizeof(client_address);

  int client_fd =
      accept(fd, (struct sockaddr *)&client_address, &client_address_len);

  handle_err(client_fd, "Accept failed");

  /* (inet_ntop = "internet network to presentation", ntohs = "network to host
   * short") */
  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
  printf("Accepted connection from %s:%d\n", client_ip,
         ntohs(client_address.sin_port));

  return client_fd;
}
typedef struct {
  char data[1024];
  int size;
} ParseResult;

ParseResult parse_req(int client_fd) {
  ParseResult result = {0};

  result.size = recv(client_fd, result.data, sizeof(result.data), 0);

  handle_err(result.size, "Request parsing failed\n");

  return result;
}

// I want to impleent streams(SSE), headers and async as well
// I want to also learn why its easy to mess up security in c as well
int main() {
  const int PORT = 8080;
  const int LISTEN_BACKLOG = 50;

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  handle_err(fd, "socket connection failed");

  printf("Socket connection success\n");

  bind_server(fd, PORT);
  listen_for_reqs(fd, PORT, LISTEN_BACKLOG);

  int client_fd = accept_reqs(fd);

  ParseResult req = parse_req(client_fd);

  printf("Request from the frontend: %s", req.data);
}
