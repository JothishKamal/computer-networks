#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  int sockfd;
  struct sockaddr_in serv_addr, cli_addr;
  socklen_t len;
  char buffer[1024];
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(9090);
  bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  printf("UDP Server running on port 9090...\n");
  while (1)
  {
    memset(buffer, 0, sizeof(buffer));
    len = sizeof(cli_addr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cli_addr, &len);
    buffer[n] = '\0';
    if (strncmp(buffer, "exit", 4) == 0)
      break;
    printf("Client: %s", buffer);
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&cli_addr, len);
  }
  close(sockfd);
  return 0;
}