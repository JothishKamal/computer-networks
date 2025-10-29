#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
uint32_t ip_to_uint(const char *s)
{
  unsigned int a, b, c, d;
  if (sscanf(s, "%u.%u.%u.%u", &a, &b, &c, &d) != 4)
  {
    printf("Invalid IP format\n");
    exit(1);
  }
  if (a > 255 || b > 255 || c > 255 || d > 255)
  {
    printf("Invalid IP octet\n");
    exit(1);
  }
  return ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d;
}
void uint_to_ip(uint32_t ip, char *buf)
{
  sprintf(buf, "%u.%u.%u.%u", (ip >> 24) & 0xff, (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff);
}
int main()
{
  char ipstr[64];
  printf("\n*** Classful Subnet Calculator ***\n\n");
  printf("Enter network IP address in dotted decimal (example: 210.100.56.0) : ");
  if (scanf("%63s", ipstr) != 1)
    return 0;
  int min_subnets = 0;
  int req_hosts = 0;
  printf("Enter minimum required number of subnets (example: 6): ");
  if (scanf("%d", &min_subnets) != 1)
    return 0;
  if (min_subnets <= 0)
  {
    printf("Minimum subnets must be positive\n");
    return 0;
  }
  printf("Enter required usable hosts per subnet (example: 30): ");
  if (scanf("%d", &req_hosts) != 1)
    return 0;
  if (req_hosts < 0)
  {
    printf("Required hosts must be nonnegative\n");
    return 0;
  }
  uint32_t ip = ip_to_uint(ipstr);
  unsigned int first_octet = (ip >> 24) & 0xFF;
  int default_mask_len = -1;
  if (first_octet >= 1 && first_octet <= 126)
    default_mask_len = 8;
  else if (first_octet >= 128 && first_octet <= 191)
    default_mask_len = 16;
  else if (first_octet >= 192 && first_octet <= 223)
    default_mask_len = 24;
  else
  {
    printf("\nProvided IP is Class D, E, loopback, or reserved and cannot be subnetted as a normal host network.\n");
    return 0;
  }
  int default_host_bits = 32 - default_mask_len;
  uint32_t default_mask = (default_mask_len == 0) ? 0 : (0xFFFFFFFFu << (32 - default_mask_len));
  uint32_t network_addr = ip & default_mask;
  char tmp[32];
  uint_to_ip(network_addr, tmp);
  int h;
  for (h = 1; h <= default_host_bits; ++h)
  {
    unsigned long long size = 1ULL << h;
    if (size <= 2)
      continue;
    if ((long long)(size - 2) >= req_hosts)
      break;
  }
  if (h > default_host_bits)
  {
    printf("\nCannot satisfy %d usable hosts per subnet inside a classful /%d network.\n", req_hosts, default_mask_len);
    printf("Either request fewer hosts-per-subnet or obtain a larger (different class) network.\n");
    return 0;
  }
  int s = default_host_bits - h;
  unsigned int possible_subnets = 1u << s;
  int mask_len = default_mask_len + s;
  unsigned int subnet_size = 1u << h;
  unsigned int usable = (subnet_size >= 2) ? subnet_size - 2 : 0;
  uint32_t mask = (mask_len == 0) ? 0 : (0xFFFFFFFFu << (32 - mask_len));
  char maskstr[32];
  uint_to_ip(mask, maskstr);
  printf("\nDetected class: ");
  if (default_mask_len == 8)
    printf("A\n");
  else if (default_mask_len == 16)
    printf("B\n");
  else if (default_mask_len == 24)
    printf("C\n");
  printf("Default mask: /%d ", default_mask_len);
  uint_to_ip(default_mask, tmp);
  printf("%s\n", tmp);
  printf("Network address (after applying default mask): %s\n", tmp);
  printf("\nSubnetting decision (based on required %d usable hosts) :\n ", req_hosts);
  printf("Minimum host bits required per subnet: %d\n", h);
  printf("Subnet mask length chosen: /%d\n", mask_len);
  printf("Subnet mask (dotted): %s\n", maskstr);
  printf("Addresses per subnet (block size): %u\n", subnet_size);
  printf("Usable hosts per subnet: %u\n", usable);
  printf("Total possible subnets from this classful network: %u\n", possible_subnets);
  if (possible_subnets < (unsigned)min_subnets)
  {
    printf("\nWarning: maximum possible subnets (%u) is less than requested minimum (%d).\n", possible_subnets, min_subnets);
    printf("The program will display the %u available subnets.\n", possible_subnets);
  }
  unsigned int to_show = possible_subnets;
  printf("\nListing all %u subnet(s):\n", to_show);
  printf("----------------------------------------------------------------------------------------------------------------------\n");
  printf("| %-5s | %-18s | %-18s | %-16s | %-16s | %-10s |\n", "Idx", "Network Address", "Broadcast Address", "First Host", "Last Host", "Usable");
  printf("----------------------------------------------------------------------------------------------------------------------\n");
  for (unsigned int i = 0; i < to_show; ++i)
  {
    uint32_t subnet_network = network_addr + (uint32_t)i * subnet_size;
    uint32_t broadcast = subnet_network + subnet_size - 1;
    uint32_t first = (usable ? subnet_network + 1 : 0);
    uint32_t last = (usable ? broadcast - 1 : 0);
    char netbuf[32], brodbuf[32], firstbuf[32], lastbuf[32];
    uint_to_ip(subnet_network, netbuf);
    uint_to_ip(broadcast, brodbuf);
    uint_to_ip(first, firstbuf);
    uint_to_ip(last, lastbuf);
    printf("| %-5u | %-18s | %-18s | %-16s | %-16s | %-10u |\n", i + 1, netbuf, brodbuf, firstbuf, lastbuf, usable);
  }
  printf("----------------------------------------------------------------------------------------------------------------------\n");
  printf("\nNOTE: This program used classful addressing rules (default mask based on IP class)\n");
  printf("If you want only a subset of the above subnets (for example exactly 6), pick the first 6 listed.\n\n");
  return 0;
}