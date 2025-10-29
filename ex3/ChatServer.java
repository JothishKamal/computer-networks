import java.io.*;
import java.net.*;
import java.util.*;
import java.util.concurrent.*;

public class ChatServer {
  private static final int PORT = 10000;
  private static final int MAX_CLIENTS = 100;
  private static final Set<ClientHandler> clients = ConcurrentHashMap.newKeySet();

  public static void main(String[] args) {
    try (ServerSocket server = new ServerSocket(PORT)) {
      System.out.println("Chat server started on port " + PORT);

      while (true) {
        Socket socket = server.accept();
        if (clients.size() >= MAX_CLIENTS) {
          try (PrintWriter out = new PrintWriter(socket.getOutputStream(), true)) {
            out.println("Server full. Try later.");
            socket.close();
          }
          continue;
        }

        ClientHandler handler = new ClientHandler(socket);
        clients.add(handler);
        new Thread(handler).start();
      }

    } catch (IOException e) {
      e.printStackTrace();
    }
  }

  static void broadcast(String msg, ClientHandler exclude) {
    for (ClientHandler c : clients) {
      if (c != exclude) {
        c.send(msg);
      }
    }
  }

  static void remove(ClientHandler c) {
    clients.remove(c);
  }

  static class ClientHandler implements Runnable {
    private final Socket socket;
    private PrintWriter out;
    private BufferedReader in;
    private String username;

    ClientHandler(Socket socket) {
      this.socket = socket;
    }

    @Override
    public void run() {
      try {
        in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        out = new PrintWriter(socket.getOutputStream(), true);

        username = in.readLine();
        if (username == null || username.isEmpty()) {
          socket.close();
          return;
        }

        System.out.println(username + " has joined.");
        ChatServer.broadcast(username + " has joined the chat\n", this);

        String msg;
        while ((msg = in.readLine()) != null) {
          if (msg.equalsIgnoreCase("exit"))
            break;
          String full = username + ": " + msg;
          System.out.println(full);
          ChatServer.broadcast(full + "\n", this);
        }
      } catch (IOException e) {
        e.printStackTrace();
      } finally {
        try {
          socket.close();
        } catch (IOException e) {
          e.printStackTrace();
        }
        ChatServer.remove(this);
        if (username != null) {
          ChatServer.broadcast(username + " has left the chat\n", this);
          System.out.println(username + " has left.");
        }
      }
    }

    void send(String msg) {
      out.println(msg);
    }
  }
}
