
import java.util.*;

public class SelectiveRepeat {

    static class Frame {

        int id;
        int timer;
        boolean acked;
        boolean sent;
        boolean inTransit;
        int arrivalTime;
        int transmissions;

        Frame(int id) {
            this.id = id;
            this.timer = 0;
            this.acked = false;
            this.sent = false;
            this.inTransit = false;
            this.arrivalTime = -1;
            this.transmissions = 0;
        }
    }

    static class ReceiverBuffer {

        int id;
        boolean received;
        boolean acked;

        ReceiverBuffer(int id) {
            this.id = id;
            this.received = false;
            this.acked = false;
        }
    }

    public static void main(String[] args) {
        try (Scanner sc = new Scanner(System.in)) {
            final int MAX_FRAMES = 100;
            
            System.out.print("Enter number of frames (N): ");
            int N = sc.nextInt();
            System.out.print("Enter window size (W): ");
            int W = sc.nextInt();
            System.out.print("Enter timeout value (units): ");
            int timeout = sc.nextInt();
            System.out.print("Choose Scenario: 1.Normal 2.Lost Frame 3.Lost ACK : ");
            int scenario = sc.nextInt();
            
            Frame[] sender = new Frame[MAX_FRAMES];
            ReceiverBuffer[] receiver = new ReceiverBuffer[MAX_FRAMES];
            boolean[] frameLostOnce = new boolean[MAX_FRAMES];
            boolean[] ackLostOnce = new boolean[MAX_FRAMES];
            int[] ackArrivalTime = new int[MAX_FRAMES];
            
            for (int i = 0; i < N; i++) {
                sender[i] = new Frame(i);
                receiver[i] = new ReceiverBuffer(i);
                ackArrivalTime[i] = -1;
            }
            
            int time = 0;
            int nextFrameToSend = 0;
            int allAcked = 0;
            int channelDelay = 2;
            
            System.out.println("\n--- Simulation Start ---\n");
            
            while (allAcked < N && time < 200) {
                
                int winStart = 0;
                for (int i = 0; i < N; i++) {
                    if (!sender[i].acked) {
                        winStart = i;
                        break;
                    }
                }
                
                while (nextFrameToSend < N && nextFrameToSend < winStart + W) {
                    Frame f = sender[nextFrameToSend];
                    if (!f.sent || f.timer == 0) {
                        System.out.printf("[Time %d] Sender: Sending Frame %d -> Timer[%d]=%d started%n",
                                time, nextFrameToSend, nextFrameToSend, timeout);
                        f.sent = true;
                        f.timer = timeout;
                        f.inTransit = true;
                        f.arrivalTime = time + channelDelay;
                        f.transmissions++;
                    }
                    nextFrameToSend++;
                }
                
                for (int i = 0; i < N; i++) {
                    Frame f = sender[i];
                    if (f.inTransit && f.arrivalTime == time) {
                        if (scenario == 2 && i == 1 && !frameLostOnce[i]) {
                            System.out.printf("[Time %d] Frame %d lost in transit!%n", time, i);
                            frameLostOnce[i] = true;
                            f.inTransit = false;
                            continue;
                        }
                        
                        if (receiver[i].received) {
                            System.out.printf("[Time %d] Receiver: Duplicate Frame %d received -> Discarded%n", time, i);
                        } else {
                            receiver[i].received = true;
                            System.out.printf("[Time %d] Receiver: Frame %d received -> Sending ACK %d%n", time, i, i);
                        }
                        
                        f.inTransit = false;
                        receiver[i].acked = true;
                        ackArrivalTime[i] = time + 1;
                    }
                }
                
                for (int i = 0; i < N; i++) {
                    if (receiver[i].acked && !sender[i].acked && ackArrivalTime[i] == time) {
                        if (scenario == 3 && i == 2 && !ackLostOnce[i]) {
                            System.out.printf("[Time %d] ACK %d lost in transit!%n", time, i);
                            ackLostOnce[i] = true;
                            continue;
                        }
                        sender[i].acked = true;
                        sender[i].timer = 0;
                        System.out.printf("[Time %d] Sender: ACK %d received -> Timer stopped%n", time, i);
                        allAcked++;
                    }
                }
                
                for (int i = 0; i < N; i++) {
                    Frame f = sender[i];
                    if (f.sent && !f.acked && f.timer > 0) {
                        f.timer--;
                        if (f.timer == 0) {
                            System.out.printf("[Time %d] Sender: Timeout! Retransmitting Frame %d -> Timer reset=%d%n",
                                    time, i, timeout);
                            f.timer = timeout;
                            f.inTransit = true;
                            f.arrivalTime = time + channelDelay;
                            f.transmissions++;
                        }
                    }
                }
                
                System.out.printf("[Time %d] ", time);
                for (int i = 0; i < N; i++) {
                    Frame f = sender[i];
                    if (f.sent && !f.acked) {
                        System.out.printf("Timer[%d]=%d ", f.id, f.timer);
                    }
                }
                System.out.println();
                
                System.out.print("Receiver: Delivering frames in order -> ");
                for (int i = 0; i < N; i++) {
                    if (receiver[i].received) {
                        System.out.print(receiver[i].id + " ");
                    }else {
                        break;
                    }
                }
                System.out.println("\n");
                
                time++;
            }
            
            System.out.printf("\n--- Simulation Complete: All %d frames delivered successfully ---\n", N);
        }
    }
}
