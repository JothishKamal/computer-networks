import java.io.*;
import java.net.*;

public class TcpServer {
  public static void main(String[] args) {
    try (ServerSocket server = new ServerSocket(9090)) {
      System.out.println("TCP Server running on port 9090...");

      try (Socket client = server.accept()) {
        System.out.println("Client connected.");

        BufferedReader in = new BufferedReader(new InputStreamReader(client.getInputStream()));
        PrintWriter out = new PrintWriter(client.getOutputStream(), true);

        String msg;
        while ((msg = in.readLine()) != null) {
          System.out.println("Client: " + msg);
          if (msg.equals("exit")) {
            break;
          }
          out.println("Echo: " + msg);
        }
      } catch (IOException e) {
        e.printStackTrace();
      }
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}
