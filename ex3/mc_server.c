#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 10000
#define MAX_CLIENTS 100
#define BUFFER_SIZE 2048
#define USERNAME_LEN 50

int client_sockets[MAX_CLIENTS];
char usernames[MAX_CLIENTS][USERNAME_LEN];
pthread_mutex_t clients_mutex =
    PTHREAD_MUTEX_INITIALIZER;

typedef struct
{
  int sock;
  int idx;
} client_t;

void broadcast_all(const char *msg, int msglen)
{
  pthread_mutex_lock(&clients_mutex);
  for (int i = 0; i < MAX_CLIENTS; i++)
  {
    if (client_sockets[i] != -1)
    {
      int sent = send(client_sockets[i], msg, msglen, 0);
      if (sent <= 0)
      {
        close(client_sockets[i]);
        client_sockets[i] = -1;
        usernames[i][0] = '\0';
      }
    }
  }
  pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg)
{
  client_t *c = (client_t *)arg;
  int sock = c->sock;
  int idx = c->idx;
  free(c);
  char name[USERNAME_LEN];
  int r = recv(sock, name, sizeof(name) - 1, 0);
  if (r <= 0)
  {
    close(sock);
    pthread_mutex_lock(&clients_mutex);
    client_sockets[idx] = -1;
    usernames[idx][0] = '\0';
    pthread_mutex_unlock(&clients_mutex);
    return NULL;
  }
  name[r] = '\0';
  name[strcspn(name, "\r\n")] = '\0';
  pthread_mutex_lock(&clients_mutex);
  strncpy(usernames[idx], name, USERNAME_LEN - 1);
  usernames[idx][USERNAME_LEN - 1] = '\0';
  pthread_mutex_unlock(&clients_mutex);
  char joinmsg[USERNAME_LEN + 32];
  int jm = snprintf(joinmsg, sizeof(joinmsg), "%s has joined the chat\n", name);
  broadcast_all(joinmsg, jm);
  char buffer[BUFFER_SIZE];
  while (1)
  {
    int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0)
      break;
    buffer[bytes] = '\0';
    char out[USERNAME_LEN + BUFFER_SIZE + 4];
    int outlen = snprintf(out, sizeof(out), "%s: %s", name, buffer);
    broadcast_all(out, outlen);
  }
  char leavemsg[USERNAME_LEN + 32];
  int lm = snprintf(leavemsg, sizeof(leavemsg), "%s has left the chat\n", name);
  pthread_mutex_lock(&clients_mutex);
  client_sockets[idx] = -1;
  usernames[idx][0] = '\0';
  pthread_mutex_unlock(&clients_mutex);
  broadcast_all(leavemsg, lm);
  close(sock);
  return NULL;
}

int main()
{
  for (int i = 0; i < MAX_CLIENTS; i++)
    client_sockets[i] = -1;

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);
  bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  listen(server_fd, 10);
  printf("Chat server started on port %d\n", PORT);

  while (1)
  {
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int new_sock = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
    if (new_sock < 0)
      continue;
    pthread_mutex_lock(&clients_mutex);
    
    int found = -1;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
      if (client_sockets[i] == -1)
      {
        client_sockets[i] = new_sock;
        usernames[i][0] = '\0';
        found = i;
        break;
      }
    }
    
    pthread_mutex_unlock(&clients_mutex);
    
    if (found == -1)
    {
      const char *full = "Server full. Try later.\n";
      send(new_sock, full, strlen(full), 0);
      close(new_sock);
      continue;
    }
    
    client_t *c = malloc(sizeof(client_t));
    c->sock = new_sock;
    c->idx = found;
    pthread_t tid;
    pthread_create(&tid, NULL, handle_client, c);
    pthread_detach(tid);
  }
  
  close(server_fd);
  return 0;
}