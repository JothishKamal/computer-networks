#include <stdio.h>
#include <math.h>
#include <stdint.h>

void prefix_to_mask(int prefix, unsigned char mask[4])
{
  for (int i = 0; i < 4; i++)
  {
    if (prefix >= 8)
    {
      mask[i] = 255;
      prefix -= 8;
    }
    else
    {
      mask[i] = (unsigned char)(~((1 << (8 - prefix)) - 1) & 0xFF);
      prefix = 0;
    }
  }
}

void print_ip(uint32_t ip)
{
  printf("%u.%u.%u.%u",
         (ip >> 24) & 0xFF,
         (ip >> 16) & 0xFF,
         (ip >> 8) & 0xFF,
         ip & 0xFF);
}

void allocate(const char *org, uint32_t *start_ip, int hosts)
{
  int required = hosts + 2;
  int size = 1;
  while (size < required)
    size <<= 1;
  int prefix = 32 - (int)log2(size);

  uint32_t net = *start_ip;
  uint32_t broadcast = net + size - 1;
  uint32_t first_usable = net + 1;
  uint32_t last_usable = broadcast - 1;
  int total_hosts = size;
  int remaining = size - hosts;

  unsigned char mask[4];
  prefix_to_mask(prefix, mask);

  printf("\nOrganisation %s\n", org);
  printf("--------------------------\n");
  printf("Prefix length     : /%d\n", prefix);
  printf("Subnet mask       : %u.%u.%u.%u\n", mask[0], mask[1], mask[2], mask[3]);
  printf("Total hosts avail : %d\n", total_hosts);
  printf("Network address   : ");
  print_ip(net);
  printf("\n");
  printf("Broadcast address : ");
  print_ip(broadcast);
  printf("\n");
  printf("First usable IP   : ");
  print_ip(first_usable);
  printf("\n");
  printf("Last usable IP    : ");
  print_ip(last_usable);
  printf("\n");
  printf("Remaining hosts   : %d\n", remaining);

  *start_ip = broadcast + 1;
}

int main()
{

  uint32_t start_ip = (198 << 24) | (16 << 16) | (0 << 8) | 0;

  int requests[4] = {4000, 2000, 4000, 8000};
  const char *orgs[4] = {"A", "B", "C", "D"};

  for (int i = 0; i < 4; i++)
  {
    allocate(orgs[i], &start_ip, requests[i]);
  }

  return 0;
}
