#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

unsigned short calculate_checksum_binary(char *data);
unsigned short calculate_checksum_octal(char *data);
unsigned short calculate_checksum_hex(char *data);

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

  char octal_error[20];
  strcpy(octal_error, octal_data);
  octal_error[3] = '7';
  unsigned short cs2_err = calculate_checksum_octal(octal_error);
  printf("Octal data with error: %s\n", octal_error);
  printf("Checksum with error: %04X\n", cs2_err);
  printf("Verification: %s\n", (cs2 == cs2_err) ? "PASSED" : "FAILED (Error detected)");

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

int main(void)
{
  srand(time(NULL));
  checksum_demo();
  return 0;
}