#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#define PORT 11000
#define BUFFER_SIZE 1024
int main()
{
  int server_fd, new_socket;
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);
  char buffer[BUFFER_SIZE];
  FILE *fp;
  struct stat st;
  long filesize;
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0)
    exit(1);
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) <
      0)
    exit(1);
  if (listen(server_fd, 3) < 0)
    exit(1);
  printf("Server started on port %d\n", PORT);
  while (1)
  {
    new_socket = accept(server_fd, (struct sockaddr *)&address,
                        &addrlen);
    if (new_socket < 0)
      continue;
    memset(buffer, 0, BUFFER_SIZE);
    read(new_socket, buffer, BUFFER_SIZE);
    printf("Client requested file: %s\n", buffer);
    if (stat(buffer, &st) == 0)
    {
      filesize = st.st_size;
      write(new_socket, "FOUND", 5);
      write(new_socket, &filesize, sizeof(filesize));
      fp = fopen(buffer, "rb");
      if (fp == NULL)
      {
        close(new_socket);
        continue;
      }
      while (!feof(fp))
      {
        size_t n = fread(buffer, 1, BUFFER_SIZE, fp);
        if (n > 0)
          write(new_socket, buffer, n);
      }
      fclose(fp);
      printf("File sent successfully.\n");
    }
    else
    {
      write(new_socket, "NOTFOUND", 8);
      printf("File not found.\n");
    }
    close(new_socket);
  }
  close(server_fd);
  return 0;
}