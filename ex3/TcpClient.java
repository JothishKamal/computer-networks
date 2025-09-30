import java.io.*;
import java.net.*;

public class TcpClient {
  public static void main(String[] args) {
    try (Socket socket = new Socket("127.0.0.1", 9090)) {
      System.out.println("Connected to server on port 9090...");

      BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
      PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
      BufferedReader userInput = new BufferedReader(new InputStreamReader(System.in));

      String msg;
      while (true) {
        System.out.print("You: ");
        msg = userInput.readLine();
        out.println(msg);

        if (msg.equals("exit")) {
          break;
        }

        System.out.println(in.readLine());
      }
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}
