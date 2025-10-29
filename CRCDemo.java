import java.util.Random;

public class CRCDemo {
    public static void main(String[] args) {
        new CRCDemo().runDemo();
    }

    private void runDemo() {
        System.out.println("\n=== CRC DEMO ===");

        String data = "1010101011001100";
        String generator = "11010101";
        String codeword;
        String received;

        System.out.println("Data word (16 bits): " + data);
        System.out.println("Generator polynomial (CRC-8): " + generator);

        codeword = generateCRC(data, generator);
        System.out.println("Generated codeword: " + codeword);

        String[] testNames = { "No error", "Single bit error", "Odd number of errors", "Burst error" };
        Random rand = new Random();

        for (int test = 0; test < 4; test++) {
            System.out.println("\n--- Test Case " + (test + 1) + ": " + testNames[test] + " ---");
            received = codeword;

            if (test > 0) {
                received = introduceErrors(received, test, rand);
            }

            System.out.println("Received: " + received);
            boolean errorDetected = checkCRC(received, generator);
            System.out.println("CRC Check: " + (errorDetected ? "ERROR DETECTED" : "NO ERROR"));
        }
    }

    private String generateCRC(String data, String generator) {
        int dataLen = data.length();
        int genLen = generator.length();

        StringBuilder temp = new StringBuilder(data);
        for (int i = 0; i < genLen - 1; i++) {
            temp.append('0');
        }

        char[] tempArr = temp.toString().toCharArray();
        char[] genArr = generator.toCharArray();

        for (int i = 0; i <= dataLen - 1; i++) {
            if (tempArr[i] == '1') {
                for (int j = 0; j < genLen; j++) {
                    tempArr[i + j] = (char) (((tempArr[i + j] - '0') ^ (genArr[j] - '0')) + '0');
                }
            }
        }

        String remainder = new String(tempArr, dataLen, genLen - 1);
        System.out.println("CRC remainder: " + remainder);

        return data + remainder;
    }

    private boolean checkCRC(String received, String generator) {
        int recLen = received.length();
        int genLen = generator.length();

        char[] tempArr = received.toCharArray();
        char[] genArr = generator.toCharArray();

        for (int i = 0; i <= recLen - genLen; i++) {
            if (tempArr[i] == '1') {
                for (int j = 0; j < genLen; j++) {
                    tempArr[i + j] = (char) (((tempArr[i + j] - '0') ^ (genArr[j] - '0')) + '0');
                }
            }
        }

        String remainder = new String(tempArr, recLen - genLen + 1, genLen - 1);
        System.out.println("Check remainder: " + remainder);

        for (int i = recLen - genLen + 1; i < recLen; i++) {
            if (tempArr[i] == '1') {
                return true;
            }
        }
        return false;
    }

    private String introduceErrors(String data, int errorType, Random rand) {
        char[] arr = data.toCharArray();
        int len = arr.length;

        switch (errorType) {
            case 1 ->  {
                int pos = rand.nextInt(len);
                arr[pos] = (arr[pos] == '0') ? '1' : '0';
                System.out.println("Single bit error at position " + pos);
            }
            case 2 ->  {
                System.out.println("Introducing 3 random errors");
                for (int i = 0; i < 3; i++) {
                    int pos = rand.nextInt(len);
                    arr[pos] = (arr[pos] == '0') ? '1' : '0';
                }
            }
            case 3 ->  {
                int burstLen = 4;
                int start = rand.nextInt(len - burstLen);
                System.out.println("Burst error from position " + start + " to " + (start + burstLen - 1));
                for (int i = 0; i < burstLen; i++) {
                    arr[start + i] = (arr[start + i] == '0') ? '1' : '0';
                }
            }
        }

        return new String(arr);
    }
}
