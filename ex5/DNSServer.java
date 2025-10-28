import java.io.*;
import java.net.*;

public class DNSServer {
  private static final int PORT = 5353;

  public static void main(String[] args) {
    try (ServerSocket serverSocket = new ServerSocket(PORT)) {
      System.out.println("DNS Server started on port " + PORT);
      System.out.println("Waiting for clients...\n");

      while (true) {
        try (Socket clientSocket = serverSocket.accept();
            BufferedReader in = new BufferedReader(
                new InputStreamReader(clientSocket.getInputStream()));
            PrintWriter out = new PrintWriter(
                clientSocket.getOutputStream(), true)) {

          System.out.println("Client connected: " +
              clientSocket.getInetAddress());

          String input = in.readLine();
          System.out.println("Received: " + input);

          String result = resolveQuery(input);
          out.println(result);

          System.out.println("Sent: " + result + "\n");

        } catch (IOException e) {
          System.err.println("Error handling client: " +
              e.getMessage());
        }
      }
    } catch (IOException e) {
      System.err.println("Server error: " + e.getMessage());
    }
  }

  private static String resolveQuery(String input) {
    try {
      if (isIPAddress(input)) {
        InetAddress addr = InetAddress.getByName(input);
        String hostname = addr.getCanonicalHostName();

        if (hostname.equals(input)) {
          return "ERROR: Could not resolve hostname for IP: " + input;
        }
        return "Hostname: " + hostname;
      } else {
        InetAddress addr = InetAddress.getByName(input);
        return "IP Address: " + addr.getHostAddress();
      }
    } catch (UnknownHostException e) {
      return "ERROR: Unable to resolve: " + input;
    }
  }

  private static boolean isIPAddress(String input) {
    String ipPattern = "(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])";
    return input.matches(ipPattern);
  }
}
