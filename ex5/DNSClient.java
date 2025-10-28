import java.io.*;
import java.net.*;
import java.util.Scanner;

public class DNSClient {
  private static final String SERVER_HOST = "localhost";
  private static final int SERVER_PORT = 5353;

  public static void main(String[] args) {
    try (Scanner scanner = new Scanner(System.in)) {
      System.out.println("=== DNS Client ===");
      System.out.println("Enter hostname or IP address (or 'exit' to quit):\n");

      while (true) {
        System.out.print("Query: ");
        String query = scanner.nextLine().trim();

        if (query.equalsIgnoreCase("exit")) {
          System.out.println("Exiting...");
          break;
        }

        if (query.isEmpty()) {
          System.out.println("ERROR: Please enter a valid hostname or IP\n");
          continue;
        }

        queryDNSServer(query);
      }
    }
  }

  private static void queryDNSServer(String query) {
    try (Socket socket = new Socket(SERVER_HOST, SERVER_PORT);
        PrintWriter out = new PrintWriter(
            socket.getOutputStream(), true);
        BufferedReader in = new BufferedReader(
            new InputStreamReader(socket.getInputStream()))) {

      System.out.println("Connected to DNS Server");

      out.println(query);
      System.out.println("Sent: " + query);

      String response = in.readLine();
      System.out.println("Response: " + response);
      System.out.println();

    } catch (UnknownHostException e) {
      System.err.println("ERROR: Server not found: " + e.getMessage());
    } catch (IOException e) {
      System.err.println("ERROR: Unable to connect to server");
      System.err.println("Make sure DNS Server is running on port " +
          SERVER_PORT + "\n");
    }
  }
}