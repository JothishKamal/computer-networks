import java.util.*;

public class HammingDemo {

  static void generateHamming(String data, char[] codeword, int dataBits, int parityType) {
    int numParityBits = 0;
    while ((1 << numParityBits) < dataBits + numParityBits + 1) {
      numParityBits++;
    }
    int totalBits = dataBits + numParityBits;
    Arrays.fill(codeword, 0, totalBits, '0');

    int dataIndex = 0;
    for (int position = 1; position <= totalBits; position++) {
      boolean isParityPosition = false;
      for (int parityBitIndex = 0; parityBitIndex < numParityBits; parityBitIndex++) {
        if (position == (1 << parityBitIndex)) {
          isParityPosition = true;
          break;
        }
      }
      if (!isParityPosition && dataIndex < dataBits) {
        codeword[totalBits - position] = data.charAt(dataIndex++);
      }
    }

    for (int parityBitIndex = 0; parityBitIndex < numParityBits; parityBitIndex++) {
      int parityValue = 0;
      int parityPosition = 1 << parityBitIndex;
      for (int position = 1; position <= totalBits; position++) {
        if ((position & parityPosition) != 0 && codeword[totalBits - position] == '1') {
          parityValue ^= 1;
        }
      }
      if (parityType == 1) {
        parityValue ^= 1;
      }
      codeword[totalBits - parityPosition] = parityValue == 1 ? '1' : '0';
    }

    System.out.print("Parity bits: ");
    for (int parityBitIndex = numParityBits - 1; parityBitIndex >= 0; parityBitIndex--) {
      int parityPosition = 1 << parityBitIndex;
      System.out.print("R" + parityPosition + "-" + codeword[totalBits - parityPosition]);
      if (parityBitIndex > 0) {
        System.out.print(", ");
      }
    }
    System.out.println();
  }

  static int detectCorrectHamming(char[] received, int totalBits, int parityType) {
    int numParityBits = 0;
    while ((1 << numParityBits) < totalBits) {
      numParityBits++;
    }
    int errorPosition = 0;
    int[] parityResults = new int[10];

    for (int parityBitIndex = 0; parityBitIndex < numParityBits; parityBitIndex++) {
      int parityValue = 0;
      int parityPos = 1 << parityBitIndex;
      for (int position = 1; position <= totalBits; position++) {
        if ((position & parityPos) != 0 && received[totalBits - position] == '1') {
          parityValue ^= 1;
        }
      }
      if (parityType == 1) {
        parityValue ^= 1;
      }
      parityResults[parityBitIndex] = parityValue;
      if (parityValue == 1) {
        errorPosition += parityPos;
      }
    }

    System.out.print("Parity bits: ");
    for (int parityBitIndex = numParityBits - 1; parityBitIndex >= 0; parityBitIndex--) {
      System.out.print("C" + (parityBitIndex + 1) + "-" + parityResults[parityBitIndex]);
      if (parityBitIndex > 0) {
        System.out.print(", ");
      }
    }
    System.out.println();

    if (errorPosition != 0) {
      int flipIndex = totalBits - errorPosition;
      received[flipIndex] = received[flipIndex] == '0' ? '1' : '0';
    }
    return errorPosition;
  }

  static void extractData(char[] codeword, char[] data, int totalBits) {
    int numParityBits = 0;
    while ((1 << numParityBits) < totalBits) {
      numParityBits++;
    }
    int dataIndex = 0;

    for (int position = 1; position <= totalBits; position++) {
      boolean isParityPosition = false;
      for (int parityBitIndex = 0; parityBitIndex < numParityBits; parityBitIndex++) {
        if (position == (1 << parityBitIndex)) {
          isParityPosition = true;
          break;
        }
      }
      if (!isParityPosition) {
        data[dataIndex++] = codeword[totalBits - position];
      }
    }
    data[dataIndex] = '\0';
  }

  static void hammingDemo() {
    System.out.println("\n=== HAMMING CODE DEMO ===");

    System.out.println("\n--- Demo 1: 11-bit data word ---");
    String data1 = "00101101011";
    char[] codeword1 = new char[20];
    char[] received1 = new char[20];
    System.out.println("Given data word: " + data1);
    generateHamming(data1, codeword1, 11, 0);
    String codewordStr1 = new String(codeword1, 0, 11 + (int) Math.ceil(Math.log(11 + 1) / Math.log(2)));
    System.out.println("Generated codeword: " + codewordStr1);

    System.out.println("\n-- Error-free transmission --");
    System.arraycopy(codeword1, 0, received1, 0, codewordStr1.length());
    System.out.println("Received codeword: " + new String(received1, 0, codewordStr1.length()));
    int errorPosition = detectCorrectHamming(received1, codewordStr1.length(), 0);
    if (errorPosition == 0) {
      System.out.println("No error detected.");
    }

    System.out.println("\n-- Error at position 7 --");
    System.arraycopy(codeword1, 0, received1, 0, codewordStr1.length());

    int totalLength = codewordStr1.length();
    int flipIndex = totalLength - 7;
    received1[flipIndex] = received1[flipIndex] == '0' ? '1' : '0';
    System.out.println("Received codeword with error: " + new String(received1, 0, totalLength));

    detectCorrectHamming(received1, totalLength, 0);
    System.out.println("Corrected codeword: " + new String(received1, 0, totalLength));

    char[] recovered = new char[20];
    extractData(received1, recovered, totalLength);
    StringBuilder recoveredString = new StringBuilder();
    for (char c : recovered) {
      if (c == '\0') {
        break;
      }
      recoveredString.append(c);
    }
    System.out.println("Original data recovered: " + recoveredString);

    System.out.println("\n--- Demo 2: 4-bit message ---");
    String data2 = "1101";
    char[] codeword2 = new char[10];
    char[] received2 = new char[10];

    int parityChoice = 0;
    int errorChoice = 1;
    System.out.println("Input the data word: " + data2);
    System.out.println("Input parity(odd-1/even-0): " + parityChoice);
    System.out.println("Enter the choice(Error-1/ Errorless transmission-0): " + errorChoice);
    System.out.println("Introduce Error: Random\n");

    System.out.println("Sender side:");
    System.out.println("Given Dataword: " + data2);
    generateHamming(data2, codeword2, 4, parityChoice);

    int totalBits2 = 4 + (int) Math.ceil(Math.log(4 + 1) / Math.log(2));
    String codewordStr2 = new String(codeword2, 0, totalBits2);
    System.out.println("Codeword: " + codewordStr2);

    System.out.println("\nReceiver side:");
    System.arraycopy(codeword2, 0, received2, 0, totalBits2);

    if (errorChoice == 1) {
      Random random = new Random();
      int errorBitPosition = random.nextInt(totalBits2);
      received2[errorBitPosition] = received2[errorBitPosition] == '0' ? '1' : '0';
      System.out.println("Codeword: " + new String(received2, 0, totalBits2) +
          " (Error in position " + (totalBits2 - errorBitPosition) + ")");

      int detectedErrorPosition = detectCorrectHamming(received2, totalBits2, parityChoice);
      System.out.println("Result of verification logic: Error in bit position " + detectedErrorPosition);
      System.out.println("Code word after correction: " + new String(received2, 0, totalBits2));

      char[] recoveredData = new char[10];
      extractData(received2, recoveredData, totalBits2);
      StringBuilder recoveredDataString = new StringBuilder();
      for (char c : recoveredData) {
        if (c == '\0') {
          break;
        }
        recoveredDataString.append(c);
      }
      System.out.println("Data word: " + recoveredDataString);
    }
  }

  public static void main(String[] args) {
    new Random().nextInt();
    hammingDemo();
  }
}
