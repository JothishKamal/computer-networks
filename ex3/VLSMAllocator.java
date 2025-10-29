public class VLSMAllocator {

  private static void prefixToMask(int prefix, int[] mask) {
    for (int i = 0; i < 4; i++) {
      if (prefix >= 8) {
        mask[i] = 255;
        prefix -= 8;
      } else {
        mask[i] = (~((1 << (8 - prefix)) - 1)) & 0xFF;
        prefix = 0;
      }
    }
  }

  private static void printIp(long ip) {
    System.out.printf("%d.%d.%d.%d",
        (ip >> 24) & 0xFF,
        (ip >> 16) & 0xFF,
        (ip >> 8) & 0xFF,
        ip & 0xFF);
  }

  private static void allocate(String org, long[] startIpRef, int hosts) {
    int required = hosts + 2;
    int size = 1;
    while (size < required)
      size <<= 1;
    int prefix = 32 - (int) (Math.log(size) / Math.log(2));
    long net = startIpRef[0];
    long broadcast = net + size - 1;
    long firstUsable = net + 1;
    long lastUsable = broadcast - 1;
    int totalHosts = size;
    int remaining = size - hosts;

    int[] mask = new int[4];
    prefixToMask(prefix, mask);

    System.out.println("\nOrganisation " + org);
    System.out.println("--------------------------");
    System.out.println("Prefix length     : /" + prefix);
    System.out.printf("Subnet mask       : %d.%d.%d.%d\n", mask[0], mask[1], mask[2], mask[3]);
    System.out.println("Total hosts avail : " + totalHosts);
    System.out.print("Network address   : ");
    printIp(net);
    System.out.println();
    System.out.print("Broadcast address : ");
    printIp(broadcast);
    System.out.println();
    System.out.print("First usable IP   : ");
    printIp(firstUsable);
    System.out.println();
    System.out.print("Last usable IP    : ");
    printIp(lastUsable);
    System.out.println();
    System.out.println("Remaining hosts   : " + remaining);

    startIpRef[0] = broadcast + 1;
  }

  public static void main(String[] args) {
    long startIp = (198L << 24) | (16L << 16);
    int[] requests = { 4000, 2000, 4000, 8000 };
    String[] orgs = { "A", "B", "C", "D" };
    long[] startIpRef = { startIp };
    for (int i = 0; i < orgs.length; i++) {
      allocate(orgs[i], startIpRef, requests[i]);
    }
  }
}
