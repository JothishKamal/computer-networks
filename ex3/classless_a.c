#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
unsigned int ip_to_int(int a, int b, int c, int d)
{
  return ((unsigned int)a << 24) | ((unsigned int)b << 16) | ((unsigned int)c << 8) | (unsigned int)d;
}
void int_to_ip(unsigned int ip, int *a, int *b, int *c, int *d)
{
  *a = (ip >> 24) & 0xFF;
  *b = (ip >> 16) & 0xFF;
  *c = (ip >> 8) & 0xFF;
  *d = ip & 0xFF;
}
unsigned int prefix_to_mask(int prefix)
{
  return prefix == 0 ? 0 : (0xFFFFFFFF << (32 - prefix));
}
int main()
{
  char cidr[50];
  int a, b, c, d, prefix;
  printf("Enter CIDR Address (e.g., 172.16.45.5/20): ");
  scanf("%s", cidr);
  if (sscanf(cidr, "%d.%d.%d.%d/%d", &a, &b, &c, &d, &prefix) !=
      5)
  {
    printf("Invalid CIDR format.\n");
    return 1;
  }
  unsigned int ip = ip_to_int(a, b, c, d);
  unsigned int mask = prefix_to_mask(prefix);
  unsigned int network = ip & mask;
  unsigned int broadcast = network | (~mask);
  unsigned int total_hosts = (prefix == 32) ? 1 : (prefix == 31 ? 2 : (unsigned int)pow(2, 32 - prefix));
  int ma, mb, mc, md;
  int na, nb, nc, nd;
  int ba, bb, bc, bd;
  int fha, fhb, fhc, fhd;
  int lha, lhb, lhc, lhd;
  int_to_ip(mask, &ma, &mb, &mc, &md);
  int_to_ip(network, &na, &nb, &nc, &nd);
  int_to_ip(broadcast, &ba, &bb, &bc, &bd);
  if (prefix <= 30)
  {
    int_to_ip(network + 1, &fha, &fhb, &fhc, &fhd);
    int_to_ip(broadcast - 1, &lha, &lhb, &lhc, &lhd);
  }
  else
  {
    fha = lha = fhb = lhb = fhc = lhc = fhd = lhd = -1;
  }
  printf("\nCIDR Address: %s\n", cidr);
  printf("Prefix length: %d\n", prefix);
  printf("Subnet Mask: %d.%d.%d.%d\n", ma, mb, mc, md);
  printf("Network Address: %d.%d.%d.%d\n", na, nb, nc, nd);
  printf("Broadcast Address: %d.%d.%d.%d\n", ba, bb, bc, bd);
  if (prefix == 32)
  {
    printf("Total Hosts: 1\n");
    printf("Host Address: %d.%d.%d.%d\n", na, nb, nc, nd);
  }
  else if (prefix == 31)
  {
    printf("Total Hosts: 2\n");
    printf("Host 1: %d.%d.%d.%d\n", na, nb, nc, nd);
    printf("Host 2: %d.%d.%d.%d\n", ba, bb, bc, bd);
  }
  else
  {
    printf("Total Hosts: %u\n", total_hosts - 2);
    printf("First Host: %d.%d.%d.%d\n", fha, fhb, fhc, fhd);
    printf("Last Host: %d.%d.%d.%d\n", lha, lhb, lhc, lhd);
  }
  return 0;
}
