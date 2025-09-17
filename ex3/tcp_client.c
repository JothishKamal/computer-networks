#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  int sockfd;
  struct sockaddr_in serv_addr;
  char buffer[1024];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(9090);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  printf("Connected to TCP server. Type 'exit' to quit.\n");

  while (1)
  {
    memset(buffer, 0, sizeof(buffer));
    printf("You: ");
    fgets(buffer, sizeof(buffer), stdin);
    write(sockfd, buffer, strlen(buffer));

    if (strncmp(buffer, "exit", 4) == 0)
      break;

    memset(buffer, 0, sizeof(buffer));
    read(sockfd, buffer, sizeof(buffer));
    printf("Echo: %s", buffer);
  }

  close(sockfd);
  return 0;
}
