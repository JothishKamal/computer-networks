#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void generate_2d_parity(char data[4][4], char parity[5][5]);
int check_2d_parity(char received[5][5], int *err_row, int *err_col);
void print_2d_block(char block[5][5]);

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
  printf("Received matrix:\n");
  print_2d_block(received);

  int err_row, err_col;
  int result = check_2d_parity(received, &err_row, &err_col);
  printf("Result: %s\n", (result == 0) ? "NO ERROR" : "ERROR DETECTED");

  printf("\n--- Test Case 2: Single bit error ---\n");
  memcpy(received, parity, sizeof(parity));
  int r = rand() % 4, c = rand() % 4;
  printf("Error introduced at (%d,%d)\n", r, c);
  received[r][c] = (received[r][c] == '0') ? '1' : '0';

  printf("Received matrix with error:\n");
  print_2d_block(received);

  result = check_2d_parity(received, &err_row, &err_col);
  if (result == 1)
  {
    printf("Error detected and corrected at (%d,%d)\n", err_row, err_col);
    printf("Corrected matrix:\n");
    print_2d_block(received);
  }

  printf("\n--- Test Case 3: Two errors in same row ---\n");
  memcpy(received, parity, sizeof(parity));
  int same_row = rand() % 4;
  int col1 = rand() % 4, col2;
  do
  {
    col2 = rand() % 4;
  } while (col2 == col1);

  printf("Two errors in row %d at columns %d and %d\n", same_row, col1, col2);
  received[same_row][col1] = (received[same_row][col1] == '0') ? '1' : '0';
  received[same_row][col2] = (received[same_row][col2] == '0') ? '1' : '0';

  printf("Received matrix with errors:\n");
  print_2d_block(received);

  result = check_2d_parity(received, &err_row, &err_col);
  printf("Result: %s\n", (result == 0) ? "ERRORS NOT DETECTED" : "ERRORS DETECTED");

  printf("\n--- Test Case 4: Three errors in different rows/columns ---\n");
  memcpy(received, parity, sizeof(parity));

  printf("Introducing 3 random errors:\n");
  for (int i = 0; i < 3; i++)
  {
    int r = rand() % 4, c = rand() % 4;
    printf("Error %d at (%d,%d)\n", i + 1, r, c);
    received[r][c] = (received[r][c] == '0') ? '1' : '0';
  }

  printf("Received matrix with errors:\n");
  print_2d_block(received);

  result = check_2d_parity(received, &err_row, &err_col);
  printf("Result: %s\n", (result > 0) ? "ERRORS DETECTED" : "ERRORS NOT DETECTED");

  printf("\n--- Test Case 5: Four errors in rectangle pattern ---\n");
  memcpy(received, parity, sizeof(parity));

  printf("Four errors in rectangle pattern at (0,0), (0,1), (1,0), (1,1)\n");
  printf("Original matrix:\n");
  print_2d_block(received);

  received[0][0] = (received[0][0] == '0') ? '1' : '0';
  received[0][1] = (received[0][1] == '0') ? '1' : '0';
  received[1][0] = (received[1][0] == '0') ? '1' : '0';
  received[1][1] = (received[1][1] == '0') ? '1' : '0';

  printf("Matrix with rectangle pattern errors:\n");
  print_2d_block(received);

  result = check_2d_parity(received, &err_row, &err_col);
  printf("Result: %s\n", (result == 0) ? "ERRORS NOT DETECTED (Rectangle pattern limitation)" : "ERRORS DETECTED");
}

int main(void)
{
  srand(time(NULL));
  parity_2d_demo();
  return 0;
}