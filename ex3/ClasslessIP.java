import java.util.Scanner;

public class ClasslessIP {

  static long ipToLong(int a, int b, int c, int d) {
    return ((long) a << 24) | ((long) b << 16) | ((long) c << 8) | d;
  }

  static String longToIp(long ip) {
    return String.format("%d.%d.%d.%d",
        (ip >> 24) & 0xFF,
        (ip >> 16) & 0xFF,
        (ip >> 8) & 0xFF,
        ip & 0xFF);
  }

  static long prefixToMask(int prefix) {
    return prefix == 0 ? 0 : (0xFFFFFFFFL << (32 - prefix)) & 0xFFFFFFFFL;
  }

  public static void main(String[] args) {
    Scanner sc = new Scanner(System.in);
    System.out.print("Enter CIDR Address (e.g., 172.16.45.5/20): ");
    String cidr = sc.nextLine().trim();

    String[] ipAndPrefix = cidr.split("/");
    if (ipAndPrefix.length != 2) {
      System.out.println("Invalid CIDR format.");
      return;
    }

    String[] parts = ipAndPrefix[0].split("\\.");
    if (parts.length != 4) {
      System.out.println("Invalid IP format.");
      return;
    }

    int a, b, c, d, prefix;
    try {
      a = Integer.parseInt(parts[0]);
      b = Integer.parseInt(parts[1]);
      c = Integer.parseInt(parts[2]);
      d = Integer.parseInt(parts[3]);
      prefix = Integer.parseInt(ipAndPrefix[1]);
    } catch (NumberFormatException e) {
      System.out.println("Invalid values.");
      return;
    }

    long ip = ipToLong(a, b, c, d);
    long mask = prefixToMask(prefix);
    long network = ip & mask;
    long broadcast = network | (~mask & 0xFFFFFFFFL);

    long totalHosts;
    if (prefix == 32)
      totalHosts = 1;
    else if (prefix == 31)
      totalHosts = 2;
    else
      totalHosts = 1L << (32 - prefix);

    System.out.println("\nCIDR Address: " + cidr);
    System.out.println("Prefix length: " + prefix);
    System.out.println("Subnet Mask: " + longToIp(mask));
    System.out.println("Network Address: " + longToIp(network));
    System.out.println("Broadcast Address: " + longToIp(broadcast));

    if (prefix == 32) {
      System.out.println("Total Hosts: 1");
      System.out.println("Host Address: " + longToIp(network));
    } else if (prefix == 31) {
      System.out.println("Total Hosts: 2");
      System.out.println("Host 1: " + longToIp(network));
      System.out.println("Host 2: " + longToIp(broadcast));
    } else {
      System.out.println("Total Hosts: " + (totalHosts - 2));
      System.out.println("First Host: " + longToIp(network + 1));
      System.out.println("Last Host: " + longToIp(broadcast - 1));
    }
  }
}
