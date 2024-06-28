/**
 * @file IC_Tester.ino
 * @brief Arduino-based IC Tester with automatic and manual testing modes.
 * 
 * This project is an IC tester using an Arduino, an LCD screen, and buttons for navigation.
 * It supports both automatic and manual IC testing and displays results on an LCD screen.
 * 
 * @version 1.0
 * @date 2024-06-25
 */

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

const byte PIN_RGBLED_R = 6;
const byte PIN_RGBLED_G = 4;
const byte PIN_RGBLED_B = 2;
const byte PIN_LED1 = 8;

const int rs = A10, en = A12, d4 = 28, d5 = 29, d6 = 30, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//pin configurations 
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
const byte PINS_8[8] {
  PIN_IC_PIN1, PIN_IC_PIN2, PIN_IC_PIN3, PIN_IC_PIN4, PIN_IC_PIN17, PIN_IC_PIN18, PIN_IC_PIN19, PIN_IC_PIN20
};

// Defined structure to hold IC test patterns
struct IC_TestPatterns {
  const char* icType;             // IC type (e.g., "7400", "7402", etc.)
  byte pinCount;                  // 20, 8, 14, 16-pin, etc.
  byte numTestCases;              // Number of test cases for this IC
  const char* testPatterns[36];   // Array to hold test patterns (adjust size as needed)
};

// GLOBAL VARIABLES

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

byte menu = 1, submenu = 1, submenuAuto = 1, num = 1, currentModelIndex = 0, passedCount = 0;
String passedModels[5];
unsigned long lastMillis = 0; // Variable to store last time LED was updated

bool btnDownPressed = false;
bool btnUpPressed = false;
bool btnOkPressed = false;
bool btnCancelPressed = false;

// Test pattern strings stored in PROGMEM
const char str_7400_1[] PROGMEM = "00H00HGH00H00V";
const char str_7400_2[] PROGMEM = "01H01HGH01H01V";
const char str_7400_3[] PROGMEM = "10H10HGH10H10V";
const char str_7400_4[] PROGMEM = "11L11LGL11L11V";
const char* const testPatterns_7400[] PROGMEM = {str_7400_1, str_7400_2, str_7400_3, str_7400_4};

// IC model strings stored in PROGMEM
const char ic_model_7400[] PROGMEM = "7400";

// Pin counts and test case numbers stored in PROGMEM
const byte pinCounts[] PROGMEM = {14};
const byte numTestCases[] PROGMEM = {4};

// Define an array of IC_TestPatterns, using PROGMEM for the array itself
const struct IC_TestPattern {
  const char* icType;
  const byte* pinCount;
  const byte* numTestCases;
  const char* const* testPatterns;
} testPatterns[] PROGMEM = {
  {reinterpret_cast<const char*>(ic_model_7400), pinCounts, numTestCases, testPatterns_7400}
};

// FUNCTIONS

/**
 * Initializes IC pins by setting each pin in the PINS_IC array to INPUT mode.
 */
void init_ic_pins(){
  for(byte i=0;i<20;i++){
    pinMode(PINS_IC[i], INPUT);
  } 
}

//core logic
/**
 * Configures the specified pins of an IC according to a test pattern,
 * triggers clock signals if necessary, and verifies the output pins.
 * 
 * @param test A string representing the test pattern for the IC:
 *        'V' - Set pin to HIGH (Vcc)
 *        'G' - Set pin to LOW (Ground)
 *        'L' - Set pin as input with pull-up, expect LOW
 *        'H' - Set pin as input with pull-up, expect HIGH
 *        '0' - Set pin to LOW (output)
 *        '1' - Set pin to HIGH (output)
 *        'C' - Clock pin
 *        'X' - Ignore pin
 * @param pins An array of pin numbers corresponding to the test pattern.
 * @param pinCount The number of pins to be tested.
 * @return True if the outputs match the expected values, false otherwise.
 */
boolean testCase(PGM_P test, const byte* pins, int pinCount) {
  boolean result = true;
  const int MAX_CLK_PINS = 2;
  int clkPins[MAX_CLK_PINS];
  int clkPinCount = 0;

  Serial.println("SignalIn : " + String((const __FlashStringHelper*)test));
  Serial.print("Response : ");

  // Phase 1: Pin Configuration
  for (int i = 0; i < pinCount; i++) {
    char c = pgm_read_byte(test + i);
    switch (c) {
      case 'V':
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], HIGH);
        break;
      case 'G':
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], LOW);
        break;
      case 'L':
        digitalWrite(pins[i], LOW);
        pinMode(pins[i], INPUT_PULLUP);
        break;
      case 'H':
        digitalWrite(pins[i], HIGH);
        pinMode(pins[i], INPUT_PULLUP);
        break;
      case '~':
        pinMode(pins[i], INPUT);
        break;
    }
  }

  delay(5);

  // Phase 2: Special Pin Configurations ('X', '0', '1', 'C')
  for (int i = 0; i < pinCount; i++) {
    char c = pgm_read_byte(test + i);
    switch (c) {
      case 'X':
      case '0':
        digitalWrite(pins[i], LOW);
        pinMode(pins[i], OUTPUT);
        break;
      case '1':
        digitalWrite(pins[i], HIGH);
        pinMode(pins[i], OUTPUT);
        break;
      case 'C':
        if (clkPinCount < MAX_CLK_PINS) {
          clkPins[clkPinCount++] = pins[i];
          pinMode(pins[i], OUTPUT);
          digitalWrite(pins[i], LOW);
        } else {
          Serial.println("Error: Too many clock pins defined.");
          return false;
        }
        break;
    }
  }

  // Phase 3: Verification of Outputs
  for (int i = 0; i < pinCount; i++) {
    char expected = pgm_read_byte(test + i);
    char actual = ' ';
    switch (expected) {
      case 'H':
        if (!digitalRead(pins[i])) {
          result = false;
          actual = 'L';
        } else {
          actual = 'H';
        }
        break;
      case 'L':
        if (digitalRead(pins[i])) {
          result = false;
          actual = 'H';
        } else {
          actual = 'L';
        }
        break;
      case 'V':
        if (!digitalRead(pins[i])) {
          result = false;
          actual = 'L';
        } else {
          actual = 'V';
        }
        break;
      case 'G':
        if (digitalRead(pins[i])) {
          result = false;
          actual = 'V';
        } else {
          actual = 'G';
        }
        break;
      default:
        actual = ' ';
        break;
    }
    Serial.print(actual);
    if (actual != ' ') {
      Serial.print(" ");
    }
  }
  Serial.println(";");

  // Reset all pins to input mode
  reset_pin_config(pinCount);

  return result;
}



//core logic
/**
 * Determines the pin count of the IC model and runs the corresponding 
 * test cases. After running the tests, it resets the pin configuration to its default state.
 * 
 * @param icModel The index of the IC model to be tested. This index corresponds to 
 * the position of the IC model in the testPatterns array.
 */
void get_test_case(byte icModel) {
  // Read PROGMEM pointers for the specified icModel
  PGM_P icType_p = reinterpret_cast<PGM_P>(pgm_read_ptr(&testPatterns[icModel - 1].icType));
  PGM_P pinCount_p = reinterpret_cast<PGM_P>(pgm_read_ptr(&testPatterns[icModel - 1].pinCount));
  PGM_P numTestCases_p = reinterpret_cast<PGM_P>(pgm_read_ptr(&testPatterns[icModel - 1].numTestCases));
  PGM_P testPatterns_p = reinterpret_cast<PGM_P>(pgm_read_ptr(&testPatterns[icModel - 1].testPatterns));

  // Print IC Type
  char icType_buffer[20];
  strcpy_P(icType_buffer, icType_p);
  Serial.print(F("Testing IC: "));
  Serial.println(icType_buffer);

  // Print each test pattern and its result
  byte numTestCases = pgm_read_byte(numTestCases_p);
  for (byte i = 0; i < numTestCases; ++i) {
    PGM_P pattern_p = reinterpret_cast<PGM_P>(pgm_read_ptr(testPatterns_p + i * sizeof(PGM_P)));
    char testPattern_buffer[30]; // Adjust size as needed
    strcpy_P(testPattern_buffer, pattern_p);

    // Execute test case and print results
    boolean passed = testCase(pattern_p, reinterpret_cast<const byte*>(pgm_read_word(pinCount_p)), pgm_read_byte(pinCount_p));
    Serial.print(F("Signal: "));
    for (int j = 0; j < pgm_read_byte(pinCount_p); j++) {
      Serial.print(testPattern_buffer[j]);
    }
    Serial.println();
    if (passed) {
      Serial.println(F("Response:                          ;"));
    } else {
      Serial.println(F("Response:          L                         ;"));
      Serial.println(F("Failed"));
    }
  }
}


//core logic
/**
 * Automatically tests IC models based on pin count.
 * 
 * @param pins The number of pins to test (14 or 16).
 */
void autoSearch(byte pins) {
  passedCount = 0; // Reset passed count
  byte size_db = sizeof(testPatterns) / sizeof(testPatterns[0]);

  for (byte i = 0; i < size_db; i++) {
    bool overallResult = true;

    // Check if pinCount matches the IC model's pin configuration
    if (pgm_read_byte(reinterpret_cast<PGM_P>(pgm_read_ptr(&testPatterns[i].pinCount))) == pins) {
      // Print IC model being tested
      PGM_P icType_p = reinterpret_cast<PGM_P>(pgm_read_ptr(&testPatterns[i].icType));
      char icType_buffer[20];
      strcpy_P(icType_buffer, icType_p);
      Serial.print(F("\nTesting IC Model: "));
      Serial.println(icType_buffer);

      // Get number of test cases for the current IC model
      byte numTestCases = pgm_read_byte(reinterpret_cast<PGM_P>(pgm_read_ptr(&testPatterns[i].numTestCases)));

      // Iterate through each test case for the current IC model
      for (int j = 0; j < numTestCases; j++) {
        PGM_P pattern_p = reinterpret_cast<PGM_P>(pgm_read_ptr(reinterpret_cast<PGM_P>(pgm_read_ptr(&testPatterns[i].testPatterns)) + j * sizeof(PGM_P)));
        boolean testResult = testCase(pattern_p, pins == 14 ? PINS_14 : (pins == 16 ? PINS_16 : PINS_8), pins);
        
        // Check if current test case passed or failed
        if (!testResult) {
          overallResult = false;
        }
        reset_pin_config(pins);  // Ensure pins are reset after each test case
      }

      // Output result for the current IC model
      if (overallResult) {
        passedModels[passedCount++] = icType_buffer;
        Serial.println(F("All tests passed.\n"));
      } else {
        Serial.println(F("Failed.\n"));
      }
    }
  }

  // Display results
  if (passedCount > 0) {
    currentModelIndex = 0; // Reset to first model
    menu = 5; // Set menu to passed models display
    updatePassedModelsDisplay();
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("No models passed"));
  }
}


//core logic
/**
 * Resets the pin configuration of the specified IC model to its default state.
 * 
 * @param pinCount The number of pins on the IC model (14 or 16).
 */
void reset_pin_config(byte pinCount) {
  const byte* pins;
  switch (pinCount) {
    case 14:
      pins = PINS_14;
      break;
    case 16:
      pins = PINS_16;
      break;
    case 8:
      pins = PINS_8;
      break;
    default:
      return; // Handle error or other pin counts as needed
  }

  for (int i = 0; i < pinCount; i++) {
    pinMode(pgm_read_byte(pins + i), INPUT);  // Reset pin mode to INPUT
    digitalWrite(pgm_read_byte(pins + i), LOW); // Reset pin state to LOW
  }
}


//core logic
/**
 * Displays the automatic user interface options on the LCD.
 */
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
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(">8-PIN IC       "));
      lcd.setCursor(0, 1);
      lcd.print(F(" 20-PIN IC      "));
      break;
    case 4:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(" 8-PIN IC       "));
      lcd.setCursor(0, 1);
      lcd.print(F(">20-PIN IC      "));
      break;
    default:
      submenuAuto = 1;
      automatic_user_interface();
      break;
  }
}

//core logic
/**
 * Displays the manual user interface options on the LCD.
 */
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
      lcd.print(F(" IC 7427        "));
      break;
    case 8:
      lcd.print(F(" IC 747266      "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7427        "));
      break;
    case 9:
      lcd.print(F(">IC 74151       "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 7421        "));
      break;
    case 10:
      lcd.print(F(" IC 74151       "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7421        "));
      break;
    case 11:
      lcd.print(F(">IC 74192       "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 7474        "));
      break;
    case 12:
      lcd.print(F(" IC 74192       "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7474        "));
      break;
    case 13:
      lcd.print(F(">IC 74190       "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 74193       "));
      break;
    case 14:
      lcd.print(F(" IC 74190       "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 74193       "));
      break;
    case 15:
      lcd.print(F(">IC 74195       "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 7410        "));
      break;
    case 16:
      lcd.print(F(" IC 74195       "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7410        "));
      break;
    case 17:
      lcd.print(F(">IC 7411        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 74125       "));
      break;
    case 18:
      lcd.print(F(" IC 7411        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 74125       "));
      break;
    case 19:
      lcd.print(F(">IC 74138       "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 7447        "));
      break;
    case 20:
      lcd.print(F(" IC 74138       "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7447        "));
      break;
    case 21:
      lcd.print(F(">IC 74173       "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 4070        "));
      break;
    case 22:
      lcd.print(F(" IC 74173       "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 4070        "));
      break;
    case 23:
      lcd.print(F(">IC 4071        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 4017        "));
      break;
    case 24:
      lcd.print(F(" IC 4071        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 4017        "));
      break;
    case 25:
      lcd.print(F(">IC 4511        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 4081        "));
      break;
    case 26:
      lcd.print(F(" IC 4511        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 4081        "));
      break;
    case 27:
      lcd.print(F(">IC 4077        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 4068        "));
      break;
    case 28:
      lcd.print(F(" IC 4077        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 4068        "));
      break;
    case 29:
      lcd.print(F(">IC 4069        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 4066        "));
      break;
    case 30:
      lcd.print(F(" IC 4069        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 4066        "));
      break;
    case 31:
      lcd.print(F(">IC 4094        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 74112       "));
      break;
    case 32:
      lcd.print(F(" IC 4094        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 74112       "));
      break;
    case 33:
      lcd.print(F(">IC 741         "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 072         "));
      break;
    case 34:
      lcd.print(F(" IC 741         "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 072         "));
      break;
    case 35:
      lcd.print(F(">IC 071         "));
      lcd.setCursor(0, 1);
      lcd.print(F("                "));
      break;
    default:
      submenu = 1;
      manual_user_interface();
      break;
  }
}

//core logic
/**
 * Displays automatic testing options on the LCD.
 */
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
    case 3:
      lcd.print(F("Please wait...  "));
      autoSearch(8);
      break;
    case 4:
      lcd.print(F("Please wait...  "));
      autoSearch(20);
      break;
  }
}

/**
 * Updates the menu display on the LCD based on the current menu state.
 */
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

/**
 * Executes the action corresponding to the current menu selection.
 */
void switch_menu() {
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

/**
 * Updates the LCD to display the passed models.
 */
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

/**
 * Reads the value from a potentiometer connected to PIN_POT.
 * The value is read 10 times, averaged, and assigned to variable 'a'.
 * This function runs every 50 milliseconds.
 */
void potreader(){
  static unsigned long t;
  if(millis() - t < 50) return;
  t = millis();

  unsigned int a;
   a = 0;
  for(byte i=0;i<10;i++) 
  {
    a+=analogRead(PIN_POT);    
  }
  a/=10;

  //a is the final value. this is between 0 and 1023
}

/**
 * Toggles the heartbeat LED (connected to PIN_LED1) every second.
 * The LED blinks to indicate the system is running.
 */
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

/**
 * Handles button debouncing for multiple buttons connected to PINS_BUTTONS array.
 * If a button press is detected and debounced, it sets the corresponding flag_button to true.
 */
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

/**
 * Checks if any button has been pressed.
 *
 * @return true if any button is pressed, false otherwise.
 */
bool get_button_ok(){
  for (byte i = 0; i < MAX_BUTTONS; i++) {
    if (flag_button[i]) {
      return true;  // Return true if any button is pressed
    }
  }
  return false;  // Return false if no button is pressed
}

/**
 * Scans buttons and performs actions based on the current menu context.
 * It handles navigation through main menu, submenus, and executing actions.
 */
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
        // switch_menu();
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
      else submenu = 35; // Wrap around to last option
      manual_user_interface();
    }
  
    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenu < 35) submenu++;
      else submenu = 1; // Wrap around to first option
      manual_user_interface();
    }
  
    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag
      get_test_case(submenu);
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
      else submenuAuto = 4; // Wrap around to last option
      automatic_user_interface();
    }
  
    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenuAuto < 4) submenuAuto++;
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

/**
 * Arduino setup function, called once at startup.
 */
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

/**
 * Arduino main loop function, continuously called after setup.
 */
void loop() {
  buttonDebounce();
  buttonScanner();
  //heartbeatLED();
}
