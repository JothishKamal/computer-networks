import java.io.*;
import java.net.*;

public class ChatClient {
  private static final int PORT = 10000;

  public static void main(String[] args) {
    String serverIp = "127.0.0.1";
    if (args.length >= 1)
      serverIp = args[0];

    try (Socket socket = new Socket(serverIp, PORT)) {
      BufferedReader console = new BufferedReader(new InputStreamReader(System.in));
      BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      PrintWriter out = new PrintWriter(socket.getOutputStream(), true);

      System.out.print("Enter username: ");
      String username = console.readLine();
      out.println(username);

      Thread reader = new Thread(() -> {
        try {
          String line;
          while ((line = in.readLine()) != null) {
            System.out.println(line);
          }
        } catch (IOException e) {
          System.out.println("Disconnected from server.");
        }
        System.exit(0);
      });
      reader.setDaemon(true);
      reader.start();

      String msg;
      while ((msg = console.readLine()) != null) {
        if (msg.equalsIgnoreCase("exit")) {
          socket.close();
          break;
        }
        out.println(msg);
      }
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}
