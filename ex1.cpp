#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

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

  strcpy(codeword, data);
  strncat(codeword, &temp[data_len], gen_len - 1);
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

void generate_2d_parity(char data[4][4], char parity[5][5])
{

  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      parity[i][j] = data[i][j];
    }
  }

  for (int i = 0; i < 4; i++)
  {
    int count = 0;
    for (int j = 0; j < 4; j++)
    {
      if (data[i][j] == '1')
        count++;
    }
    parity[i][4] = (count % 2) ? '1' : '0';
  }

  for (int j = 0; j < 4; j++)
  {
    int count = 0;
    for (int i = 0; i < 4; i++)
    {
      if (data[i][j] == '1')
        count++;
    }
    parity[4][j] = (count % 2) ? '1' : '0';
  }

  int count = 0;
  for (int i = 0; i < 4; i++)
  {
    if (parity[i][4] == '1')
      count++;
  }
  parity[4][4] = (count % 2) ? '1' : '0';
}

int check_2d_parity(char received[5][5], int *err_row, int *err_col)
{
  *err_row = *err_col = -1;
  int row_errors = 0, col_errors = 0;

  for (int i = 0; i < 4; i++)
  {
    int count = 0;
    for (int j = 0; j < 5; j++)
    {
      if (received[i][j] == '1')
        count++;
    }
    if (count % 2)
    {
      row_errors++;
      *err_row = i;
    }
  }

  for (int j = 0; j < 4; j++)
  {
    int count = 0;
    for (int i = 0; i < 5; i++)
    {
      if (received[i][j] == '1')
        count++;
    }
    if (count % 2)
    {
      col_errors++;
      *err_col = j;
    }
  }

  if (row_errors == 1 && col_errors == 1)
  {
    received[*err_row][*err_col] =
        (received[*err_row][*err_col] == '0') ? '1' : '0';
    return 1;
  }

  return (row_errors > 0 || col_errors > 0) ? 2 : 0;
}

void print_2d_block(char block[5][5])
{
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      printf("%c ", block[i][j]);
    }
    printf("\n");
  }
}

void parity_2d_demo()
{
  printf("\n=== 2D PARITY DEMO ===\n");

  char data[4][4], parity[5][5], received[5][5];

  printf("Original 4x4 data block (16 bits):\n");
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      data[i][j] = (rand() % 2) + '0';
      printf("%c ", data[i][j]);
    }
    printf("\n");
  }

  generate_2d_parity(data, parity);

  printf("\nData block with parity bits:\n");
  print_2d_block(parity);

  printf("\n--- Test Case 1: No error ---\n");
  memcpy(received, parity, sizeof(parity));
  int err_row, err_col;
  int result = check_2d_parity(received, &err_row, &err_col);
  printf("Result: %s\n", (result == 0) ? "NO ERROR" : "ERROR DETECTED");

  printf("\n--- Test Case 2: Single bit error ---\n");
  memcpy(received, parity, sizeof(parity));
  int r = rand() % 4, c = rand() % 4;
  received[r][c] = (received[r][c] == '0') ? '1' : '0';
  printf("Error introduced at (%d,%d)\n", r, c);
  print_2d_block(received);
  result = check_2d_parity(received, &err_row, &err_col);
  if (result == 1)
  {
    printf("Error detected and corrected at (%d,%d)\n", err_row, err_col);
  }

  printf("\n--- Test Case 3: Two errors in same row ---\n");
  memcpy(received, parity, sizeof(parity));
  int same_row = rand() % 4;
  int col1 = rand() % 4, col2;
  do
  {
    col2 = rand() % 4;
  } while (col2 == col1);
  received[same_row][col1] = (received[same_row][col1] == '0') ? '1' : '0';
  received[same_row][col2] = (received[same_row][col2] == '0') ? '1' : '0';
  printf("Two errors in row %d at columns %d and %d\n", same_row, col1, col2);
  result = check_2d_parity(received, &err_row, &err_col);
  printf("Result: %s\n", (result == 0) ? "ERRORS NOT DETECTED" : "ERRORS DETECTED");

  printf("\n--- Test Case 4: Three errors in different rows/columns ---\n");
  memcpy(received, parity, sizeof(parity));
  for (int i = 0; i < 3; i++)
  {
    int r = rand() % 4, c = rand() % 4;
    received[r][c] = (received[r][c] == '0') ? '1' : '0';
  }
  result = check_2d_parity(received, &err_row, &err_col);
  printf("Result: %s\n", (result > 0) ? "ERRORS DETECTED" : "ERRORS NOT DETECTED");

  printf("\n--- Test Case 5: Four errors in rectangle pattern ---\n");
  memcpy(received, parity, sizeof(parity));

  received[0][0] = (received[0][0] == '0') ? '1' : '0';
  received[0][1] = (received[0][1] == '0') ? '1' : '0';
  received[1][0] = (received[1][0] == '0') ? '1' : '0';
  received[1][1] = (received[1][1] == '0') ? '1' : '0';
  printf("Four errors in rectangle pattern at (0,0), (0,1), (1,0), (1,1)\n");
  result = check_2d_parity(received, &err_row, &err_col);
  printf("Result: %s\n", (result == 0) ? "ERRORS NOT DETECTED (Rectangle pattern limitation)" : "ERRORS DETECTED");
}

unsigned short calculate_checksum_binary(char *data)
{
  unsigned long sum = 0;
  int len = strlen(data);

  for (int i = 0; i < len; i += 16)
  {
    unsigned short chunk = 0;
    for (int j = 0; j < 16 && i + j < len; j++)
    {
      if (data[i + j] == '1')
        chunk |= (1 << (15 - j));
    }
    sum += chunk;
  }

  while (sum >> 16)
    sum = (sum & 0xFFFF) + (sum >> 16);
  return ~sum;
}

unsigned short calculate_checksum_octal(char *data)
{
  unsigned long sum = 0;
  int len = strlen(data);

  for (int i = 0; i < len; i += 2)
  {
    unsigned short chunk = 0;
    if (i < len)
      chunk += (data[i] - '0') * 8;
    if (i + 1 < len)
      chunk += (data[i + 1] - '0');
    sum += chunk;
  }

  while (sum >> 16)
    sum = (sum & 0xFFFF) + (sum >> 16);
  return ~sum;
}

unsigned short calculate_checksum_hex(char *data)
{
  unsigned long sum = 0;
  int len = strlen(data);

  for (int i = 0; i < len; i += 4)
  {
    unsigned short chunk = 0;
    for (int j = 0; j < 4 && i + j < len; j++)
    {
      char c = data[i + j];
      unsigned char val = (c >= '0' && c <= '9') ? c - '0' : (c >= 'A' && c <= 'F') ? c - 'A' + 10
                                                         : (c >= 'a' && c <= 'f')   ? c - 'a' + 10
                                                                                    : 0;
      chunk = (chunk << 4) | val;
    }
    sum += chunk;
  }

  while (sum >> 16)
    sum = (sum & 0xFFFF) + (sum >> 16);
  return ~sum;
}

void checksum_demo()
{
  printf("\n=== CHECKSUM DEMO ===\n");

  printf("\n--- Test Case 1: 16-bit binary string ---\n");
  char binary_data[] = "1010101011001100";
  printf("Data: %s\n", binary_data);
  unsigned short cs1 = calculate_checksum_binary(binary_data);
  printf("Calculated checksum: %04X\n", cs1);

  char binary_error[20];
  strcpy(binary_error, binary_data);
  binary_error[5] = (binary_error[5] == '0') ? '1' : '0';
  unsigned short cs1_err = calculate_checksum_binary(binary_error);
  printf("Data with error: %s\n", binary_error);
  printf("Checksum with error: %04X\n", cs1_err);
  printf("Verification: %s\n", (cs1 == cs1_err) ? "PASSED" : "FAILED (Error detected)");

  printf("\n--- Test Case 2: 10 octets of octal data ---\n");
  char octal_data[] = "1234567012";
  printf("Octal data: %s\n", octal_data);
  unsigned short cs2 = calculate_checksum_octal(octal_data);
  printf("Calculated checksum: %04X\n", cs2);

  printf("\n--- Test Case 3: 16 octets of hexadecimal data ---\n");
  char hex_data[] = "123456789ABCDEF0";
  printf("Hex data: %s\n", hex_data);
  unsigned short cs3 = calculate_checksum_hex(hex_data);
  printf("Calculated checksum: %04X\n", cs3);

  char hex_error[20];
  strcpy(hex_error, hex_data);
  hex_error[3] = 'F';
  unsigned short cs3_err = calculate_checksum_hex(hex_error);
  printf("Hex data with error: %s\n", hex_error);
  printf("Checksum with error: %04X\n", cs3_err);
  printf("Verification: %s\n", (cs3 == cs3_err) ? "PASSED" : "FAILED (Error detected)");
}

int main()
{
  srand(time(NULL));

  int choice;
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
    hamming_demo();
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
    hamming_demo();
    crc_demo();
    parity_2d_demo();
    checksum_demo();
    break;
  default:
    printf("Invalid choice!\n");
  }

  return 0;
}