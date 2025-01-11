int rows[] = {21, 22, 23, 25, 26, 27, 32, 33}; // Row pins
int cols[] = {4, 5, 13, 14, 16, 17, 18, 19};   // Column pins

byte font[][8] = {
  {0b00111100, 0b01000010, 0b01000010, 0b01111110, 0b01000010, 0b01000010, 0b01000010, 0b00000000}, // A
  {0b01111100, 0b01000010, 0b01000010, 0b01111100, 0b01000010, 0b01000010, 0b01111100, 0b00000000}, // B
  {0b00111110, 0b01000010, 0b01000000, 0b01000000, 0b01000000, 0b01000010, 0b00111110, 0b00000000}, // C
  {0b01111100, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01111100, 0b00000000}, // D
  {0b01111110, 0b01000000, 0b01000000, 0b01111100, 0b01000000, 0b01000000, 0b01111110, 0b00000000}, // E
  {0b01111110, 0b01000000, 0b01000000, 0b01111100, 0b01000000, 0b01000000, 0b01000000, 0b00000000}, // F
  {0b00111110, 0b01000010, 0b01000000, 0b01001110, 0b01000010, 0b01000010, 0b00111110, 0b00000000}, // G
  {0b01000010, 0b01000010, 0b01111110, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b00000000}, // H
  {0b00111100, 0b00001000, 0b00001000, 0b00111100, 0b00001000, 0b00001000, 0b00111100, 0b00000000}, // I
  {0b00011110, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b01000100, 0b00111110, 0b00000000}, // J
  {0b01000010, 0b01000100, 0b01001000, 0b01010000, 0b01100000, 0b01001000, 0b01000100, 0b00000000}, // K
  {0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01111110, 0b00000000}, // L
  {0b01000010, 0b01100110, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b00000000}, // M
  {0b01000010, 0b01100010, 0b01010010, 0b01001010, 0b01000110, 0b01000010, 0b01000010, 0b00000000}, // N
  {0b00111110, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b00111110, 0b00000000}, // O
  {0b01111100, 0b01000010, 0b01000010, 0b01111100, 0b01000000, 0b01000000, 0b01000000, 0b00000000}, // P
  {0b00111110, 0b01000010, 0b01000010, 0b01000010, 0b01001010, 0b01000010, 0b00111110, 0b00000000}, // Q
  {0b01111100, 0b01000010, 0b01000010, 0b01111100, 0b01001000, 0b01000100, 0b01000010, 0b00000000}, // R
  {0b00111110, 0b01000010, 0b01000000, 0b00111100, 0b00000010, 0b01000010, 0b00111110, 0b00000000}, // S
  {0b01111110, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00000000}, // T
  {0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b00111100, 0b00000000}, // U
  {0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b00100100, 0b00011000, 0b00000000}, // V
  {0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01100110, 0b01000010, 0b00000000}, // W
  {0b01000010, 0b01000010, 0b00100100, 0b00011000, 0b00011000, 0b00100100, 0b01000010, 0b01000010}, // X
  {0b01000010, 0b01000010, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00011000, 0b00000000}, // Y
  {0b01111110, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b01111110, 0b00000000}, // Z
};


// Precomputed matrices for the input text
#define MAX_TEXT_LENGTH 20 // Maximum number of characters in the input text
int textMatrices[MAX_TEXT_LENGTH][8][8]; // Array to store matrices for each character
int textLength = 0; // Number of characters in the input text

// Function to convert a character to a 2D matrix
void characterToMatrix(char character, int outputMatrix[8][8]) {
  int index = character - 'A'; // Get index of the character in the font array
  if (index < 0 || index >= sizeof(font) / sizeof(font[0])) {
    // If unsupported character, fill the matrix with 0s
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        outputMatrix[row][col] = 0;
      }
    }
    return;
  }

  // Populate the matrix row by row
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      outputMatrix[row][col] = (font[index][row] & (1 << (7 - col))) != 0 ? 1 : 0;
    }
  }
}

// Function to display the pattern on the LED matrix
void displayPattern(int matrix[8][8], int duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {
        if (matrix[row][col] == 1) {
          lightLED(row, col); // Light up the LED for a '1'
        }
      }
    }
  }
}

// Function to light up a single LED
void lightLED(int rowIdx, int colIdx) {
  // Turn off all rows and columns first
  for (int i = 0; i < 8; i++) {
    digitalWrite(rows[i], LOW);  // Turn all rows LOW
    digitalWrite(cols[i], HIGH); // Turn all columns HIGH
  }

  // Activate the desired row and column
  digitalWrite(rows[rowIdx], HIGH); // Set the specific row HIGH
  digitalWrite(cols[colIdx], LOW);  // Set the specific column LOW
  delay(1); // Small delay for stability
}
// Function to scroll the text across the LED matrix
void scrollText(int textMatrices[MAX_TEXT_LENGTH][8][8], int textLength, int scrollSpeed) {
  for (int charIdx = 0; charIdx < textLength; charIdx++) {
    for (int shift = 0; shift < 8; shift++) {
      int scrollMatrix[8][8] = {0};

      // Create a scrolling effect by shifting columns to the left
      for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
          int shiftedCol = col - shift;
          if (shiftedCol >= 0) {
            scrollMatrix[row][shiftedCol] = textMatrices[charIdx][row][col];
          }
        }
      }

      // Display the shifted matrix
      displayPattern(scrollMatrix, scrollSpeed);
    }
  }
}

void setup() {
  // Set all row pins as OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], LOW); // Turn rows off initially
  }

  // Set all column pins as OUTPUT
  for (int i = 0; i < 8; i++) {
    pinMode(cols[i], OUTPUT);
    digitalWrite(cols[i], HIGH); // Turn columns off initially
  }

  Serial.begin(115200); // Initialize serial communication

  // Define the input text
  const char *inputText = "SURATHKAL";
  textLength = strlen(inputText); // Get the length of the input text

  // Precompute matrices for the input text
  for (int i = 0; i < textLength; i++) {
    characterToMatrix(inputText[i], textMatrices[i]);
  }

  Serial.println("Matrices precomputed for the input text.");
}

void loop() {
  // Scroll through the text continuously
  scrollText(textMatrices, textLength, 200); // Adjust scroll speed (lower value = faster)
}
