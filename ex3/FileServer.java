
import java.io.*;
import java.net.*;

public class FileServer {

    private static final int PORT = 11000;
    private static final int BUFFER_SIZE = 1024;

    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Server started on port " + PORT);

            while (true) {
                try (Socket clientSocket = serverSocket.accept()) {
                    System.out.println("Client connected: " + clientSocket.getInetAddress());

                    DataInputStream dis = new DataInputStream(clientSocket.getInputStream());
                    DataOutputStream dos = new DataOutputStream(clientSocket.getOutputStream());

                    byte[] filenameBuffer = new byte[BUFFER_SIZE];
                    int bytesRead = dis.read(filenameBuffer);
                    if (bytesRead <= 0) {
                        continue;
                    }

                    String filename = new String(filenameBuffer, 0, bytesRead).trim();
                    System.out.println("Client requested file: " + filename);

                    File file = new File(filename);
                    if (file.exists() && file.isFile()) {
                        dos.writeUTF("FOUND");
                        long fileSize = file.length();
                        dos.writeLong(fileSize);

                        try (FileInputStream fis = new FileInputStream(file)) {
                            byte[] buffer = new byte[BUFFER_SIZE];
                            int count;
                            while ((count = fis.read(buffer)) > 0) {
                                dos.write(buffer, 0, count);
                            }
                        }

                        System.out.println("File sent successfully: " + filename);
                    } else {
                        dos.writeUTF("NOTFOUND");
                        System.out.println("File not found: " + filename);
                    }
                } catch (IOException e) {
                    System.err.println("Client handling error: " + e.getMessage());
                }
            }
        } catch (IOException e) {
            System.err.println("Server error: " + e.getMessage());
        }
    }
}
