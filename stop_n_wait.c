#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FRAMES 100

int isLost()
{
  return rand() % 2;
}

int main()
{
  int N, timeout, scenario;
  int frameNo = 0;
  int expectedAck = 1;
  int i = 0;

  srand(time(NULL));

  printf("Enter number of frames to send: ");
  scanf("%d", &N);

  printf("\nChoose transmission scenario:\n");
  printf("1. Successful Transmission (No Loss)\n");
  printf("2. Frame Loss\n");
  printf("3. ACK Loss\n");
  printf("Enter choice: ");
  scanf("%d", &scenario);

  printf("\nEnter Timeout value (in seconds): ");
  scanf("%d", &timeout);

  printf("\n--- Stop and Wait Simulation Started ---\n\n");

  while (i < N)
  {
    printf("Sender: Transmitting Frame %d [SeqNo=%d]\n", i + 1, frameNo);
    printf("Sender: Timer started for Frame %d (Timeout=%ds)\n", i + 1, timeout);

    int frameLost = 0, ackLost = 0;

    if (scenario == 2)
    {
      frameLost = isLost();
    }
    if (scenario == 3)
    {
      ackLost = isLost();
    }

    if (frameLost)
    {
      printf("!!! Frame %d Lost during transmission !!!\n", i + 1);
      printf("Sender: Timeout occurred. Retransmitting Frame %d...\n\n", i + 1);
      continue;
    }

    printf("Receiver: Frame %d received correctly.\n", i + 1);
    printf("Receiver: Sending ACK%d (Next expected SeqNo)\n", expectedAck);

    if (ackLost)
    {
      printf("!!! ACK%d Lost in transmission !!!\n", expectedAck);
      printf("Sender: Timeout occurred. Retransmitting Frame %d...\n\n", i + 1);
      continue;
    }

    printf("Sender: ACK%d received. Timer stopped.\n", expectedAck);

    frameNo = 1 - frameNo;
    expectedAck = 1 - expectedAck;
    i++;

    printf("-------------------------------------------------\n");
  }

  printf("\nAll %d frames transmitted and acknowledged successfully!\n", N);
  printf("--- Simulation Completed ---\n");

  return 0;
}