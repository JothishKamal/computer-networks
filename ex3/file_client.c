#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 11000
#define BUFFER_SIZE 1024
int main()
{
  int sock;
  struct sockaddr_in serv_addr;
  char filename[256], buffer[BUFFER_SIZE];
  FILE *fp;
  long filesize, received = 0;
  ssize_t n;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
    exit(1);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    exit(1);
  printf("Enter filename to request: ");
  scanf("%s", filename);
  write(sock, filename, strlen(filename));
  memset(buffer, 0, BUFFER_SIZE);
  read(sock, buffer, 8);
  if (strncmp(buffer, "FOUND", 5) == 0)
  {
    read(sock, &filesize, sizeof(filesize));
    fp = fopen(filename, "wb");
    if (fp == NULL)
      exit(1);
    while (received < filesize && (n = read(sock, buffer,
                                            BUFFER_SIZE)) > 0)
    {
      fwrite(buffer, 1, n, fp);
      received += n;
    }
    fclose(fp);
    printf("File received and saved as %s\n", filename);
  }
  else
  {
    printf("File not found on server.\n");
  }
  close(sock);
  return 0;
}