
import java.util.Scanner;

public class GoBackN {

    private static final int LOST_FRAME = 2;
    private static final int LOST_ACK = 3;

    private static void simulate(int N, int W, int scenario, int timeout) {
        int time = 0, base = 0, nextFrame = 0, timer = -1, delivered = 0;
        boolean ackLostOnce = false, frameLostOnce = false;
        int[] sendTime = new int[100];
        boolean[] acked = new boolean[100];

        System.out.println("\n--- Go-Back-N Sliding Window Protocol Simulation ---");

        while (delivered < N) {

            if (nextFrame < base + W && nextFrame < N) {
                System.out.printf("[Time %d] Sender: Sending Frame %d%n", time, nextFrame);
                sendTime[nextFrame] = time;
                if (base == nextFrame && timer == -1) {
                    timer = timeout;
                    System.out.printf("[Time %d] Sender: Timer started for Frame %d (T=%d)%n", time, base, timeout);
                }
                nextFrame++;
            }

            for (int i = base; i < nextFrame; i++) {
                if (!acked[i] && time >= sendTime[i] + 2) {
                    if (scenario == LOST_FRAME && i == 2 && !frameLostOnce) {
                        System.out.printf("[Time %d] Receiver: Frame %d lost in transit%n", time, i);
                        System.out.printf("[Time %d] Receiver: Sending ACK %d (last in-order)%n", time, i);
                        frameLostOnce = true;
                        continue;
                    }
                    if (scenario == LOST_ACK && i == W - 1 && !ackLostOnce) {
                        System.out.printf("[Time %d] Receiver: Frame %d received -> Sending ACK %d%n", time, i, i + 1);
                        System.out.printf("(ACK %d lost in transit)%n", i + 1);
                        ackLostOnce = true;
                        continue;
                    }
                    System.out.printf("[Time %d] Receiver: Frame %d received -> Sending ACK %d%n", time, i, i + 1);
                    System.out.printf("[Time %d] Sender: ACK %d received -> Timer stopped%n", time, i + 1);
                    acked[i] = true;

                    while (base < N && acked[base]) {
                        base++;
                    }

                    delivered = base;
                    timer = -1;
                    if (base < nextFrame) {
                        timer = timeout;
                        System.out.printf("[Time %d] Sender: Timer restarted for Frame %d (T=%d)%n", time, base, timeout);
                    }
                }
            }

            if (timer > 0) {
                timer--;
                System.out.printf("[Time %d] Timer = %d%n", time, timer);
            } else if (timer == 0) {
                System.out.printf("[Time %d] Sender: Timeout -> Retransmitting Frames %d to %d%n",
                        time, base, nextFrame - 1);
                for (int i = base; i < nextFrame; i++) {
                    sendTime[i] = time;
                }
                timer = timeout;
            }

            time++;
        }

        System.out.printf("All %d frames successfully delivered.%n", N);
    }

    public static void main(String[] args) {
        try (Scanner sc = new Scanner(System.in)) {
            int N, W, scenario, timeout;

            System.out.print("Enter number of frames to send: ");
            N = sc.nextInt();
            System.out.print("Enter window size: ");
            W = sc.nextInt();

            System.out.println("Choose Transmission Scenario:");
            System.out.println("1. Normal (No Loss)");
            System.out.println("2. Lost Frame");
            System.out.println("3. Lost ACK");
            System.out.print("Enter choice: ");
            scenario = sc.nextInt();

            System.out.print("Enter timeout value: ");
            timeout = sc.nextInt();

            simulate(N, W, scenario, timeout);
            sc.close();
        }
    }
}
