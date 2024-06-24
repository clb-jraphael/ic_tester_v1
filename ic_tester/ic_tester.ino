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
  PIN_IC_PIN1, PIN_IC_PIN2, PIN_IC_PIN3,
  PIN_IC_PIN4, PIN_IC_PIN5, PIN_IC_PIN6, 
  PIN_IC_PIN7, PIN_IC_PIN8, PIN_IC_PIN9, 
  PIN_IC_PIN10, PIN_IC_PIN11, PIN_IC_PIN12, 
  PIN_IC_PIN13, PIN_IC_PIN14, PIN_IC_PIN15,
  PIN_IC_PIN16, PIN_IC_PIN17, PIN_IC_PIN18,
  PIN_IC_PIN19, PIN_IC_PIN20
};

const byte PINS_14[14] = {
  PIN_IC_PIN1, PIN_IC_PIN2, PIN_IC_PIN3, PIN_IC_PIN4, PIN_IC_PIN5, PIN_IC_PIN6, PIN_IC_PIN7,
  PIN_IC_PIN14, PIN_IC_PIN15, PIN_IC_PIN16, PIN_IC_PIN17, PIN_IC_PIN18, PIN_IC_PIN19, PIN_IC_PIN20
};

const byte PINS_16[16] = {
  PIN_IC_PIN1, PIN_IC_PIN2, PIN_IC_PIN3, PIN_IC_PIN4, PIN_IC_PIN5, PIN_IC_PIN6, PIN_IC_PIN7, PIN_IC_PIN8,
  PIN_IC_PIN13, PIN_IC_PIN14, PIN_IC_PIN15, PIN_IC_PIN16, PIN_IC_PIN17, PIN_IC_PIN18, PIN_IC_PIN19, PIN_IC_PIN20
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
  const char* icType;      // IC type (e.g., "7400", "7402", etc.)
  byte pinCount;
  byte numTestCases;        // Number of test cases for this IC
  const char* testPatterns[36]; // Array to hold test patterns (adjust size as needed)
};

// Define your IC test patterns with pin counts and test cases
IC_TestPatterns testPatterns[] = {
  {"7400", 14, 4, {
    "00H00HGH00H00V", // A = 0, B = 0, Y = 1
    "01H01HGH01H01V", // A = 0, B = 1, Y = 1
    "10H10HGH10H10V", // A = 1, B = 0, Y = 1
    "11L11LGL11L11V"  // A = 1, B = 1, Y = 0
  }},
  
  {"7402", 14, 4, {
    "00H00HGH00H00V", // A=0, B=0 -> Y=HIGH
    "01L01LGL01L01V", // A=0, B=1 -> Y=LOW
    "10L10LGL10L10V", // A=1, B=0 -> Y=LOW
    "11L11LGL11L11V"  // A=1, B=1 -> Y=LOW
  }},

  {"7404", 14, 2, {
    "0H0H0HGH0H0H0V", // A = 0, Y = 1
    "1L1L1LGL1L1L1V"  // A = 1, Y = 0
  }},

  {"7408", 14, 4, {
    "00L00LGL00L00V",
    "01L01LGL01L01V",
    "10L10LGL10L10V",
    "11H11HGH11H11V"
  }},

  {"7432", 14, 4, {
    "00L00LGL00L00V", // A = 0, B = 0, Y = 0
    "01H01HGH01H01V", // A = 0, B = 1, Y = 1
    "10H10HGH10H10V", // A = 1, B = 0, Y = 1
    "11H11HGH11H11V"  // A = 1, B = 1, Y = 1
  }},

  {"7486", 14, 4, {
    "00L00LGL00L00V", // A = 0, B = 0, Y = 0
    "01H01HGH01H01V", // A = 0, B = 1, Y = 1
    "10H10HGH10H10V", // A = 1, B = 0, Y = 1
    "11L11LGL11L11V"  // A = 1, B = 1, Y = 0
  }},

  {"747266", 14, 4, {
    "00H00HGH00H00V", // A = 0, B = 0, Y = 1
    "01L01LGL01L01V", // A = 0, B = 1, Y = 0
    "10L10LGL10L10V", // A = 1, B = 0, Y = 0
    "11H11HGH11H11V"  // A = 1, B = 1, Y = 1
  }},

  {"7427", 14, 8, {
    "00000HGH000H0V", 
    "00100LGL100L1V", 
    "10010LGL010L0V", 
    "10110LGL110L1V",
    "01001LGL001L0V",
    "10101LGL101L1V",
    "11011LGL011L0V",
    "11111LGL111L1V"
  }},

  // Add more IC test patterns here as needed
  {"4070", 14, 4, {
    "00LL00G00LL00V", // A = 0, B = 0, Y = 1
    "10HH10G10HH10V", // A = 0, B = 1, Y = 0
    "01HH01G01HH01V", // A = 1, B = 0, Y = 0
    "11LL11G11LL11V"  // A = 1, B = 1, Y = 1
  }},

  {"4081", 14, 4, {
    "00LH11G11HL00V",
    "10LL10G10LL10V",
    "01LL01G01LL01V",
    "11HL00G00LH11V"
  }},

  {"7421", 14, 4, {
    "11X11HGH11X11V",
    "01X01LGL01X01V",
    "10X10LGL10X10V",
    "00X00LGL00X00V"
  }},

  {"4077", 14, 4, {
    "00HH00G00HH00V",
    "10LL10G10LL10V",
    "01LL01G01LL01V",
    "11HH11G11HH11V"
  }},

  {"4068", 14, 10, {
    "011110G01111LV",
    "010100G01010HV",
    "001010G00101HV",
    "000110G00011HV",
    "011000G01100HV",
    "000000G01111HV",
    "000000G00000HV",
    "011110G01110HV",
    "011010G01111HV",
    "001110G00111HV"
  }},

  {"4069", 14, 2, {
    "0H0H0HGH0H0H0V",
    "1L1L1LGL1L1L1V"
  }},

  {"4066", 14, 4, {
    "0HH000G0HH000V",
    "1HH100G1HH100V",
    "0LL011G0LL011V",
    "1HH111G1HH111V"
  }},

  {"7474", 14, 8, {
    "01C1LHGHL1000V",
    "10C0HLGHL1000V",
    "10C1LHGHL1000V",
    "11C1HLGHL1000V",
    "0001LHGHL1C10V",
    "0001LHGLH0C01V",
    "0001LHGHL1C01V",
    "0001LHGLH1C11V"
  }},

  {"4066", 14, 4, {
    "0HH000G0HH000V",
    "1HH100G1HH100V",
    "0LL011G0LL011V",
    "1HH111G1HH111V"
  }},

  {"4094", 16, 14, {
    "1X0XXXXGXXXXXX1V",
    "11CHXXXGXXXXXX1V",
    "10CLHXXGXXXXXX1V",
    "11CHLHXGXXXXXX1V",
    "10CLHLHGXXXXXX1V",
    "11CHLHLGXXXXXH1V",
    "10CLHLHGXXXXHL1V",
    "11CHLHLGXXXHLH1V",
    "10CLHLHGXXHLHL1V",
    "11CHLHLGLXLHLH1V",
    "10CLHLHGHXHLHL1V",
    "01CLHLHGLXHLHL1V",
    "110HLHLGLXLHLH1V",
    "100HHHHGLXHHHH0V"
  }},

  {"74151", 16, 18, {
    "0101LH1G1010101V",
    "1010LH1G0101010V",
    "1110LH0G0001111V",
    "0001HL0G0000000V",
    "1101LH0G0011111V",
    "0010HL0G0010000V",
    "1011LH0G0101111V",
    "0100HL0G0100000V",
    "0111LH0G0111111V",
    "1000HL0G0110000V",
    "1111LH0G1001110V",
    "0000HL0G1000001V",
    "1111LH0G1011101V",
    "0000HL0G1010010V",
    "1111LH0G1101011V",
    "0000HL0G1100100V",
    "1111LH0G1110111V",
    "0000HL0G1111000V"
  }}
};

// GLOBAL VARIABLES
byte menu = 1, submenu = 1, submenuAuto = 1, num = 1, currentModelIndex = 0, passedCount = 0;
String passedModels[5];
unsigned long lastMillis = 0; // Variable to store last time LED was updated

bool btnDownPressed = false;
bool btnUpPressed = false;
bool btnOkPressed = false;
bool btnCancelPressed = false;

// FUNCTIONS

void init_ic_pins(){
  for(byte i=0;i<20;i++){
    pinMode(PINS_IC[i], INPUT);
  } 
}

void potreader(){
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

bool get_button_ok(){
  for (byte i = 0; i < MAX_BUTTONS; i++) {
    if (flag_button[i]) {
      return true;  // Return true if any button is pressed
    }
  }
  return false;  // Return false if no button is pressed
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
      if (menu == 1) {
        automatic_user_interface(); // Enter submenu
        menu = 4; // Switch to submenu mode
      } else if (menu == 2) {
        manual_user_interface();
        menu = 3;
        // execute_action();
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
      else submenu = 13; // Wrap around to last option
      manual_user_interface();
    }
  
    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenu < 13) submenu++;
      else submenu = 1; // Wrap around to first option
      manual_user_interface();
    }
  
    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag
      display_placeholder_text(); // Execute the selected IC test
    }
  
    if (flag_button[5]) { // CANCEL button
      flag_button[5] = false; // Reset flag
      menu = 1; // Go back to main menu
      update_menu();
    }
  } else if (menu == 4) {
    // Submenu navigation
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (submenuAuto > 1) submenuAuto--;
      else submenuAuto = 2; // Wrap around to last option
      automatic_user_interface();
    }
  
    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenuAuto < 2) submenuAuto++;
      else submenuAuto = 1; // Wrap around to first option
      automatic_user_interface();
    }
  
    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag
      automatic_options(); // Execute the selected IC test
    }
  
    if (flag_button[5]) { // CANCEL button
      flag_button[5] = false; // Reset flag
      menu = 1; // Go back to main menu
      update_menu();
    }
  } else if (menu == 5) {
    // Scroll through passed IC models
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (currentModelIndex > 0) currentModelIndex--;
      updatePassedModelsDisplay();
    }
  
    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (currentModelIndex < passedCount - 1) currentModelIndex++;
      updatePassedModelsDisplay();
    }

    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag
      menu = 4; // Go back to automatic submenu
      automatic_user_interface();
    }
    
    if (flag_button[5]) {
      flag_button[5] = false;
      menu = 4;
      automatic_user_interface();
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

void automatic_options() {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (submenuAuto) {
    case 1:
      lcd.print(F("Please wait...  "));
      autoSearch(14);
      break;
    case 2:
      lcd.print(F("Please wait...  "));
      autoSearch(16);
      break;
  }
}

void display_placeholder_text() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  if (submenu >= 1 && submenu <= 9) {
    lcd.print(F("Testing IC "));
    lcd.print(testPatterns[submenu - 1].icType); // Assuming icType is the field holding IC name/type
    bool result = testIC(testPatterns[submenu - 1], PINS_14);
    
    lcd.setCursor(0, 1);
    if (result) {
      lcd.print(F("Test Passed"));
    } else {
      lcd.print(F("Test Failed"));
    }
  }

  // After testing, allow navigation back to submenu
  manual_user_interface();
}


void execute_action() {
  switch (menu) {
    case 1:
      // automaticUserInterface();
      // TO DO: Implement automatic through a different function
      // automatic_testing();
      automatic_user_interface();
      break;
    case 2:
      manual_user_interface();
      num++;
      break;
  }
}

void automatic_user_interface() {
  lcd.clear();
  switch (submenuAuto) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(">14-PIN IC      "));
      lcd.setCursor(0, 1);
      lcd.print(F(" 16-PIN IC      "));
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(" 14-PIN IC      "));
      lcd.setCursor(0, 1);
      lcd.print(F(">16-PIN IC      "));
      break;
    default:
      submenuAuto = 1;
      automatic_user_interface();
      break;
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
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 4070        "));
      break;
    case 8:
      lcd.print(F(" IC 747266      "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 4070        "));
      break;
    case 9:
      lcd.print(F(">IC 4081        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 7421        "));
      break;
    case 10:
      lcd.print(F(" IC 4081        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7421        "));
      break;
    case 11:
      lcd.print(F(">IC 4077        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 4068        "));
      break;
    case 12:
      lcd.print(F(" IC 4077        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 4068        "));
      break;
    case 13:
      lcd.print(F(">IC 4069        "));
      lcd.setCursor(0, 1);
      lcd.print(F("                "));
      break;
    default:
      submenu = 1;
      manual_user_interface();
      break;
  }
}

void updatePassedModelsDisplay() {
  lcd.clear();
  if (passedCount > 0) {
    lcd.setCursor(0, 0);
    lcd.print("Passed Models:");
    lcd.setCursor(0, 1);
    lcd.print(passedModels[currentModelIndex]);
  } else {
    lcd.setCursor(0, 0);
    lcd.print("No models passed");
  }
}

void configurePins(const char* testPattern, const byte* pins, int pinCount) {
  for (int i = 0; i < pinCount; i++) {
    switch (testPattern[i]) {
      case 'V':
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], HIGH);
        break;
      case 'G':
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], LOW);
        break;
      case 'L':
        pinMode(pins[i], INPUT);
        digitalWrite(pins[i], LOW); // Ensure pull-down for low impedance
        break;
      case 'H':
        pinMode(pins[i], INPUT);
        digitalWrite(pins[i], HIGH); // Ensure pull-up for high impedance
        break;
      case '0':
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], LOW);
        break;
      case '1':
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], HIGH);
        break;
      case 'X':
        pinMode(pins[i], INPUT);
        digitalWrite(pins[i], LOW); // Floating input
        break;
      case 'C':
        // Handle clock pin if needed, currently not used in this example
        break;
      default:
        break;
    }
  }
}

bool testIC(const IC_TestPatterns & icPattern, const byte* pins) {
  bool allTestsPassed = true;

  // Display "Please wait..." on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Please wait..."));

  // Loop through each test case pattern
  for (int test = 0; test < icPattern.numTestCases; test++) {
    // Set pins according to the current test pattern
    const char* testPattern = icPattern.testPatterns[test];
    int pinCount = icPattern.pinCount; // Get the pin count for this IC test pattern

    configurePins(testPattern, pins, pinCount);

    // Delay for stabilization
    delay(10);

    // Read and compare the actual outputs
    Serial.print(F("Testing gate "));
    Serial.println(test + 1);

    for (int gate = 0; gate < pinCount; gate++) {
      byte expectedOutput = (testPattern[gate] == 'L' || testPattern[gate] == '0') ? LOW : HIGH;
      if (testPattern[gate] == 'L' || testPattern[gate] == 'H') {
        pinMode(pins[gate], INPUT);
        byte actualOutput = digitalRead(pins[gate]);
        Serial.print(F("A => "));
        Serial.print(testPattern[0] == '0' ? "0" : "1");
        Serial.print(F(" B => "));
        Serial.print(testPattern[1] == '0' ? "0" : "1");
        Serial.print(F(" Y => "));
        Serial.print(actualOutput);
        Serial.print(F(" Expected Output ["));
        Serial.print(expectedOutput);
        Serial.print(F("] : "));

        if (actualOutput == expectedOutput) {
          Serial.println(F("PASS"));
        } else {
          Serial.println(F("FAIL"));
          allTestsPassed = false;
        }
      } else {
        continue;
      }
    }

    // Small delay between tests
    delay(5);
  }

  return allTestsPassed;
}

byte detectNumPins() {
  byte detectedPins = 0;

  // Set all pins to INPUT_PULLUP
  for (byte i = 0; i < 20; i++) {
    pinMode(PINS_IC[i], INPUT_PULLUP);
  }

  // Check for connected pins
  for (byte i = 0; i < 20; i++) {
    // Temporarily set pin as OUTPUT and LOW
    pinMode(PINS_IC[i], OUTPUT);
    digitalWrite(PINS_IC[i], LOW);
    delay(10);

    // Check if this pin is connected to any other pin (which would pull it LOW)
    for (byte j = 0; j < 20; j++) {
      if (i != j && digitalRead(PINS_IC[j]) == LOW) {
        detectedPins++;
        break;
      }
    }

    // Restore the pin to INPUT_PULLUP
    pinMode(PINS_IC[i], INPUT_PULLUP);
    delay(10);
  }

  return detectedPins;
}

boolean testCase(String test, byte pins)
{
  if (pins == 14) {
  boolean result = true;
  int clkPin = -1;

  Serial.println("SignalIn : " + test);
  Serial.print("Response : ");

  //Setting Vcc, GND and INPUTS
  for (int i = 0; i < pins; i++)
  {
    switch (test[i])
    {
      case 'V' : pinMode(PINS_14[i], OUTPUT); digitalWrite(PINS_14[i], HIGH);
        break;
      case 'G' : pinMode(PINS_14[i], OUTPUT); digitalWrite(PINS_14[i], LOW);
        break;
      case 'L' : digitalWrite(PINS_14[i], LOW); pinMode(PINS_14[i], INPUT_PULLUP);
        break;
      case 'H' : digitalWrite(PINS_14[i], LOW); pinMode(PINS_14[i], INPUT_PULLUP);
        break;
    }
  }

  delay(5);

  //Setting Input Signals
  for (int i = 0; i < pins; i++)
  {
    switch (test[i])
    {
      case 'X' :
      case '0' : pinMode(PINS_14[i], OUTPUT); digitalWrite(PINS_14[i], LOW);
        break;
      case '1' : pinMode(PINS_14[i], OUTPUT); digitalWrite(PINS_14[i], HIGH);
        break;
      case 'C' : clkPin = PINS_14[i]; pinMode(PINS_14[i], OUTPUT); digitalWrite(PINS_14[i], LOW);
        break;
    }
  }

  if (clkPin != -1)
  {
    //Clock Trigger
    pinMode(clkPin, INPUT_PULLUP);
    delay(10);
    pinMode(clkPin, OUTPUT);
    digitalWrite(clkPin, LOW);
  }

  delay(5);

  //Reading Outputs
  for (int i = 0; i < pins; i++)
  {
    switch (test[i])
    {
      case 'H' : if (!digitalRead(PINS_14[i])) {
          result = false;
          Serial.print('L');
        }
        else Serial.print(' ');
        break;
      case 'L' : if (digitalRead(PINS_14[i])) {
          result = false;
          Serial.print('H');
        }
        else Serial.print(' ');
        break;
      default : Serial.print(' ');
    }
  }
  Serial.println(";");
  //Serial.println("\nCase Result : "+String(result));
  return result;
  }
}

void autoSearch(byte pins) {
  passedCount = 0; // Reset passed count
  byte size_db = sizeof(testPatterns) / sizeof(testPatterns[0]);
  for (byte i = 0; i < size_db; i++) {
    bool overallResult = true;
    if (testPatterns[i].pinCount == pins) {
      Serial.println("\nTesting IC Model: " + String(testPatterns[i].icType));
      for (int j = 0; j < testPatterns[i].numTestCases; j++) {
        if (!testCase(testPatterns[i].testPatterns[j], pins)) {
          overallResult = false;
        }
      }
      if (overallResult) {
        passedModels[passedCount++] = testPatterns[i].icType;
        Serial.println("IC Model " + String(testPatterns[i].icType) + " passed all tests.\n");
      } else {
        Serial.println("IC Model " + String(testPatterns[i].icType) + " failed.\n");
      }
    }
  }

  if (passedCount > 0) {
    currentModelIndex = 0; // Reset to first model
    menu = 5; // Set menu to passed models display
    updatePassedModelsDisplay();
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("No models passed");
  }
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

void loop() {
  buttonDebounce();
  buttonScanner();
  //potreader();
  //heartbeatLED();
  //byte numPins = detectNumPins();
  //Serial.print("Detected number of pins: ");
  //Serial.println(numPins);
  //delay(1000); // Wait 5 seconds before checking again
}
