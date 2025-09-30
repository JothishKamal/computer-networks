import java.io.*;
import java.net.*;

public class UdpServer {
  public static void main(String[] args) {
    try (DatagramSocket socket = new DatagramSocket(9090)) {
      System.out.println("UDP Server running on port 9090...");

      byte[] buffer = new byte[1024];
      DatagramPacket packet;

      while (true) {
        packet = new DatagramPacket(buffer, buffer.length);
        socket.receive(packet);

        String msg = new String(packet.getData(), 0, packet.getLength());
        System.out.println("Client: " + msg);

        if (msg.equals("exit")) {
          break;
        }

        String response = "Echo: " + msg;
        byte[] responseData = response.getBytes();
        DatagramPacket responsePacket = new DatagramPacket(
            responseData, responseData.length, packet.getAddress(), packet.getPort());
        socket.send(responsePacket);
      }
    } catch (IOException e) {
      e.printStackTrace();
    }
  }
}
