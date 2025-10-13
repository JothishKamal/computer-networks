import java.io.*;
import java.net.*;
import java.util.Scanner;

public class UdpClient {
  public static void main(String[] args) {
    try (DatagramSocket socket = new DatagramSocket()) {
      InetAddress address = InetAddress.getByName("127.0.0.1");
      try (Scanner sc = new Scanner(System.in)) {
        System.out.println("Connected to UDP server. Type \"exit\" to quit.");

        while (true) {
          System.out.print("You: ");
          String msg = sc.nextLine();

          byte[] buffer = msg.getBytes();
          DatagramPacket packet = new DatagramPacket(buffer, buffer.length, address, 9090);
          socket.send(packet);

          if (msg.equals("exit")) {
            break;
          }

          byte[] responseBuffer = new byte[1024];
          DatagramPacket responsePacket = new DatagramPacket(responseBuffer, responseBuffer.length);
          socket.receive(responsePacket);

          String response = new String(responsePacket.getData(), 0, responsePacket.getLength());
          System.out.println(response);
        }
      }
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}
