
import java.util.Random;
import java.util.Scanner;

public class StopAndWait {

    private static final Random random = new Random();

    private static boolean isLost() {
        return random.nextInt(2) == 0;
    }

    public static void main(String[] args) {
        try (Scanner sc = new Scanner(System.in)) {
            int N, timeout, scenario;
            int frameNo = 0, expectedAck = 1, i = 0;

            System.out.print("Enter number of frames to send: ");
            N = sc.nextInt();

            System.out.println("\nChoose transmission scenario:");
            System.out.println("1. Successful Transmission (No Loss)");
            System.out.println("2. Frame Loss");
            System.out.println("3. ACK Loss");
            System.out.print("Enter choice: ");
            scenario = sc.nextInt();

            System.out.print("\nEnter Timeout value (in seconds): ");
            timeout = sc.nextInt();

            System.out.println("\n--- Stop and Wait Simulation Started ---\n");

            while (i < N) {
                System.out.printf("Sender: Transmitting Frame %d [SeqNo=%d]%n", i + 1, frameNo);
                System.out.printf("Sender: Timer started for Frame %d (Timeout=%ds)%n", i + 1, timeout);

                boolean frameLost = false, ackLost = false;

                if (scenario == 2) {
                    frameLost = isLost();
                }
                if (scenario == 3) {
                    ackLost = isLost();
                }

                if (frameLost) {
                    System.out.printf("!!! Frame %d Lost during transmission !!!%n", i + 1);
                    System.out.printf("Sender: Timeout occurred. Retransmitting Frame %d...%n%n", i + 1);
                    continue;
                }

                System.out.printf("Receiver: Frame %d received correctly.%n", i + 1);
                System.out.printf("Receiver: Sending ACK%d (Next expected SeqNo)%n", expectedAck);

                if (ackLost) {
                    System.out.printf("!!! ACK%d Lost in transmission !!!%n", expectedAck);
                    System.out.printf("Sender: Timeout occurred. Retransmitting Frame %d...%n%n", i + 1);
                    continue;
                }

                System.out.printf("Sender: ACK%d received. Timer stopped.%n", expectedAck);

                frameNo = 1 - frameNo;
                expectedAck = 1 - expectedAck;
                i++;

                System.out.println("-------------------------------------------------");
            }

            System.out.printf("%nAll %d frames transmitted and acknowledged successfully!%n", N);
            System.out.println("--- Simulation Completed ---");
            sc.close();
        }
    }
}
