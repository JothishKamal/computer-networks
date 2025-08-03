#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

void hamming_code_demo();
void crc_demo();
void parity_2d_demo();
void checksum_demo();

void generate_hamming_code(char *data, char *codeword, int data_bits, int parity_type);
int detect_and_correct_hamming(char *received, int total_bits, int parity_type);
void print_binary(char *bits, int length);

void generate_crc(char *data, char *generator, char *codeword);
int check_crc(char *received, char *generator);
void xor_operation(char *a, char *b, int length);

void generate_2d_parity(char data[4][4], char parity_block[5][5]);
int check_2d_parity(char received[5][5], int *error_row, int *error_col);

unsigned short calculate_checksum_16bit(char *data, int length);
unsigned short calculate_checksum_octal(char *data);
unsigned short calculate_checksum_hex(char *data);

void introduce_error(char *data, int length, int error_type);
int power_of_2(int n);

int main()
{
  int choice;
  srand(time(NULL));

  printf("=== Error Detection and Correction Methods ===\n");
  printf("1. Hamming Code\n");
  printf("2. CRC (Cyclic Redundancy Check)\n");
  printf("3. 2D Parity Check\n");
  printf("4. Checksum\n");
  printf("5. Run All Demos\n");
  printf("Enter your choice: ");
  scanf("%d", &choice);

  switch (choice)
  {
  case 1:
    hamming_code_demo();
    break;
  case 2:
    crc_demo();
    break;
  case 3:
    parity_2d_demo();
    break;
  case 4:
    checksum_demo();
    break;
  case 5:
    printf("\n=== Running All Demos ===\n");
    hamming_code_demo();
    crc_demo();
    parity_2d_demo();
    checksum_demo();
    break;
  default:
    printf("Invalid choice!\n");
  }

  return 0;
}

void hamming_code_demo()
{
  printf("\n=== HAMMING CODE DEMO ===\n");

  printf("\n--- Demo 1: 11-bit data word ---\n");
  char data1[] = "00101101011";
  char codeword1[20];
  int data_bits1 = 11;

  printf("Given data word: %s\n", data1);
  generate_hamming_code(data1, codeword1, data_bits1, 0);
  printf("Generated codeword: %s\n", codeword1);

  printf("\n-- Error-free transmission --\n");
  char received1[20];
  strcpy(received1, codeword1);
  printf("Received codeword: %s\n", received1);
  int error_pos1 = detect_and_correct_hamming(received1, strlen(received1), 0);
  if (error_pos1 == 0)
  {
    printf("No error detected.\n");
  }

  printf("\n-- Error at position 7 --\n");
  strcpy(received1, codeword1);
  int total_len = strlen(received1);
  received1[total_len - 7] = (received1[total_len - 7] == '0') ? '1' : '0';
  printf("Received codeword with error: %s\n", received1);
  error_pos1 = detect_and_correct_hamming(received1, strlen(received1), 0);
  printf("Corrected codeword: %s\n", received1);

  printf("Original data recovered: ");
  int r = 0;
  while ((1 << r) < total_len)
    r++;

  int data_idx = 0;
  char recovered_data[20];
  for (int i = 1; i <= total_len; i++)
  {
    int is_parity_pos = 0;
    for (int j = 0; j < r; j++)
    {
      if (i == (1 << j))
      {
        is_parity_pos = 1;
        break;
      }
    }
    if (!is_parity_pos)
    {
      recovered_data[data_idx++] = received1[total_len - i];
    }
  }
  recovered_data[data_idx] = '\0';
  printf("%s\n", recovered_data);

  printf("\n--- Demo 2: 4-bit message ---\n");
  char data2[] = "1101";
  char codeword2[10];

  printf("Input the data word: %s\n", data2);
  printf("Input parity(odd-1/even-0): 0\n");
  printf("Enter the choice(Error-1/ Errorless transmission-0): 1\n");
  printf("Introduce Error: Random\n\n");

  printf("Sender side:\n");
  printf("Given Dataword: %s\n", data2);
  generate_hamming_code(data2, codeword2, 4, 0);
  printf("Codeword: %s\n", codeword2);

  printf("\nReceiver side:\n");
  char received2[10];
  strcpy(received2, codeword2);

  int error_bit = rand() % strlen(received2);
  received2[error_bit] = (received2[error_bit] == '0') ? '1' : '0';
  printf("Codeword: %s (Error in position %d)\n", received2, strlen(received2) - error_bit);

  int error_pos2 = detect_and_correct_hamming(received2, strlen(received2), 0);
  printf("Result of verification logic: Error in bit position %d\n", error_pos2);
  printf("Code word after correction: %s\n", received2);

  char recovered_data2[5];
  int r2 = 0;
  while ((1 << r2) < strlen(received2))
    r2++;

  int data_idx2 = 0;
  for (int i = 1; i <= strlen(received2); i++)
  {
    int is_parity_pos = 0;
    for (int j = 0; j < r2; j++)
    {
      if (i == (1 << j))
      {
        is_parity_pos = 1;
        break;
      }
    }
    if (!is_parity_pos)
    {
      recovered_data2[data_idx2++] = received2[strlen(received2) - i];
    }
  }
  recovered_data2[data_idx2] = '\0';
  printf("Data word: %s\n", recovered_data2);
}

void generate_hamming_code(char *data, char *codeword, int data_bits, int parity_type)
{
  int r = 0;
  while ((1 << r) < data_bits + r + 1)
    r++;

  int total_bits = data_bits + r;

  for (int i = 0; i < total_bits; i++)
  {
    codeword[i] = '0';
  }
  codeword[total_bits] = '\0';

  int data_index = 0;
  for (int i = 1; i <= total_bits; i++)
  {

    int is_parity_pos = 0;
    for (int j = 0; j < r; j++)
    {
      if (i == (1 << j))
      {
        is_parity_pos = 1;
        break;
      }
    }

    if (!is_parity_pos && data_index < data_bits)
    {
      codeword[total_bits - i] = data[data_index];
      data_index++;
    }
  }

  for (int i = 0; i < r; i++)
  {
    int parity = 0;
    int parity_pos = 1 << i;

    for (int j = 1; j <= total_bits; j++)
    {
      if ((j & parity_pos) && codeword[total_bits - j] == '1')
      {
        parity ^= 1;
      }
    }

    if (parity_type == 1)
      parity ^= 1;
    codeword[total_bits - parity_pos] = (parity == 0) ? '0' : '1';
  }

  printf("Parity bits: ");
  for (int i = r - 1; i >= 0; i--)
  {
    int pos = 1 << i;
    int bit_val = codeword[total_bits - pos] - '0';
    printf("R%d-%d", pos, bit_val);
    if (i > 0)
      printf(", ");
  }
  printf("\n");
}

int detect_and_correct_hamming(char *received, int total_bits, int parity_type)
{
  int r = 0;
  while ((1 << r) < total_bits)
    r++;

  int error_pos = 0;

  for (int i = 0; i < r; i++)
  {
    int parity = 0;
    int parity_pos = 1 << i;

    for (int j = 1; j <= total_bits; j++)
    {
      if ((j & parity_pos) && received[total_bits - j] == '1')
      {
        parity ^= 1;
      }
    }

    if (parity_type == 1)
      parity ^= 1;

    if (parity != 0)
    {
      error_pos += parity_pos;
    }
  }

  printf("Parity bits: ");
  for (int i = r - 1; i >= 0; i--)
  {
    int parity = 0;
    int parity_pos = 1 << i;

    for (int j = 1; j <= total_bits; j++)
    {
      if ((j & parity_pos) && received[total_bits - j] == '1')
      {
        parity ^= 1;
      }
    }

    if (parity_type == 1)
      parity ^= 1;

    printf("C%d-%d", i + 1, parity);
    if (i > 0)
      printf(", ");
  }
  printf("\n");

  if (error_pos != 0)
  {

    received[total_bits - error_pos] = (received[total_bits - error_pos] == '0') ? '1' : '0';
  }

  return error_pos;
}

void crc_demo()
{
  printf("\n=== CRC DEMO ===\n");

  char data[17] = "1010101011001100";
  char generator[9] = "11010101";
  char codeword[25];

  printf("Data word (16 bits): %s\n", data);
  printf("Generator polynomial (CRC-8): %s\n", generator);

  generate_crc(data, generator, codeword);
  printf("Generated codeword: %s\n", codeword);

  printf("\n--- Test Case 1: No error ---\n");
  char received1[25];
  strcpy(received1, codeword);
  printf("Received: %s\n", received1);
  if (check_crc(received1, generator) == 0)
  {
    printf("No error detected.\n");
  }
  else
  {
    printf("Error detected!\n");
  }

  printf("\n--- Test Case 2: Single bit error ---\n");
  char received2[25];
  strcpy(received2, codeword);
  int error_pos = rand() % strlen(received2);
  received2[error_pos] = (received2[error_pos] == '0') ? '1' : '0';
  printf("Received with error at position %d: %s\n", error_pos, received2);
  if (check_crc(received2, generator) == 0)
  {
    printf("No error detected.\n");
  }
  else
  {
    printf("Error detected!\n");
  }

  printf("\n--- Test Case 3: Odd number of errors ---\n");
  char received3[25];
  strcpy(received3, codeword);

  for (int i = 0; i < 3; i++)
  {
    int pos = rand() % strlen(received3);
    received3[pos] = (received3[pos] == '0') ? '1' : '0';
  }
  printf("Received with 3 errors: %s\n", received3);
  if (check_crc(received3, generator) == 0)
  {
    printf("No error detected.\n");
  }
  else
  {
    printf("Error detected!\n");
  }

  printf("\n--- Test Case 4: Burst error ---\n");
  char received4[25];
  strcpy(received4, codeword);

  int burst_start = rand() % (strlen(received4) - 4);
  for (int i = 0; i < 4; i++)
  {
    received4[burst_start + i] = (received4[burst_start + i] == '0') ? '1' : '0';
  }
  printf("Received with burst error: %s\n", received4);
  if (check_crc(received4, generator) == 0)
  {
    printf("No error detected.\n");
  }
  else
  {
    printf("Error detected!\n");
  }
}

void generate_crc(char *data, char *generator, char *codeword)
{
  int data_len = strlen(data);
  int gen_len = strlen(generator);
  char temp[50];

  strcpy(temp, data);
  for (int i = 0; i < gen_len - 1; i++)
  {
    strcat(temp, "0");
  }

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

  strcpy(codeword, data);
  strncat(codeword, &temp[data_len], gen_len - 1);
}

int check_crc(char *received, char *generator)
{
  int rec_len = strlen(received);
  int gen_len = strlen(generator);
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

  for (int i = rec_len - gen_len + 1; i < rec_len; i++)
  {
    if (temp[i] == '1')
      return 1;
  }
  return 0;
}

void xor_operation(char *a, char *b, int length)
{
  for (int i = 0; i < length; i++)
  {
    a[i] = ((a[i] - '0') ^ (b[i] - '0')) + '0';
  }
}

void parity_2d_demo()
{
  printf("\n=== 2D PARITY DEMO ===\n");

  char data[4][4];
  char parity_block[5][5];

  printf("Original 4x4 data block:\n");
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      data[i][j] = (rand() % 2) + '0';
      printf("%c ", data[i][j]);
    }
    printf("\n");
  }

  generate_2d_parity(data, parity_block);

  printf("\nData block with parity bits:\n");
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      printf("%c ", parity_block[i][j]);
    }
    printf("\n");
  }

  printf("\n--- Test Case 1: No error ---\n");
  char received1[5][5];
  memcpy(received1, parity_block, sizeof(parity_block));
  int error_row, error_col;
  if (check_2d_parity(received1, &error_row, &error_col) == 0)
  {
    printf("No error detected.\n");
  }

  printf("\n--- Test Case 2: Single bit error ---\n");
  char received2[5][5];
  memcpy(received2, parity_block, sizeof(parity_block));
  int err_r = rand() % 4, err_c = rand() % 4;
  received2[err_r][err_c] = (received2[err_r][err_c] == '0') ? '1' : '0';
  printf("Error introduced at position (%d,%d)\n", err_r, err_c);

  printf("Received block:\n");
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      printf("%c ", received2[i][j]);
    }
    printf("\n");
  }

  if (check_2d_parity(received2, &error_row, &error_col) > 0)
  {
    printf("Error detected and corrected at position (%d,%d)\n", error_row, error_col);
  }

  printf("\n--- Test Case 3: Two errors in same row ---\n");
  char received3[5][5];
  memcpy(received3, parity_block, sizeof(parity_block));
  int same_row = rand() % 4;
  int col1 = rand() % 4, col2;
  do
  {
    col2 = rand() % 4;
  } while (col2 == col1);

  received3[same_row][col1] = (received3[same_row][col1] == '0') ? '1' : '0';
  received3[same_row][col2] = (received3[same_row][col2] == '0') ? '1' : '0';
  printf("Two errors in row %d at columns %d and %d\n", same_row, col1, col2);

  if (check_2d_parity(received3, &error_row, &error_col) == 0)
  {
    printf("Errors not detected (limitation of 2D parity).\n");
  }
  else
  {
    printf("Error detected.\n");
  }
}

void generate_2d_parity(char data[4][4], char parity_block[5][5])
{

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      parity_block[i][j] = data[i][j];
    }
  }

  for (int i = 0; i < 4; i++)
  {
    int parity = 0;
    for (int j = 0; j < 4; j++)
    {
      if (data[i][j] == '1')
        parity ^= 1;
    }
    parity_block[i][4] = (parity == 0) ? '0' : '1';
  }

  for (int j = 0; j < 4; j++)
  {
    int parity = 0;
    for (int i = 0; i < 4; i++)
    {
      if (data[i][j] == '1')
        parity ^= 1;
    }
    parity_block[4][j] = (parity == 0) ? '0' : '1';
  }

  int overall_parity = 0;
  for (int i = 0; i < 4; i++)
  {
    if (parity_block[i][4] == '1')
      overall_parity ^= 1;
  }
  parity_block[4][4] = (overall_parity == 0) ? '0' : '1';
}

int check_2d_parity(char received[5][5], int *error_row, int *error_col)
{
  int row_errors = 0, col_errors = 0;
  *error_row = -1;
  *error_col = -1;

  for (int i = 0; i < 4; i++)
  {
    int parity = 0;
    for (int j = 0; j < 5; j++)
    {
      if (received[i][j] == '1')
        parity ^= 1;
    }
    if (parity != 0)
    {
      row_errors++;
      *error_row = i;
    }
  }

  for (int j = 0; j < 4; j++)
  {
    int parity = 0;
    for (int i = 0; i < 5; i++)
    {
      if (received[i][j] == '1')
        parity ^= 1;
    }
    if (parity != 0)
    {
      col_errors++;
      *error_col = j;
    }
  }

  if (row_errors == 1 && col_errors == 1)
  {

    received[*error_row][*error_col] = (received[*error_row][*error_col] == '0') ? '1' : '0';
    return 1;
  }
  else if (row_errors > 0 || col_errors > 0)
  {
    return 2;
  }

  return 0;
}

void checksum_demo()
{
  printf("\n=== CHECKSUM DEMO ===\n");

  printf("\n--- Test Case 1: 16-bit binary string ---\n");
  char binary_data[] = "1010101011001100";
  printf("Data: %s\n", binary_data);

  unsigned short checksum1 = calculate_checksum_16bit(binary_data, strlen(binary_data));
  printf("Calculated checksum: %04X\n", checksum1);

  char received_binary[20];
  strcpy(received_binary, binary_data);
  unsigned short received_checksum1 = calculate_checksum_16bit(received_binary, strlen(received_binary));
  printf("Received checksum (no error): %04X\n", received_checksum1);
  if (checksum1 == received_checksum1)
  {
    printf("Checksum verification: PASSED\n");
  }
  else
  {
    printf("Checksum verification: FAILED\n");
  }

  received_binary[5] = (received_binary[5] == '0') ? '1' : '0';
  received_checksum1 = calculate_checksum_16bit(received_binary, strlen(received_binary));
  printf("Received data with error: %s\n", received_binary);
  printf("Received checksum (with error): %04X\n", received_checksum1);
  if (checksum1 == received_checksum1)
  {
    printf("Checksum verification: PASSED\n");
  }
  else
  {
    printf("Checksum verification: FAILED (Error detected)\n");
  }

  printf("\n--- Test Case 2: 10 octets of octal data ---\n");
  char octal_data[] = "1234567012";
  printf("Octal data: %s\n", octal_data);

  unsigned short checksum2 = calculate_checksum_octal(octal_data);
  printf("Calculated checksum: %04X\n", checksum2);

  printf("\n--- Test Case 3: 16 octets of hexadecimal data ---\n");
  char hex_data[] = "123456789ABCDEF0";
  printf("Hex data: %s\n", hex_data);

  unsigned short checksum3 = calculate_checksum_hex(hex_data);
  printf("Calculated checksum: %04X\n", checksum3);

  char received_hex[20];
  strcpy(received_hex, hex_data);
  received_hex[3] = 'F';
  unsigned short received_checksum3 = calculate_checksum_hex(received_hex);
  printf("Received hex with error: %s\n", received_hex);
  printf("Received checksum (with error): %04X\n", received_checksum3);
  if (checksum3 == received_checksum3)
  {
    printf("Checksum verification: PASSED\n");
  }
  else
  {
    printf("Checksum verification: FAILED (Error detected)\n");
  }
}

unsigned short calculate_checksum_16bit(char *data, int length)
{
  unsigned long sum = 0;

  for (int i = 0; i < length; i += 16)
  {
    unsigned short chunk = 0;
    for (int j = 0; j < 16 && i + j < length; j++)
    {
      if (data[i + j] == '1')
      {
        chunk |= (1 << (15 - j));
      }
    }
    sum += chunk;
  }

  while (sum >> 16)
  {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }

  return ~sum;
}

unsigned short calculate_checksum_octal(char *data)
{
  unsigned long sum = 0;
  int length = strlen(data);

  for (int i = 0; i < length; i += 2)
  {
    unsigned short chunk = 0;

    if (i < length)
      chunk += (data[i] - '0') * 8;
    if (i + 1 < length)
      chunk += (data[i + 1] - '0');
    sum += chunk;
  }

  while (sum >> 16)
  {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }

  return ~sum;
}

unsigned short calculate_checksum_hex(char *data)
{
  unsigned long sum = 0;
  int length = strlen(data);

  for (int i = 0; i < length; i += 4)
  {
    unsigned short chunk = 0;

    for (int j = 0; j < 4 && i + j < length; j++)
    {
      char c = data[i + j];
      unsigned char val;
      if (c >= '0' && c <= '9')
        val = c - '0';
      else if (c >= 'A' && c <= 'F')
        val = c - 'A' + 10;
      else if (c >= 'a' && c <= 'f')
        val = c - 'a' + 10;
      else
        val = 0;

      chunk = (chunk << 4) | val;
    }
    sum += chunk;
  }

  while (sum >> 16)
  {
    sum = (sum & 0xFFFF) + (sum >> 16);
  }

  return ~sum;
}

int power_of_2(int n)
{
  return (int)pow(2, n);
}

void introduce_error(char *data, int length, int error_type)
{
  switch (error_type)
  {
  case 1:
  {
    int pos = rand() % length;
    data[pos] = (data[pos] == '0') ? '1' : '0';
  }
  break;
  case 2:
  {
    int num_errors = 2 + rand() % 3;
    for (int i = 0; i < num_errors; i++)
    {
      int pos = rand() % length;
      data[pos] = (data[pos] == '0') ? '1' : '0';
    }
  }
  break;
  case 3:
  {
    int burst_length = 3 + rand() % 3;
    int start_pos = rand() % (length - burst_length);
    for (int i = 0; i < burst_length; i++)
    {
      data[start_pos + i] = (data[start_pos + i] == '0') ? '1' : '0';
    }
  }
  break;
  }
}

void print_binary(char *bits, int length)
{
  for (int i = 0; i < length; i++)
  {
    printf("%c", bits[i]);
  }
  printf("\n");
}