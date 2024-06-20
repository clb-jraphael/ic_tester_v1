#include <LiquidCrystal.h>

// CONSTANTS AND OBJECTS
const byte PIN_BUZZ = 3;
const byte PIN_POT = A1;
const byte PIN_BTN_UP = 23;
const byte PIN_BTN_DOWN = 25;
const byte PIN_BTN_LEFT = 22;
const byte PIN_BTN_RIGHT = 27;
const byte PIN_BTN_OK = 24;
const byte PIN_BTN_CANCEL = 26;


const byte MAX_BUTTONS = 6;
const  byte PINS_BUTTONS[MAX_BUTTONS] = {
  PIN_BTN_UP,
  PIN_BTN_DOWN,
  PIN_BTN_LEFT,
  PIN_BTN_RIGHT,
  PIN_BTN_OK,
  PIN_BTN_CANCEL
};

byte flag_button[MAX_BUTTONS];

const byte PIN_PROBE= A2;

// Px headers
const byte PIN_PWM_P0 = 5;
const byte PIN_PWM_P1 = 7;
const byte PIN_PWM_P2 = 9;
const byte PIN_PWM_P3 = 11;

// Vx headers
const byte PIN_VOLTMETER_V0 = A3;
const byte PIN_VOLTMETER_V1 = A4;
const byte PIN_VOLTMETER_V2 = A5;
const byte PIN_VOLTMETER_V3 = A6;

const byte PIN_IC_PIN1 = A15; // physical pin 1 //A1
const byte PIN_IC_PIN2 = 33; 
const byte PIN_IC_PIN3 = 35; 
const byte PIN_IC_PIN4 = 37; 
const byte PIN_IC_PIN5 = 39; 
const byte PIN_IC_PIN6 = 41; 
const byte PIN_IC_PIN7 = 43; 
const byte PIN_IC_PIN8 = 45; 
const byte PIN_IC_PIN9 = 47; 
const byte PIN_IC_PIN10 = 49; 
const byte PIN_IC_PIN11 = 48; 
const byte PIN_IC_PIN12 = 46; 
const byte PIN_IC_PIN13 = 44; 
const byte PIN_IC_PIN14 = 42; 
const byte PIN_IC_PIN15 = 40; 
const byte PIN_IC_PIN16 = 38;
const byte PIN_IC_PIN17 = 36;
const byte PIN_IC_PIN18 = 34;
const byte PIN_IC_PIN19 = 32;
const byte PIN_IC_PIN20 = A14;

const byte PINS_IC[20] = {
  PIN_IC_PIN1,
  PIN_IC_PIN2,
  PIN_IC_PIN3,
  PIN_IC_PIN4,
  PIN_IC_PIN5,
  PIN_IC_PIN6,
  PIN_IC_PIN7,
  PIN_IC_PIN8, 
  PIN_IC_PIN9, 
  PIN_IC_PIN10, 
  PIN_IC_PIN11, 
  PIN_IC_PIN12, 
  PIN_IC_PIN13, 
  PIN_IC_PIN14,
  PIN_IC_PIN15,
  PIN_IC_PIN16,
  PIN_IC_PIN17,
  PIN_IC_PIN18,
  PIN_IC_PIN19,
  PIN_IC_PIN20
};

const byte PIN_RGBLED_R = 6;
const byte PIN_RGBLED_G = 4;
const byte PIN_RGBLED_B = 2;

const byte PIN_LED1 = 8;

const int PIN_LCD_CONTRAST = A10;

const int rs = A10, en = A12, d4 = 28, d5 = 29, d6 = 30, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define the structure to hold IC test patterns
struct IC_TestPatterns {
  const char* icType;       // IC type (e.g., "7400", "7402", etc.)
  int numTestCases;         // Number of test cases for this IC
  const char* testPatterns[4];  // Array to hold test patterns (adjust size as needed)
  const byte* inputPinsA;                // Pointer to array of input pins A
  const byte* inputPinsB;                // Pointer to array of input pins B (if needed)
  const byte* outputPins;                // Pointer to array of output pins
};



// IC INPUT AND OUTPUT SETUPS
const byte inputPinsA7400[] = {PINS_IC[0], PINS_IC[3], PINS_IC[15], PINS_IC[18]}; // A1, A2, A3, A4
const byte inputPinsB7400[] = {PINS_IC[1], PINS_IC[4], PINS_IC[14], PINS_IC[17]}; // B1, B2, B3, B4
const byte outputPins7400[] = {PINS_IC[2], PINS_IC[5], PINS_IC[13], PINS_IC[16]}; // Y1, Y2, Y3, Y4

const byte inputPinsA7402[] = {PINS_IC[2], PINS_IC[5], PINS_IC[13], PINS_IC[16]}; // A1, A2, A3, A4
const byte inputPinsB7402[] = {PINS_IC[1], PINS_IC[4], PINS_IC[14], PINS_IC[17]}; // B1, B2, B3, B4
const byte outputPins7402[] = {PINS_IC[0], PINS_IC[3], PINS_IC[15], PINS_IC[18]}; // Y1, Y2, Y3, Y4

const byte inputPins7404[] = {PINS_IC[0], PINS_IC[2], PINS_IC[4], PINS_IC[14], PINS_IC[16], PINS_IC[18]}; // A1, A2, A3, A4, A5, A6
const byte outputPins7404[] = {PINS_IC[1], PINS_IC[3], PINS_IC[5], PINS_IC[13], PINS_IC[15], PINS_IC[17]}; // Y1, Y2, Y3, Y4, Y5, Y6

const byte inputPinsA7408[] = {PINS_IC[0], PINS_IC[3], PINS_IC[15], PINS_IC[18]}; // A1, A2, A3, A4
const byte inputPinsB7408[] = {PINS_IC[1], PINS_IC[4], PINS_IC[14], PINS_IC[17]}; // B1, B2, B3, B4
const byte outputPins7408[] = {PINS_IC[2], PINS_IC[5], PINS_IC[13], PINS_IC[16]}; // Y1, Y2, Y3, Y4

const byte inputPinsA7432[] = {PINS_IC[0], PINS_IC[3], PINS_IC[15], PINS_IC[18]}; // A1, A2, A3, A4
const byte inputPinsB7432[] = {PINS_IC[1], PINS_IC[4], PINS_IC[14], PINS_IC[17]}; // B1, B2, B3, B4
const byte outputPins7432[] = {PINS_IC[2], PINS_IC[5], PINS_IC[13], PINS_IC[16]}; // Y1, Y2, Y3, Y4

const byte inputPinsA7486[] = {PINS_IC[0], PINS_IC[3], PINS_IC[15], PINS_IC[18]}; // A1, A2, A3, A4
const byte inputPinsB7486[] = {PINS_IC[1], PINS_IC[4], PINS_IC[14], PINS_IC[17]}; // B1, B2, B3, B4
const byte outputPins7486[] = {PINS_IC[2], PINS_IC[5], PINS_IC[13], PINS_IC[16]}; // Y1, Y2, Y3, Y4

const byte inputPinsA747266[] = {PINS_IC[0], PINS_IC[5], PINS_IC[13], PINS_IC[18]}; // A1, A2, A3, A4
const byte inputPinsB747266[] = {PINS_IC[1], PINS_IC[4], PINS_IC[14], PINS_IC[17]}; // B1, B2, B3, B4
const byte outputPins747266[] = {PINS_IC[2], PINS_IC[3], PINS_IC[15], PINS_IC[16]}; // Y1, Y2, Y3, Y4

// Global array to hold all IC test patterns
IC_TestPatterns testPatterns[] = {
  {"7400", 4, {
    "00H00HGH00H00V", // A = 0, B = 0, Y = 1
    "01H01HGH01H01V", // A = 0, B = 1, Y = 1
    "10H10HGH10H10V", // A = 1, B = 0, Y = 1
    "11L11LGL11L11V"  // A = 1, B = 1, Y = 0
  }, inputPinsA7400, inputPinsB7400, outputPins7400},
  
  {"7402", 4, {
    "00H00HGH00H00V", // A=0, B=0 -> Y=HIGH
    "01L01LGL01L01V", // A=0, B=1 -> Y=LOW
    "10L10LGL10L10V", // A=1, B=0 -> Y=LOW
    "11L11LGL11L11V"  // A=1, B=1 -> Y=LOW
  }, inputPinsA7402, inputPinsB7402, outputPins7402},

  {"7404", 2, {
    "0H0H0HGH0H0H0V", // A = 0, Y = 1
    "1L1L1LGL1L1L1V"  // A = 1, Y = 0
  }, inputPins7404, outputPins7404},

  {"7408", 4, {
    "00L00LGL00L00V",
    "01L01LGL01L01V",
    "10L10LGL10L10V",
    "11H11HGH11H11V"
  }, inputPinsA7408, inputPinsB7408, outputPins7408},

  {"7432", 4, {
    "00L00LGL00L00V", // A = 0, B = 0, Y = 0
    "01H01HGH01H01V", // A = 0, B = 1, Y = 1
    "10H10HGH10H10V", // A = 1, B = 0, Y = 1
    "11H11HGH11H11V"  // A = 1, B = 1, Y = 1
  }, inputPinsA7432, inputPinsB7432, outputPins7432},

  {"7486", 4, {
    "00L00LGL00L00V", // A = 0, B = 0, Y = 0
    "01H01HGH01H01V", // A = 0, B = 1, Y = 1
    "10H10HGH10H10V", // A = 1, B = 0, Y = 1
    "11L11LGL11L11V"  // A = 1, B = 1, Y = 0
  }, inputPinsA7486, inputPinsB7486, outputPins7486},

  {"747266", 4, {
    "00H00HGH00H00V", // A = 0, B = 0, Y = 1
    "01L01LGL01L01V", // A = 0, B = 1, Y = 0
    "10L10LGL10L10V", // A = 1, B = 0, Y = 0
    "11H11HGH11H11V"  // A = 1, B = 1, Y = 1
  }, inputPinsA747266, inputPinsB747266, outputPins747266}
  // Add more IC test patterns here as needed
};

// GLOBAL VARIABLES
byte menu = 1, submenu = 1, num = 1;
unsigned long lastMillis = 0; // Variable to store last time LED was updated

bool btnDownPressed = false;
bool btnUpPressed = false;
bool btnOkPressed = false;
bool btnCancelPressed = false;

// FUNCTIONS

void init_ic_pins()
{
  for(byte i=0;i<20;i++){
    pinMode(PINS_IC[i], INPUT);
  } 
}


void potreader()
{
  static unsigned long t;
  if (millis() - t < 50) return;
  t = millis();

  unsigned int a = 0;
  for (byte i = 0; i < 10; i++) {
    a += analogRead(PIN_POT);
  }
  a /= 10;
  
  // Map the analog value to the PWM range (0-255)
  int contrastValue = map(a, 0, 1023, 0, 255);

  // Write the contrast value to the LCD contrast pin
  analogWrite(PIN_LCD_CONTRAST, contrastValue);

  // Debug output
  Serial.print("Potentiometer Value: ");
  Serial.print(a);
  Serial.print(" - Mapped Contrast Value: ");
  Serial.println(contrastValue);

}

void heartbeatLED() {
  unsigned long currentMillis = millis();
  static unsigned long lastMillis = 0;
  static bool ledState = false;

  // Toggle heartbeat LED every second
  if (currentMillis - lastMillis > 1000) {
    lastMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(PIN_LED1, ledState);
    digitalWrite(PIN_LED1, !digitalRead(PIN_LED1)); // Toggle heartbeat LED every second
  }
}

void buttonDebounce(){
  static unsigned t; //for loop tracking
  static unsigned last_unpress[MAX_BUTTONS];

  if(millis() - t < 1)return;
  t = millis();

  for(byte i=0;i<MAX_BUTTONS;i++)
  {
    if(digitalRead(PINS_BUTTONS[i]) == LOW)
    {
      if(flag_button[i] == false)
      {
        if(millis() -last_unpress[i] > 5)
        {
          flag_button[i] = true;
          digitalWrite(PIN_BUZZ, HIGH);
          delay(50);
          digitalWrite(PIN_BUZZ, LOW);
          while(digitalRead(PINS_BUTTONS[i]) == LOW){};

        }
      }
    }
    else
    {
        last_unpress[i] = millis();
    }
  }
}

bool get_button_ok()
{
  for (byte i = 0; i < MAX_BUTTONS; i++) {
    if (flag_button[i]) {
      return true;  // Return true if any button is pressed
    }
  }
  return false;  // Return false if no button is pressed
}

void setup() {
  Serial.begin(9600);
  init_ic_pins();
  pinMode(PIN_BUZZ, OUTPUT);
  pinMode(PIN_BTN_UP, INPUT_PULLUP);
  pinMode(PIN_BTN_DOWN, INPUT_PULLUP);
  pinMode(PIN_BTN_LEFT, INPUT_PULLUP);
  pinMode(PIN_BTN_RIGHT, INPUT_PULLUP);
  pinMode(PIN_BTN_OK, INPUT_PULLUP);
  pinMode(PIN_BTN_CANCEL, INPUT_PULLUP);
  pinMode(PIN_RGBLED_R, OUTPUT);
  // pinMode(PIN_RGBLED_G, OUTPUT); 
  pinMode(PIN_RGBLED_B, OUTPUT);
  pinMode(PIN_LED1, OUTPUT);

  pinMode(PIN_PWM_P0, OUTPUT);
  pinMode(PIN_PWM_P1, OUTPUT);
  pinMode(PIN_PWM_P2, OUTPUT);
  pinMode(PIN_PWM_P3, OUTPUT);

  lcd.begin(16, 2);
 
  lcd.setCursor(0,0);lcd.print(F("  LC IC TESTER  "));
  lcd.setCursor(0,1);lcd.print(F("HW1.0.0  SW1.0.0"));
  

  Serial.println(F(">"));
  Serial.println(F("Hello"));
  delay(3000);
  update_menu();
}

void buttonScanner() {
  if (menu == 1 || menu == 2) {
    // Main menu navigation
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (menu > 1) menu--;
      update_menu();
    }
  
    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (menu < 2) menu++;
      update_menu();
    }
  
    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag
      if (menu == 2) {
        manual_user_interface(); // Enter submenu
        menu = 3; // Switch to submenu mode
      } else {
        execute_action();
      }
    }
  
    if (flag_button[5]) { // CANCEL button
      flag_button[5] = false; // Reset flag
      // Implement cancel action if needed
    }
  } else if (menu == 3) {
    // Submenu navigation
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (submenu > 1) submenu--;
      else submenu = 7; // Wrap around to last option
      manual_user_interface();
    }
  
    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenu < 7) submenu++;
      else submenu = 1; // Wrap around to first option
      manual_user_interface();
    }
  
    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag
      display_placeholder_text(); // Execute the selected IC test

      // Check if submenu option 4 (IC 7408) is selected
      if (submenu == 4) {
        testIC7408(); // Call the function to test IC 7408
      }
    }
  
    if (flag_button[5]) { // CANCEL button
      flag_button[5] = false; // Reset flag
      menu = 1; // Go back to main menu
      update_menu();
    }
  }
}

void update_menu() {
  switch (menu) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(">Automatic"));
      lcd.setCursor(0, 1);
      lcd.print(F(" Manual"));
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(" Automatic"));
      lcd.setCursor(0, 1);
      lcd.print(F(">Manual"));
      break;
    default:
      menu = 1;
      update_menu();
      break;
  }
}

//for manual 
void display_placeholder_text() {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (submenu) {
    case 1:
      lcd.print(F("Testing IC 7400"));
      // Call corresponding method to execute testing
      testIC7400();
      break;
    case 2:
      lcd.print(F("Testing IC 7402"));
      testIC7402();
      break;
    case 3:
      lcd.print(F("Testing IC 7404"));
      testIC7404();
      break;
    case 4:
      lcd.print(F("Testing IC 7408"));
      testIC7408();
      break;
    case 5:
      lcd.print(F("Testing IC 7432"));
      testIC7432();
      break;
    case 6:
      lcd.print(F("Testing IC 7486"));
      testIC7486();
      break;
    case 7:
      lcd.print(F("Testing IC 747266"));
      testIC747266();
      break;
  }
  lcd.setCursor(0, 1);
  lcd.print(F("Please wait..."));
}

void execute_action() {
  switch (menu) {
    case 1:
      // automaticUserInterface();
      // TO DO: Implement automatic through a different function
      break;
    case 2:
      manual_user_interface();
      num++;
      break;
  }

  // Check if submenu option 4 (IC 7408) is selected
  if (menu == 3 && submenu == 4) {
    testIC7408(); // Call the function to test IC 7408
  }
}


void manual_user_interface() {
  lcd.clear();
  switch (submenu) {
    case 1:
      lcd.print(F(">IC 7400        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 7402        "));
      break;
    case 2:
      lcd.print(F(" IC 7400        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7402        "));
      break;
    case 3:
      lcd.print(F(">IC 7404        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 7408        "));
      break;
    case 4:
      lcd.print(F(" IC 7404        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7408        "));
      break;
    case 5:
      lcd.print(F(">IC 7432        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 7486        "));
      break;
    case 6:
      lcd.print(F(" IC 7432        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7486        "));
      break;
    case 7:
      lcd.print(F(">IC 747266      "));
      break;
    default:
      submenu = 1;
      manual_user_interface();
      break;
  }
}



void testIC7400() {
  // Set VCC and GND pins
  const byte PIN_GND = PINS_IC[6];   // Pin 7
  const byte PIN_VCC = PINS_IC[19];  // Pin 14

  pinMode(PIN_GND, OUTPUT);
  pinMode(PIN_VCC, OUTPUT);
  digitalWrite(PIN_GND, LOW);    // GND to LOW
  digitalWrite(PIN_VCC, HIGH);   // VCC to HIGH

  // Retrieve the test pattern for IC7400
  // const char* icType = "7400";
  const IC_TestPatterns icPattern = testPatterns[0];

  // Loop through each gate (four gates in total)
  for (byte gate = 0; gate < 4; gate++) {
    Serial.print(F("Testing gate "));
    Serial.println(gate + 1);

    // Loop through each test case pattern
    for (byte test = 0; test < icPattern.numTestCases; test++) {
      // Parse the test pattern to set inputs and expected output
      byte inputA = icPattern.testPatterns[test][0] - '0';
      byte inputB = icPattern.testPatterns[test][1] - '0';
      byte expectedOutput = (icPattern.testPatterns[test][2] == 'L') ? LOW : HIGH;

      // Set the inputs A1, A2, A3, A4
      pinMode(icPattern.inputPinsA[gate], OUTPUT);
      pinMode(icPattern.inputPinsB[gate], OUTPUT);
      digitalWrite(icPattern.inputPinsA[gate], inputA); // WRITE TO IC
      digitalWrite(icPattern.inputPinsB[gate], inputB); // WRITE TO IC

      // Set the output pin Y1, Y2, Y3, Y4 as input to read the result
      pinMode(icPattern.outputPins[gate], INPUT);
      delay(10); // Short delay for stabilization

      // Read and compare the actual output
      byte actualOutput = digitalRead(icPattern.outputPins[gate]);
      Serial.print(F("A="));
      Serial.print(inputA);
      Serial.print(F(", B="));
      Serial.print(inputB);
      Serial.print(F(" => Y="));
      Serial.print(actualOutput);
      Serial.print(F(" (Expected: "));
      Serial.print(expectedOutput);
      Serial.print(F(") Gate: "));
      Serial.print(gate + 1);
      Serial.print(F(", Output Pin: "));
      Serial.print(gate + 1); // Y1, Y2, Y3, Y4
      Serial.print(F(" Result: "));
      // Print result
      if (actualOutput == expectedOutput) {
        Serial.println(F("OK"));
      } else {
        Serial.println(F("NG"));
      }
    }
    Serial.println();

    // Reset pin modes to INPUT to ensure no interference in the next gate test
    pinMode(icPattern.inputPinsA[gate], INPUT);
    pinMode(icPattern.inputPinsB[gate], INPUT);
    pinMode(icPattern.outputPins[gate], INPUT);
  }
}


// Repeat the same approach for other IC test functions like testIC7402, testIC7404, etc.
void testIC7402() {
  // Set VCC and GND pins
  const byte PIN_GND = PINS_IC[6];   // Pin 7
  const byte PIN_VCC = PINS_IC[19];  // Pin 14

  pinMode(PIN_GND, OUTPUT);
  pinMode(PIN_VCC, OUTPUT);
  digitalWrite(PIN_GND, LOW);    // GND to LOW
  digitalWrite(PIN_VCC, HIGH);   // VCC to HIGH

  // Retrieve the test pattern for IC7402
  // const char* icType = "7402";
  const IC_TestPatterns icPattern = testPatterns[1];

  // Loop through each gate (four gates in total)
  for (byte gate = 0; gate < 4; gate++) {
    Serial.print(F("Testing gate "));
    Serial.println(gate + 1);

    // Loop through each test case pattern
    for (byte test = 0; test < icPattern.numTestCases; test++) {
      // Parse the test pattern to set inputs and expected output
      byte inputA = icPattern.testPatterns[test][0] - '0';
      byte inputB = icPattern.testPatterns[test][1] - '0';
      byte expectedOutput = (icPattern.testPatterns[test][2] == 'L') ? LOW : HIGH;

      // Set the inputs A1, A2, A3, A4
      pinMode(icPattern.inputPinsA[gate], OUTPUT);
      pinMode(icPattern.inputPinsB[gate], OUTPUT);
      digitalWrite(icPattern.inputPinsA[gate], inputA);
      digitalWrite(icPattern.inputPinsB[gate], inputB);

      // Set the output pin Y1, Y2, Y3, Y4 as input to read the result
      pinMode(icPattern.outputPins[gate], INPUT);
      delay(10); // Short delay for stabilization

      // Read and compare the actual output
      byte actualOutput = digitalRead(icPattern.outputPins[gate]);
      Serial.print(F("A="));
      Serial.print(inputA);
      Serial.print(F(", B="));
      Serial.print(inputB);
      Serial.print(F(" => Y="));
      Serial.print(actualOutput);
      Serial.print(F(" (Expected: "));
      Serial.print(expectedOutput);
      Serial.print(F(") Gate: "));
      Serial.print(gate + 1);
      Serial.print(F(", Output Pin: "));
      Serial.print(gate + 1); // Y1, Y2, Y3, Y4
      Serial.print(F(" Result: "));
      // Print result
      if (actualOutput == expectedOutput) {
        Serial.println(F("OK"));
      } else {
        Serial.println(F("NG"));
      }
    }
    Serial.println();

    // Reset pin modes to INPUT to ensure no interference in the next gate test
    pinMode(icPattern.inputPinsA[gate], INPUT);
    pinMode(icPattern.inputPinsB[gate], INPUT);
    pinMode(icPattern.outputPins[gate], INPUT);
  }
}

void testIC7404() {
  // Set VCC and GND pins
  const byte PIN_GND = PINS_IC[6];   // Pin 7
  const byte PIN_VCC = PINS_IC[19];  // Pin 14

  pinMode(PIN_GND, OUTPUT);
  pinMode(PIN_VCC, OUTPUT);
  digitalWrite(PIN_GND, LOW);    // GND to LOW
  digitalWrite(PIN_VCC, HIGH);   // VCC to HIGH

  // Retrieve the test pattern for IC7400
  // const char* icType = "7404";
  const IC_TestPatterns icPattern = testPatterns[2];

  // Loop through each inverter (six in total)
  for (byte inverter = 0; inverter < 6; inverter++) {
    Serial.print(F("Testing inverter "));
    Serial.println(inverter + 1);

    // Loop through each test case pattern
    for (byte test = 0; test < icPattern.numTestCases; test++) {
      // Parse the test pattern to set inputs and expected output
      byte inputA = icPattern.testPatterns[test][0] - '0';
      byte expectedOutput = (icPattern.testPatterns[test][1] == 'L') ? LOW : HIGH;

      // Set the inputs A1, A2, A3, A4, A5, A6
      pinMode(inputPins7404[inverter], OUTPUT);
      digitalWrite(inputPins7404[inverter], inputA);

      // Set the output pins Y1, Y2, Y3, Y4, Y5, Y6 as inputs to read the result
      pinMode(outputPins7404[inverter], INPUT);
      delay(10); // Short delay for stabilization

      // Read and compare the actual output
      byte actualOutput = digitalRead(outputPins7404[inverter]);
      Serial.print(F("A="));
      Serial.print(inputA);
      Serial.print(F(" => Y="));
      Serial.print(actualOutput);
      Serial.print(F(" (Expected: "));
      Serial.print(expectedOutput);
      Serial.print(F(") Inverter: "));
      Serial.print(inverter + 1);
      Serial.print(F(", Output Pin: "));
      Serial.print(outputPins7404[inverter]);
      Serial.print(F(" Result: "));
      // Print result
      if (actualOutput == expectedOutput) {
        Serial.println(F("OK"));
      } else {
        Serial.println(F("NG"));
      }
    }
    Serial.println();

    // Reset pin modes to INPUT to ensure no interference in the next inverter test
    pinMode(inputPins7404[inverter], INPUT);
    pinMode(outputPins7404[inverter], INPUT);
  }
}

void testIC7408() {
  // Set VCC and GND pins
  const byte PIN_GND = PINS_IC[6];   // Pin 7
  const byte PIN_VCC = PINS_IC[19];  // Pin 14

  pinMode(PIN_GND, OUTPUT);
  pinMode(PIN_VCC, OUTPUT);
  digitalWrite(PIN_GND, LOW);    // GND to LOW
  digitalWrite(PIN_VCC, HIGH);   // VCC to HIGH

  // Retrieve the test pattern for IC7402
  // const char* icType = "7402";
  const IC_TestPatterns icPattern = testPatterns[3];

  // Loop through each gate (four gates in total)
  for (byte gate = 0; gate < 4; gate++) {
    Serial.print(F("Testing gate "));
    Serial.println(gate + 1);

    // Loop through each test case pattern
    for (byte test = 0; test < icPattern.numTestCases; test++) {
      // Parse the test pattern to set inputs and expected output
      byte inputA = icPattern.testPatterns[test][0] - '0';
      byte inputB = icPattern.testPatterns[test][1] - '0';
      byte expectedOutput = (icPattern.testPatterns[test][2] == 'L') ? LOW : HIGH;

      // Set the inputs A1, A2, A3, A4
      pinMode(icPattern.inputPinsA[gate], OUTPUT);
      pinMode(icPattern.inputPinsB[gate], OUTPUT);
      digitalWrite(icPattern.inputPinsA[gate], inputA);
      digitalWrite(icPattern.inputPinsB[gate], inputB);

      // Set the output pin Y1, Y2, Y3, Y4 as input to read the result
      pinMode(icPattern.outputPins[gate], INPUT);
      delay(10); // Short delay for stabilization

      // Read and compare the actual output
      byte actualOutput = digitalRead(icPattern.outputPins[gate]);
      Serial.print(F("A="));
      Serial.print(inputA);
      Serial.print(F(", B="));
      Serial.print(inputB);
      Serial.print(F(" => Y="));
      Serial.print(actualOutput);
      Serial.print(F(" (Expected: "));
      Serial.print(expectedOutput);
      Serial.print(F(") Gate: "));
      Serial.print(gate + 1);
      Serial.print(F(", Output Pin: "));
      Serial.print(gate + 1); // Y1, Y2, Y3, Y4
      Serial.print(F(" Result: "));
      // Print result
      if (actualOutput == expectedOutput) {
        Serial.println(F("OK"));
      } else {
        Serial.println(F("NG"));
      }
    }
    Serial.println();

    // Reset pin modes to INPUT to ensure no interference in the next gate test
    pinMode(icPattern.inputPinsA[gate], INPUT);
    pinMode(icPattern.inputPinsB[gate], INPUT);
    pinMode(icPattern.outputPins[gate], INPUT);
  }
}

void testIC7432() {
  // Set VCC and GND pins
  const byte PIN_GND = PINS_IC[6];   // Pin 7
  const byte PIN_VCC = PINS_IC[19];  // Pin 14

  pinMode(PIN_GND, OUTPUT);
  pinMode(PIN_VCC, OUTPUT);
  digitalWrite(PIN_GND, LOW);    // GND to LOW
  digitalWrite(PIN_VCC, HIGH);   // VCC to HIGH

  // Retrieve test pattern and pin configurations for IC7432 from testPatterns array
  const IC_TestPatterns patterns = testPatterns[4]; // Index 4 corresponds to IC7432

  // Loop through each gate (four gates in total)
  for (byte gate = 0; gate < 4; gate++) {
    Serial.print(F("Testing gate "));
    Serial.println(gate + 1);

    // Loop through each test case pattern
    for (byte test = 0; test < patterns.numTestCases; test++) {
      // Parse the test pattern to set inputs and expected output
      byte inputA = patterns.testPatterns[test][0] - '0';
      byte inputB = patterns.testPatterns[test][1] - '0';
      byte expectedOutput = (patterns.testPatterns[test][2] == 'L') ? LOW : HIGH;

      // Set the inputs A1, A2, A3, A4
      pinMode(patterns.inputPinsA[gate], OUTPUT);
      pinMode(patterns.inputPinsB[gate], OUTPUT);
      digitalWrite(patterns.inputPinsA[gate], inputA);
      digitalWrite(patterns.inputPinsB[gate], inputB);

      // Set the output pins Y1, Y2, Y3, Y4 as inputs to read the result
      pinMode(patterns.outputPins[gate], INPUT);
      delay(10); // Short delay for stabilization

      // Read and compare the actual output
      byte actualOutput = digitalRead(patterns.outputPins[gate]);
      Serial.print(F("A="));
      Serial.print(inputA);
      Serial.print(F(", B="));
      Serial.print(inputB);
      Serial.print(F(" => Y="));
      Serial.print(actualOutput);
      Serial.print(F(" (Expected: "));
      Serial.print(expectedOutput);
      Serial.print(F(") Gate: "));
      Serial.print(gate + 1);
      Serial.print(F(", Output Pin: "));
      Serial.print(patterns.outputPins[gate]);
      Serial.print(F(" Result: "));
      // Print result
      if (actualOutput == expectedOutput) {
        Serial.println(F("OK"));
      } else {
        Serial.println(F("NG"));
      }
    }
    Serial.println();

    // Reset pin modes to INPUT to ensure no interference in the next gate test
    pinMode(patterns.inputPinsA[gate], INPUT);
    pinMode(patterns.inputPinsB[gate], INPUT);
    pinMode(patterns.outputPins[gate], INPUT);
  }
}

void testIC7486() {
  // Set VCC and GND pins
  const byte PIN_GND = PINS_IC[6];   // Pin 7
  const byte PIN_VCC = PINS_IC[19];  // Pin 14

  pinMode(PIN_GND, OUTPUT);
  pinMode(PIN_VCC, OUTPUT);
  digitalWrite(PIN_GND, LOW);    // GND to LOW
  digitalWrite(PIN_VCC, HIGH);   // VCC to HIGH

  // Retrieve test pattern and pin configurations for IC7486 from testPatterns array
  const IC_TestPatterns patterns = testPatterns[5]; // Index 5 corresponds to IC7486

  // Loop through each gate (four gates in total)
  for (byte gate = 0; gate < 4; gate++) {
    Serial.print(F("Testing gate "));
    Serial.println(gate + 1);

    // Loop through each test case pattern
    for (byte test = 0; test < patterns.numTestCases; test++) {
      // Parse the test pattern to set inputs and expected output
      byte inputA = patterns.testPatterns[test][0] - '0';
      byte inputB = patterns.testPatterns[test][1] - '0';
      byte expectedOutput = (patterns.testPatterns[test][2] == 'L') ? LOW : HIGH;

      // Set the inputs A1, A2, A3, A4
      pinMode(patterns.inputPinsA[gate], OUTPUT);
      pinMode(patterns.inputPinsB[gate], OUTPUT);
      digitalWrite(patterns.inputPinsA[gate], inputA);
      digitalWrite(patterns.inputPinsB[gate], inputB);

      // Set the output pins Y1, Y2, Y3, Y4 as inputs to read the result
      pinMode(patterns.outputPins[gate], INPUT);
      delay(10); // Short delay for stabilization

      // Read and compare the actual output
      byte actualOutput = digitalRead(patterns.outputPins[gate]);
      Serial.print(F("A="));
      Serial.print(inputA);
      Serial.print(F(", B="));
      Serial.print(inputB);
      Serial.print(F(" => Y="));
      Serial.print(actualOutput);
      Serial.print(F(" (Expected: "));
      Serial.print(expectedOutput);
      Serial.print(F(") Gate: "));
      Serial.print(gate + 1);
      Serial.print(F(", Output Pin: "));
      Serial.print(patterns.outputPins[gate]);
      Serial.print(F(" Result: "));
      // Print result
      if (actualOutput == expectedOutput) {
        Serial.println(F("OK"));
      } else {
        Serial.println(F("NG"));
      }
    }
    Serial.println();

    // Reset pin modes to INPUT to ensure no interference in the next gate test
    pinMode(patterns.inputPinsA[gate], INPUT);
    pinMode(patterns.inputPinsB[gate], INPUT);
    pinMode(patterns.outputPins[gate], INPUT);
  }
}

void testIC747266() {
  // Set VCC and GND pins
  const byte PIN_GND = PINS_IC[6];   // Pin 7
  const byte PIN_VCC = PINS_IC[19];  // Pin 14

  pinMode(PIN_GND, OUTPUT);
  pinMode(PIN_VCC, OUTPUT);
  digitalWrite(PIN_GND, LOW);    // GND to LOW
  digitalWrite(PIN_VCC, HIGH);   // VCC to HIGH

  // Retrieve test pattern and pin configurations for IC747266 from testPatterns array
  const IC_TestPatterns patterns = testPatterns[6]; // Index 6 corresponds to IC747266

  // Loop through each gate (four gates in total)
  for (byte gate = 0; gate < 4; gate++) {
    Serial.print(F("Testing gate "));
    Serial.println(gate + 1);

    // Loop through each test case pattern
    for (byte test = 0; test < patterns.numTestCases; test++) {
      // Parse the test pattern to set inputs and expected output
      byte inputA = patterns.testPatterns[test][0] - '0';
      byte inputB = patterns.testPatterns[test][1] - '0';
      byte expectedOutput = (patterns.testPatterns[test][2] == 'L') ? LOW : HIGH;

      // Set the inputs A1, A2, A3, A4
      pinMode(patterns.inputPinsA[gate], OUTPUT);
      pinMode(patterns.inputPinsB[gate], OUTPUT);
      digitalWrite(patterns.inputPinsA[gate], inputA);
      digitalWrite(patterns.inputPinsB[gate], inputB);

      // Set the output pins Y1, Y2, Y3, Y4 as inputs to read the result
      pinMode(patterns.outputPins[gate], INPUT);
      delay(10); // Short delay for stabilization

      // Read and compare the actual output
      byte actualOutput = digitalRead(patterns.outputPins[gate]);
      Serial.print(F("A="));
      Serial.print(inputA);
      Serial.print(F(", B="));
      Serial.print(inputB);
      Serial.print(F(" => Y="));
      Serial.print(actualOutput);
      Serial.print(F(" (Expected: "));
      Serial.print(expectedOutput);
      Serial.print(F(") Gate: "));
      Serial.print(gate + 1);
      Serial.print(F(", Output Pin: "));
      Serial.print(patterns.outputPins[gate]);
      Serial.print(F(" Result: "));
      // Print result
      if (actualOutput == expectedOutput) {
        Serial.println(F("OK"));
      } else {
        Serial.println(F("NG"));
      }
    }
    Serial.println();

    // Reset pin modes to INPUT to ensure no interference in the next gate test
    pinMode(patterns.inputPinsA[gate], INPUT);
    pinMode(patterns.inputPinsB[gate], INPUT);
    pinMode(patterns.outputPins[gate], INPUT);
  }
}

void loop() {
  buttonDebounce();
  buttonScanner();
  //potreader();
  //heartbeatLED();
}
