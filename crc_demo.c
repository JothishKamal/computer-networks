#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void generate_crc(char *data, char *generator, char *codeword);
int check_crc(char *received, char *generator);
void introduce_errors(char *data, int error_type);

void crc_demo()
{
  printf("\n=== CRC DEMO ===\n");

  char data[] = "1010101011001100";
  char generator[] = "11010101";
  char codeword[25], received[25];

  printf("Data word (16 bits): %s\n", data);
  printf("Generator polynomial (CRC-8): %s\n", generator);

  generate_crc(data, generator, codeword);
  printf("Generated codeword: %s\n", codeword);

  const char *test_names[] = {"No error", "Single bit error", "Odd number of errors", "Burst error"};

  for (int test = 0; test < 4; test++)
  {
    printf("\n--- Test Case %d: %s ---\n", test + 1, test_names[test]);
    strcpy(received, codeword);

    if (test > 0)
    {
      introduce_errors(received, test);
    }

    printf("Received: %s\n", received);
    int error_detected = check_crc(received, generator);
    printf("CRC Check: %s\n", error_detected ? "ERROR DETECTED" : "NO ERROR");
  }
}

void generate_crc(char *data, char *generator, char *codeword)
{
  int data_len = strlen(data), gen_len = strlen(generator);
  char temp[50];

  strcpy(temp, data);
  for (int i = 0; i < gen_len - 1; i++)
    strcat(temp, "0");

  for (int i = 0; i <= data_len - 1; i++)
  {
    if (temp[i] == '1')
    {
      for (int j = 0; j < gen_len; j++)
      {
        temp[i + j] = ((temp[i + j] - '0') ^ (generator[j] - '0')) + '0';
      }
    }
  }

  char remainder[20];
  strncpy(remainder, &temp[data_len], gen_len - 1);
  remainder[gen_len - 1] = '\0';

  printf("CRC remainder: %s\n", remainder);

  strcpy(codeword, data);
  strncat(codeword, remainder, gen_len - 1);
}

int check_crc(char *received, char *generator)
{
  int rec_len = strlen(received), gen_len = strlen(generator);
  char temp[50];
  strcpy(temp, received);

  for (int i = 0; i <= rec_len - gen_len; i++)
  {
    if (temp[i] == '1')
    {
      for (int j = 0; j < gen_len; j++)
      {
        temp[i + j] = ((temp[i + j] - '0') ^ (generator[j] - '0')) + '0';
      }
    }
  }

  char remainder[20];
  strncpy(remainder, &temp[rec_len - gen_len + 1], gen_len - 1);
  remainder[gen_len - 1] = '\0';

  printf("Check remainder: %s\n", remainder);

  for (int i = rec_len - gen_len + 1; i < rec_len; i++)
  {
    if (temp[i] == '1')
      return 1;
  }
  return 0;
}

void introduce_errors(char *data, int error_type)
{
  int len = strlen(data);
  switch (error_type)
  {
  case 1:
  {
    int pos = rand() % len;
    data[pos] = (data[pos] == '0') ? '1' : '0';
    printf("Single bit error at position %d\n", pos);
    break;
  }
  case 2:
  {
    printf("Introducing 3 random errors\n");
    for (int i = 0; i < 3; i++)
    {
      int pos = rand() % len;
      data[pos] = (data[pos] == '0') ? '1' : '0';
    }
    break;
  }
  case 3:
  {
    int burst_len = 4;
    int start = rand() % (len - burst_len);
    printf("Burst error from position %d to %d\n", start, start + burst_len - 1);
    for (int i = 0; i < burst_len; i++)
    {
      data[start + i] = (data[start + i] == '0') ? '1' : '0';
    }
    break;
  }
  }
}

int main(void)
{
  srand(time(NULL));
  crc_demo();
  return 0;
}