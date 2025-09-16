#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_FRAMES 100

typedef struct
{
  int id;
  int timer;
  bool acked;
  bool sent;
  bool in_transit;
  int arrival_time;
  int transmissions;
} Frame;

typedef struct
{
  int id;
  bool received;
  bool acked;
} ReceiverBuffer;

int N, W, timeout, scenario;
Frame sender[MAX_FRAMES];
ReceiverBuffer receiver[MAX_FRAMES];
bool frameLostOnce[MAX_FRAMES] = {false};

int main()
{
  printf("Enter number of frames (N): ");
  scanf("%d", &N);
  printf("Enter window size (W): ");
  scanf("%d", &W);
  printf("Enter timeout value (units): ");
  scanf("%d", &timeout);
  printf("Choose Scenario: 1.Normal 2.Lost Frame 3.Lost ACK : ");
  scanf("%d", &scenario);

  for (int i = 0; i < N; i++)
  {
    sender[i].id = i;
    sender[i].acked = false;
    sender[i].sent = false;
    sender[i].timer = 0;
    sender[i].in_transit = false;
    sender[i].arrival_time = -1;
    sender[i].transmissions = 0;

    receiver[i].id = i;
    receiver[i].received = false;
    receiver[i].acked = false;
  }

  int time = 0, nextFrameToSend = 0, allAcked = 0;
  bool ackLostOnce[MAX_FRAMES] = {false};
  int channel_delay = 2;
  int ack_arrival_time[MAX_FRAMES];
  for (int i = 0; i < N; i++)
    ack_arrival_time[i] = -1;

  printf("\n--- Simulation Start ---\n");

  while (allAcked < N && time < 200)
  {

    int winStart = 0;
    for (int i = 0; i < N; i++)
    {
      if (!sender[i].acked)
      {
        winStart = i;
        break;
      }
    }

    while (nextFrameToSend < N && nextFrameToSend < winStart + W)
    {
      if (!sender[nextFrameToSend].sent || sender[nextFrameToSend].timer == 0)
      {
        printf("[Time %d] Sender: Sending Frame %d -> Timer[%d]=%d started\n",
               time, nextFrameToSend, nextFrameToSend, timeout);
        sender[nextFrameToSend].sent = true;
        sender[nextFrameToSend].timer = timeout;
        sender[nextFrameToSend].in_transit = true;
        sender[nextFrameToSend].arrival_time = time + channel_delay;
        sender[nextFrameToSend].transmissions++;
      }
      nextFrameToSend++;
    }

    if (time > 0)
    {
      for (int i = 0; i < N; i++)
      {
        if (sender[i].in_transit && sender[i].arrival_time == time)
        {
          if (scenario == 2 && i == 1 && !frameLostOnce[i])
          {
            printf("[Time %d] Frame %d lost in transit!\n", time, i);
            frameLostOnce[i] = true;
            sender[i].in_transit = false;
            continue;
          }

          if (receiver[i].received)
          {
            printf("[Time %d] Receiver: Duplicate Frame %d received -> Discarded\n", time, i);
          }
          else
          {
            receiver[i].received = true;
            printf("[Time %d] Receiver: Frame %d received -> Sending ACK %d\n", time, i, i);
          }

          sender[i].in_transit = false;
          receiver[i].acked = true;
          ack_arrival_time[i] = time + 1;
        }
      }
    }

    for (int i = 0; i < N; i++)
    {
      if (receiver[i].acked && !sender[i].acked && ack_arrival_time[i] == time)
      {
        if (scenario == 3 && i == 2 && !ackLostOnce[i])
        {
          printf("[Time %d] ACK %d lost in transit!\n", time, i);
          ackLostOnce[i] = true;
          continue;
        }
        sender[i].acked = true;
        sender[i].timer = 0;
        printf("[Time %d] Sender: ACK %d received -> Timer stopped\n", time, i);
        allAcked++;
      }
    }

    for (int i = 0; i < N; i++)
    {
      if (sender[i].sent && !sender[i].acked && sender[i].timer > 0)
      {
        sender[i].timer--;
        if (sender[i].timer == 0)
        {
          printf("[Time %d] Sender: Timeout! Retransmitting Frame %d -> Timer reset=%d\n",
                 time, i, timeout);
          sender[i].timer = timeout;
          sender[i].in_transit = true;
          sender[i].arrival_time = time + channel_delay;
          sender[i].transmissions++;
        }
      }
    }

    printf("[Time %d] ", time);
    for (int i = 0; i < N; i++)
    {
      if (sender[i].sent && !sender[i].acked)
      {
        printf("Timer[%d]=%d ", sender[i].id, sender[i].timer);
      }
    }
    printf("\n");

    printf("Receiver: Delivering frames in order -> ");
    for (int i = 0; i < N; i++)
    {
      if (receiver[i].received)
        printf("%d ", receiver[i].id);
      else
        break;
    }
    printf("\n");

    time++;
  }

  printf("\n--- Simulation Complete: All %d frames delivered successfully ---\n", N);
  return 0;
}
