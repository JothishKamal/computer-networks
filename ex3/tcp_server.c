#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  int sockfd, newsockfd;
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;
  char buffer[1024];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(9090);

  bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  listen(sockfd, 5);
  printf("TCP Server running on port 9090...\n");

  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

  while (1)
  {
    memset(buffer, 0, sizeof(buffer));
    int n = read(newsockfd, buffer, sizeof(buffer));
    if (n <= 0)
      break;
    if (strncmp(buffer, "exit", 4) == 0)
      break;
    printf("Client: %s", buffer);
    write(newsockfd, buffer, strlen(buffer));
  }

  close(newsockfd);
  close(sockfd);
  return 0;
}
