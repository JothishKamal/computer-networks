#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#define PORT 10000
#define BUFFER_SIZE 2048
#define USERNAME_LEN 50
int client_sock;
void *recv_thread_fn(void *arg)
{
  char buffer[BUFFER_SIZE];
  while (1)
  {
    int r = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
    if (r <= 0)
    {
      printf("Disconnected from server\n");
      close(client_sock);
      exit(0);
    }
    buffer[r] = '\0';
    printf("%s", buffer);
    fflush(stdout);
  }
  return NULL;
}
int main(int argc, char **argv)
{
  const char *server_ip = "127.0.0.1";
  if (argc >= 2)
    server_ip = argv[1];
  client_sock = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in serv;
  serv.sin_family = AF_INET;
  serv.sin_port = htons(PORT);
  inet_pton(AF_INET, server_ip, &serv.sin_addr);
  if (connect(client_sock, (struct sockaddr *)&serv, sizeof(serv)) < 0)
  {
    perror("connect");
    return 1;
  }
  char username[USERNAME_LEN];
  printf("Enter username: ");
  if (!fgets(username, sizeof(username), stdin))
    return 0;
  username[strcspn(username, "\r\n")] = '\0';
  send(client_sock, username, strlen(username), 0);
  pthread_t rid;
  pthread_create(&rid, NULL, recv_thread_fn, NULL);
  pthread_detach(rid);
  char msg[BUFFER_SIZE];
  while (1)
  {
    if (!fgets(msg, sizeof(msg), stdin))
      break;
    char tmp[BUFFER_SIZE];
    strcpy(tmp, msg);
    tmp[strcspn(tmp, "\r\n")] = '\0';
    if (strcmp(tmp, "exit") == 0)
    {
      close(client_sock);
      exit(0);
    }
    if (strlen(tmp) == 0)
      continue;
    int sent = send(client_sock, msg, strlen(msg), 0);
    if (sent <= 0)
    {
      printf("Error sending message\n");
      close(client_sock);
      exit(0);
    }
  }
  close(client_sock);
  return 0;
}