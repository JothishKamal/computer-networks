
import java.io.*;
import java.net.*;

public class FileClient {

    private static final String SERVER_IP = "127.0.0.1";
    private static final int PORT = 11000;
    private static final int BUFFER_SIZE = 1024;

    public static void main(String[] args) {
        try (Socket socket = new Socket(SERVER_IP, PORT); DataInputStream dis = new DataInputStream(socket.getInputStream()); DataOutputStream dos = new DataOutputStream(socket.getOutputStream()); BufferedReader reader = new BufferedReader(new InputStreamReader(System.in))) {

            System.out.print("Enter filename to request: ");
            String filename = reader.readLine();
            dos.write(filename.getBytes());

            String response = dis.readUTF();
            if (response.equals("FOUND")) {
                long fileSize = dis.readLong();
                try (FileOutputStream fos = new FileOutputStream(filename)) {
                    byte[] buffer = new byte[BUFFER_SIZE];
                    long received = 0;
                    int count;

                    while (received < fileSize && (count = dis.read(buffer, 0,
                            (int) Math.min(BUFFER_SIZE, fileSize - received))) > 0) {
                        fos.write(buffer, 0, count);
                        received += count;
                    }
                }

                System.out.println("File received and saved as " + filename);
            } else {
                System.out.println("File not found on server.");
            }

        } catch (IOException e) {
            System.err.println("Client error: " + e.getMessage());
        }
    }
}
