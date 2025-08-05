#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generate_hamming(char *data, char *codeword, int data_bits, int parity_type);
int detect_correct_hamming(char *received, int total_bits, int parity_type);
void extract_data(char *codeword, char *data, int total_bits);

void hamming_demo()
{
  printf("\n=== HAMMING CODE DEMO ===\n");

  printf("\n--- Demo 1: 11-bit data word ---\n");
  char data1[] = "00101101011";
  char codeword1[20], received1[20];

  printf("Given data word: %s\n", data1);
  generate_hamming(data1, codeword1, 11, 0);
  printf("Generated codeword: %s\n", codeword1);

  printf("\n-- Error-free transmission --\n");
  strcpy(received1, codeword1);
  printf("Received codeword: %s\n", received1);
  int error_pos = detect_correct_hamming(received1, strlen(received1), 0);
  if (error_pos == 0)
    printf("No error detected.\n");

  printf("\n-- Error at position 7 --\n");
  strcpy(received1, codeword1);
  int total_len = strlen(received1);
  received1[total_len - 7] = (received1[total_len - 7] == '0') ? '1' : '0';
  printf("Received codeword with error: %s\n", received1);
  error_pos = detect_correct_hamming(received1, strlen(received1), 0);
  printf("Corrected codeword: %s\n", received1);

  char recovered[20];
  extract_data(received1, recovered, strlen(received1));
  printf("Original data recovered: %s\n", recovered);

  printf("\n--- Demo 2: 4-bit message ---\n");
  char data2[] = "1101";
  char codeword2[10], received2[10];
  int parity_choice = 0, error_choice = 1;

  printf("Input the data word: %s\n", data2);
  printf("Input parity(odd-1/even-0): %d\n", parity_choice);
  printf("Enter the choice(Error-1/ Errorless transmission-0): %d\n", error_choice);
  printf("Introduce Error: Random\n\n");

  printf("Sender side:\n");
  printf("Given Dataword: %s\n", data2);
  generate_hamming(data2, codeword2, 4, parity_choice);
  printf("Codeword: %s\n", codeword2);

  printf("\nReceiver side:\n");
  strcpy(received2, codeword2);

  if (error_choice == 1)
  {
    int error_bit = rand() % strlen(received2);
    received2[error_bit] = (received2[error_bit] == '0') ? '1' : '0';
    printf("Codeword: %s (Error in position %d)\n", received2, strlen(received2) - error_bit);

    int error_pos = detect_correct_hamming(received2, strlen(received2), parity_choice);
    printf("Result of verification logic: Error in bit position %d\n", error_pos);
    printf("Code word after correction: %s\n", received2);

    char recovered_data[10];
    extract_data(received2, recovered_data, strlen(received2));
    printf("Data word: %s\n", recovered_data);
  }
}

void generate_hamming(char *data, char *codeword, int data_bits, int parity_type)
{
  int r = 0;
  while ((1 << r) < data_bits + r + 1)
    r++;

  int total = data_bits + r;
  memset(codeword, '0', total);
  codeword[total] = '\0';

  int data_idx = 0;
  for (int i = 1; i <= total; i++)
  {
    int is_parity = 0;
    for (int j = 0; j < r; j++)
    {
      if (i == (1 << j))
      {
        is_parity = 1;
        break;
      }
    }
    if (!is_parity && data_idx < data_bits)
    {
      codeword[total - i] = data[data_idx++];
    }
  }

  for (int i = 0; i < r; i++)
  {
    int parity = 0, pos = 1 << i;
    for (int j = 1; j <= total; j++)
    {
      if ((j & pos) && codeword[total - j] == '1')
        parity ^= 1;
    }
    if (parity_type == 1)
      parity ^= 1;
    codeword[total - pos] = parity ? '1' : '0';
  }

  printf("Parity bits: ");
  for (int i = r - 1; i >= 0; i--)
  {
    int pos = 1 << i;
    printf("R%d-%c", pos, codeword[total - pos]);
    if (i > 0)
      printf(", ");
  }
  printf("\n");
}

int detect_correct_hamming(char *received, int total_bits, int parity_type)
{
  int r = 0;
  while ((1 << r) < total_bits)
    r++;

  int error_pos = 0;
  int parity_results[10];

  for (int i = 0; i < r; i++)
  {
    int parity = 0, pos = 1 << i;
    for (int j = 1; j <= total_bits; j++)
    {
      if ((j & pos) && received[total_bits - j] == '1')
        parity ^= 1;
    }
    if (parity_type == 1)
      parity ^= 1;
    parity_results[i] = parity;
    if (parity)
      error_pos += pos;
  }

  printf("Parity bits: ");
  for (int i = r - 1; i >= 0; i--)
  {
    printf("C%d-%d", i + 1, parity_results[i]);
    if (i > 0)
      printf(", ");
  }
  printf("\n");

  if (error_pos)
  {
    received[total_bits - error_pos] =
        (received[total_bits - error_pos] == '0') ? '1' : '0';
  }

  return error_pos;
}

void extract_data(char *codeword, char *data, int total_bits)
{
  int r = 0;
  while ((1 << r) < total_bits)
    r++;

  int data_idx = 0;
  for (int i = 1; i <= total_bits; i++)
  {
    int is_parity = 0;
    for (int j = 0; j < r; j++)
    {
      if (i == (1 << j))
      {
        is_parity = 1;
        break;
      }
    }
    if (!is_parity)
    {
      data[data_idx++] = codeword[total_bits - i];
    }
  }
  data[data_idx] = '\0';
}

int main(void){
  hamming_demo();
  return 0;
}