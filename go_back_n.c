#include <stdio.h>
#include <stdlib.h>

#define NORMAL 1
#define LOST_FRAME 2
#define LOST_ACK 3

void go_back_n(int N, int W, int scenario, int timeout)
{
  int time = 0, base = 0, next_frame = 0, timer = -1, delivered = 0;
  int ack_lost_once = 0, frame_lost_once = 0;
  int send_time[100], acked[100] = {0};

  printf("\n--- Go-Back-N Sliding Window Protocol Simulation ---\n");

  while (delivered < N)
  {
    if (next_frame < base + W && next_frame < N)
    {
      printf("[Time %d] Sender: Sending Frame %d\n", time, next_frame);
      send_time[next_frame] = time;
      if (base == next_frame && timer == -1)
      {
        timer = timeout;
        printf("[Time %d] Sender: Timer started for Frame %d (T=%d)\n", time, base, timeout);
      }
      next_frame++;
    }

    for (int i = base; i < next_frame; i++)
    {
      if (!acked[i] && time >= send_time[i] + 2)
      {
        if (scenario == LOST_FRAME && i == 2 && !frame_lost_once)
        {
          printf("[Time %d] Receiver: Frame %d lost in transit\n", time, i);
          printf("[Time %d] Receiver: Sending ACK %d (last in-order)\n", time, i);
          frame_lost_once = 1;
          continue;
        }
        if (scenario == LOST_ACK && i == W - 1 && !ack_lost_once)
        {
          printf("[Time %d] Receiver: Frame %d received -> Sending ACK %d\n", time, i, i + 1);
          printf("(ACK %d lost in transit)\n", i + 1);
          ack_lost_once = 1;
          continue;
        }
        printf("[Time %d] Receiver: Frame %d received -> Sending ACK %d\n", time, i, i + 1);
        printf("[Time %d] Sender: ACK %d received -> Timer stopped\n", time, i + 1);
        acked[i] = 1;
        while (base < N && acked[base])
          base++;
        delivered = base;
        timer = -1;
        if (base < next_frame)
        {
          timer = timeout;
          printf("[Time %d] Sender: Timer restarted for Frame %d (T=%d)\n", time, base, timeout);
        }
      }
    }

    if (timer > 0)
    {
      timer--;
      printf("[Time %d] Timer = %d\n", time, timer);
    }
    else if (timer == 0)
    {
      printf("[Time %d] Sender: Timeout -> Retransmitting Frames %d to %d\n", time, base, next_frame - 1);
      for (int i = base; i < next_frame; i++)
        send_time[i] = time;
      timer = timeout;
    }

    time++;
  }

  printf("All %d frames successfully delivered.\n", N);
}

int main()
{
  int N, W, scenario, timeout;
  printf("Enter number of frames to send: ");
  scanf("%d", &N);
  printf("Enter window size: ");
  scanf("%d", &W);
  printf("Choose Transmission Scenario:\n");
  printf("1. Normal (No Loss)\n2. Lost Frame\n3. Lost ACK\n");
  printf("Enter choice: ");
  scanf("%d", &scenario);
  printf("Enter timeout value: ");
  scanf("%d", &timeout);
  go_back_n(N, W, scenario, timeout);
  return 0;
}