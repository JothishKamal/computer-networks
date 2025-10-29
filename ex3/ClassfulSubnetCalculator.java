import java.util.*;

public class ClassfulSubnetCalculator {

  private static long ipToLong(String s) {
    String[] parts = s.split("\\.");
    if (parts.length != 4) {
      System.out.println("Invalid IP format");
      System.exit(1);
    }
    long a = Long.parseLong(parts[0]);
    long b = Long.parseLong(parts[1]);
    long c = Long.parseLong(parts[2]);
    long d = Long.parseLong(parts[3]);
    if (a > 255 || b > 255 || c > 255 || d > 255) {
      System.out.println("Invalid IP octet");
      System.exit(1);
    }
    return (a << 24) | (b << 16) | (c << 8) | d;
  }

  private static String longToIp(long ip) {
    return String.format("%d.%d.%d.%d",
        (ip >> 24) & 0xFF,
        (ip >> 16) & 0xFF,
        (ip >> 8) & 0xFF,
        ip & 0xFF);
  }

  public static void main(String[] args) {
    Scanner sc = new Scanner(System.in);
    System.out.println("\n*** Classful Subnet Calculator ***\n");
    System.out.print("Enter network IP address in dotted decimal (example: 210.100.56.0): ");
    String ipStr = sc.next();
    System.out.print("Enter minimum required number of subnets (example: 6): ");
    int minSubnets = sc.nextInt();
    if (minSubnets <= 0) {
      System.out.println("Minimum subnets must be positive");
      return;
    }
    System.out.print("Enter required usable hosts per subnet (example: 30): ");
    int reqHosts = sc.nextInt();
    if (reqHosts < 0) {
      System.out.println("Required hosts must be nonnegative");
      return;
    }
    long ip = ipToLong(ipStr);
    long firstOctet = (ip >> 24) & 0xFF;
    int defaultMaskLen;
    if (firstOctet >= 1 && firstOctet <= 126)
      defaultMaskLen = 8;
    else if (firstOctet >= 128 && firstOctet <= 191)
      defaultMaskLen = 16;
    else if (firstOctet >= 192 && firstOctet <= 223)
      defaultMaskLen = 24;
    else {
      System.out.println(
          "\nProvided IP is Class D, E, loopback, or reserved and cannot be subnetted as a normal host network.");
      return;
    }
    int defaultHostBits = 32 - defaultMaskLen;
    long defaultMask = (defaultMaskLen == 0) ? 0 : (0xFFFFFFFFL << (32 - defaultMaskLen)) & 0xFFFFFFFFL;
    long networkAddr = ip & defaultMask;

    int h;
    for (h = 1; h <= defaultHostBits; ++h) {
      long size = 1L << h;
      if (size <= 2)
        continue;
      if ((size - 2) >= reqHosts)
        break;
    }
    if (h > defaultHostBits) {
      System.out.printf("\nCannot satisfy %d usable hosts per subnet inside a classful /%d network.\n", reqHosts,
          defaultMaskLen);
      System.out.println("Either request fewer hosts-per-subnet or obtain a larger (different class) network.");
      return;
    }
    int s = defaultHostBits - h;
    long possibleSubnets = 1L << s;
    int maskLen = defaultMaskLen + s;
    long subnetSize = 1L << h;
    long usable = (subnetSize >= 2) ? subnetSize - 2 : 0;
    long mask = (maskLen == 0) ? 0 : (0xFFFFFFFFL << (32 - maskLen)) & 0xFFFFFFFFL;

    System.out.println("\nDetected class: " + (defaultMaskLen == 8 ? "A" : defaultMaskLen == 16 ? "B" : "C"));
    System.out.println("Default mask: /" + defaultMaskLen + " " + longToIp(defaultMask));
    System.out.println("Network address (after applying default mask): " + longToIp(networkAddr));
    System.out.println("\nSubnetting decision (based on required " + reqHosts + " usable hosts) :");
    System.out.println("Minimum host bits required per subnet: " + h);
    System.out.println("Subnet mask length chosen: /" + maskLen);
    System.out.println("Subnet mask (dotted): " + longToIp(mask));
    System.out.println("Addresses per subnet (block size): " + subnetSize);
    System.out.println("Usable hosts per subnet: " + usable);
    System.out.println("Total possible subnets from this classful network: " + possibleSubnets);

    if (possibleSubnets < minSubnets) {
      System.out.printf("\nWarning: maximum possible subnets (%d) is less than requested minimum (%d).\n",
          possibleSubnets, minSubnets);
      System.out.println("The program will display the " + possibleSubnets + " available subnets.");
    }
    System.out.println("\nListing all " + possibleSubnets + " subnet(s):");
    System.out.println(
        "----------------------------------------------------------------------------------------------------------------------");
    System.out.printf("| %-5s | %-18s | %-18s | %-16s | %-16s | %-10s |\n", "Idx", "Network Address",
        "Broadcast Address", "First Host", "Last Host", "Usable");
    System.out.println(
        "----------------------------------------------------------------------------------------------------------------------");
    for (long i = 0; i < possibleSubnets; ++i) {
      long subnetNetwork = networkAddr + (i * subnetSize);
      long broadcast = subnetNetwork + subnetSize - 1;
      long first = (usable > 0) ? subnetNetwork + 1 : 0;
      long last = (usable > 0) ? broadcast - 1 : 0;
      System.out.printf("| %-5d | %-18s | %-18s | %-16s | %-16s | %-10d |\n",
          i + 1, longToIp(subnetNetwork), longToIp(broadcast),
          longToIp(first), longToIp(last), usable);
    }
    System.out.println(
        "----------------------------------------------------------------------------------------------------------------------");
    System.out.println("\nNOTE: This program used classful addressing rules (default mask based on IP class)");
    System.out
        .println("If you want only a subset of the above subnets (for example exactly 6), pick the first 6 listed.\n");
  }
}
