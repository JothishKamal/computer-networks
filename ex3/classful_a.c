#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main()
{
  char ipstr[40];
  unsigned int a, b, c, d;
  printf("Enter IP Address: ");
  if (scanf("%39s", ipstr) != 1)
    return 0;
  if (sscanf(ipstr, "%u.%u.%u.%u", &a, &b, &c, &d) != 4)
  {
    printf("Invalid IP format.\n");
    return 0;
  }
  if (a > 255 || b > 255 || c > 255 || d > 255)
  {
    printf("Invalid IP address values.\n");
    return 0;
  }
  if (a == 0)
  {
    printf("IP starting with 0 is reserved/invalid for classful computations.\n");
    return 0;
  }
  uint32_t ip = ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d;
  char cls = '?';
  int mask_bits = 0;
  if (a >= 1 && a <= 126)
  {
    cls = 'A';
    mask_bits = 8;
  }
  else if (a == 127)
  {
    printf("127.x.x.x is loopback/reserved.\n");
    return 0;
  }
  else if (a >= 128 && a <= 191)
  {
    cls = 'B';
    mask_bits = 16;
  }
  else if (a >= 192 && a <= 223)
  {
    cls = 'C';
    mask_bits = 24;
  }
  else if (a >= 224 && a <= 239)
  {
    cls = 'D';
    printf("Class D (Multicast) - mask/hosts not applicable.\n");
    return 0;
  }
  else if (a >= 240 && a <= 255)
  {
    cls = 'E';
    printf("Class E (Experimental) - mask/hosts not applicable.\n");
    return 0;
  }
  uint32_t mask = mask_bits ? (~((1u << (32 - mask_bits)) - 1u)) : 0u;
  uint32_t network = ip & mask;
  uint32_t broadcast = network | (~mask);
  uint32_t first = network + 1u;
  uint32_t last = broadcast - 1u;
  unsigned long long total_addresses = 1ull << (32 - mask_bits);
  unsigned long long usable_hosts = total_addresses > 2 ? total_addresses - 2 : 0;
  printf("\nIP Address: %u.%u.%u.%u\n", a, b, c, d);
  printf("Class: %c\n", cls);
  printf("Default Mask: %u.%u.%u.%u\n",
         (mask >> 24) & 0xFF, (mask >> 16) & 0xFF, (mask >> 8) & 0xFF, mask & 0xFF);
  printf("Size of Network (total addresses): %llu\n", total_addresses);
  printf("Number of usable hosts: %llu\n", usable_hosts);
  printf("Network Address: %u.%u.%u.%u\n",
         (network >> 24) & 0xFF, (network >> 16) & 0xFF, (network >> 8) & 0xFF, network & 0xFF);
  printf("Broadcast Address: %u.%u.%u.%u\n",
         (broadcast >> 24) & 0xFF, (broadcast >> 16) & 0xFF,
         (broadcast >> 8) & 0xFF, broadcast & 0xFF);
  printf("First Host: %u.%u.%u.%u\n",
         (first >> 24) & 0xFF, (first >> 16) & 0xFF, (first >> 8) & 0xFF, first & 0xFF);
  printf("Last Host: %u.%u.%u.%u\n",
         (last >> 24) & 0xFF, (last >> 16) & 0xFF, (last >> 8) & 0xFF,
         last & 0xFF);
  return 0;
}