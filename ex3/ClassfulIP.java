import java.util.Scanner;

public class ClassfulIP {
  public static void main(String[] args) {
    try (Scanner sc = new Scanner(System.in)) {
      System.out.print("Enter IP Address: ");
      String ipstr = sc.nextLine().trim();
      String[] parts = ipstr.split("\\.");
      if (parts.length != 4) {
        System.out.println("Invalid IP format.");
        return;
      }
      int a, b, c, d;
      try {
        a = Integer.parseInt(parts[0]);
        b = Integer.parseInt(parts[1]);
        c = Integer.parseInt(parts[2]);
        d = Integer.parseInt(parts[3]);
      } catch (NumberFormatException e) {
        System.out.println("Invalid IP values.");
        return;
      }
      if (a < 0 || a > 255 || b < 0 || b > 255 || c < 0 || c > 255 || d < 0 || d > 255) {
        System.out.println("Invalid IP address values.");
        return;
      }
      if (a == 0) {
        System.out.println("IP starting with 0 is reserved/invalid for classful computations.");
        return;
      }
      long ip = ((long) a << 24) | ((long) b << 16) | ((long) c << 8) | d;
      char cls = '?';
      int maskBits = 0;
      if (a >= 1 && a <= 126) {
        cls = 'A';
        maskBits = 8;
      } else if (a == 127) {
        System.out.println("127.x.x.x is loopback/reserved.");
        return;
      } else if (a >= 128 && a <= 191) {
        cls = 'B';
        maskBits = 16;
      } else if (a >= 192 && a <= 223) {
        cls = 'C';
        maskBits = 24;
      } else if (a >= 224 && a <= 239) {
        System.out.println("Class D (Multicast) - mask/hosts not applicable.");
        return;
      } else if (a >= 240 && a <= 255) {
        System.out.println("Class E (Experimental) - mask/hosts not applicable.");
        return;
      }
      long mask = (maskBits == 0) ? 0 : (0xFFFFFFFFL << (32 - maskBits)) & 0xFFFFFFFFL;
      long network = ip & mask;
      long broadcast = network | (~mask & 0xFFFFFFFFL);
      long first = network + 1;
      long last = broadcast - 1;
      long totalAddresses = 1L << (32 - maskBits);
      long usableHosts = (totalAddresses > 2) ? totalAddresses - 2 : 0;
      System.out.printf("\nIP Address: %d.%d.%d.%d\n", a, b, c, d);
      System.out.println("Class: " + cls);
      System.out.printf("Default Mask: %d.%d.%d.%d\n",
          (mask >> 24) & 0xFF, (mask >> 16) & 0xFF, (mask >> 8) & 0xFF, mask & 0xFF);
      System.out.println("Size of Network (total addresses): " + totalAddresses);
      System.out.println("Number of usable hosts: " + usableHosts);
      System.out.printf("Network Address: %d.%d.%d.%d\n",
          (network >> 24) & 0xFF, (network >> 16) & 0xFF, (network >> 8) & 0xFF, network & 0xFF);
      System.out.printf("Broadcast Address: %d.%d.%d.%d\n",
          (broadcast >> 24) & 0xFF, (broadcast >> 16) & 0xFF, (broadcast >> 8) & 0xFF, broadcast & 0xFF);
      System.out.printf("First Host: %d.%d.%d.%d\n",
          (first >> 24) & 0xFF, (first >> 16) & 0xFF, (first >> 8) & 0xFF, first & 0xFF);
      System.out.printf("Last Host: %d.%d.%d.%d\n",
          (last >> 24) & 0xFF, (last >> 16) & 0xFF, (last >> 8) & 0xFF, last & 0xFF);
    }
  }
}
