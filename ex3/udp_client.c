#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  int sockfd;
  struct sockaddr_in serv_addr;
  socklen_t len;
  char buffer[1024];

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(9090);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  printf("Connected to UDP server. Type 'exit' to quit.\n");

  while (1)
  {
    memset(buffer, 0, sizeof(buffer));
    printf("You: ");
    fgets(buffer, sizeof(buffer), stdin);
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    
    if (strncmp(buffer, "exit", 4) == 0)
      break;
    
    memset(buffer, 0, sizeof(buffer));
    len = sizeof(serv_addr);
    int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&serv_addr, &len);
    buffer[n] = '\0';
    printf("Echo: %s", buffer);
  }
  
  close(sockfd);
  return 0;
}