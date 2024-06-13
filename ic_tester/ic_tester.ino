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

const  byte PINS_BUTTONS[6] = {
  PIN_BTN_UP,
  PIN_BTN_DOWN,
  PIN_BTN_LEFT,
  PIN_BTN_RIGHT,
  PIN_BTN_OK,
  PIN_BTN_CANCEL
};

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

const byte PIN_IC_PIN1 = A15; // physical pin 1
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

const int rs = A10, en = A12, d4 = 28, d5 = 29, d6 = 30, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);





// GLOBAL VARIABLES
int menu = 1;
unsigned long lastButtonPressTime = 0;
unsigned long debounceDelay = 100;


// FUNCTIONS

void init_ic_pins()
{
  for(byte i=0;i<20;i++) pinMode(PINS_IC[i], INPUT);
}


void potreader()
{
  static unsigned long t;
  if (millis() - t < 50) return;
  t = millis();

  unsigned int a;
  a = 0;
  for (byte i = 0; i < 10; i++) {
    a += analogRead(PIN_POT);
  }
  a /= 10;
  // a is the final value. this is between 0 and 1023
  //analogWrite(PIN_RGBLED_R, map(a, 0, 1023, 0, 255));
  analogWrite(PIN_RGBLED_B, map(a, 0, 1023, 0, 255));

}

void button_scanner() {
  static unsigned long lastDebounceTime = 0;
  static boolean lastButtonState[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH}; // Initialize the last button states
  
  // Loop through each button pin
  for (int i = 0; i < 6; i++) {
    byte buttonPin = PINS_BUTTONS[i];
    int buttonState = digitalRead(buttonPin);
    
    // Check if current button state is different from the last known state
    if (buttonState != lastButtonState[i]) {
      // Reset the debounce timer
      lastDebounceTime = millis();
    }
    
    // Check if debounce delay has elapsed
    if ((millis() - lastDebounceTime) > debounceDelay) {
      // If button state has remained stable for debounceDelay, consider it a valid button press
      if (buttonState != lastButtonState[i]) {
        lastButtonState[i] = buttonState; // Update last button state
        
        // Perform action based on button pin
        switch (buttonPin) {
          case PIN_BTN_UP:
            // Handle button up action
            menu++;
            if (menu > 7) {
              menu = 1; // Wrap around if menu goes out of bounds
            }
            update_menu();
            break;
          case PIN_BTN_DOWN:
            // Handle button down action
            menu--;
            if (menu < 1) {
              menu = 7; // Wrap around if menu goes out of bounds
            }
            update_menu();
            break;
          case PIN_BTN_LEFT:
            // TODO: Implementation
            break;
          case PIN_BTN_RIGHT:
            // TODO: Implementation
            break;
          case PIN_BTN_OK:
            execute_action();
            update_menu();
            break;
          case PIN_BTN_CANCEL:
            // TODO: Implementation
            break;
          default:
            // Handle other button actions if needed
            break;
        }
      }
    }
  }
}

void setup() {
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
  potreader();
  unsigned long currentMillis = millis();

  // Handle button presses with non-blocking delay using millis()
  if (!digitalRead(PIN_BTN_DOWN) && (currentMillis - lastButtonPressTime >= debounceDelay)) {
    lastButtonPressTime = currentMillis;
    menu++;
    if (menu > 2) {
      menu = 2;
    }
    update_menu();
  }
  if (!digitalRead(PIN_BTN_UP) && (currentMillis - lastButtonPressTime >= debounceDelay)) {
    lastButtonPressTime = currentMillis;
    menu--;
    if (menu < 1) {
      menu = 1;
    }
    update_menu();
  }
  if (!digitalRead(PIN_BTN_OK) && (currentMillis - lastButtonPressTime >= debounceDelay)) {
    lastButtonPressTime = currentMillis;
    execute_action();
    if (menu == 1) {
      // Automatic testing function
    } else {
      display_manual_submenu();
    }
  }
}

void update_menu() {
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Automatic");
      lcd.setCursor(0, 1);
      lcd.print(" Manual");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Automatic");
      lcd.setCursor(0, 1);
      lcd.print(">Manual");
      break;
    case 3:
      menu = 2;
      break;
  }
}

void execute_action() {
  switch (menu) {
    case 1:
      // automaticUserInterface();
      // TO DO: Implement 
      break;
    case 2:
      display_manual_submenu();
      break;
  }
}

void display_manual_submenu(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">IC 7400        ");
  lcd.setCursor(0, 1);
  lcd.print(" IC 7402        ");
  manual_user_interface();
}

void manual_user_interface() {
  lcd.clear();
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">IC 7400        ");
      lcd.setCursor(0, 1);
      lcd.print(" IC 7402        ");
      break;
    case 2:
      lcd.clear();
      lcd.print(" IC 7400");
      lcd.setCursor(0, 1);
      lcd.print(">IC 7402");
      break;
    case 3:
      lcd.clear();
      lcd.print(">IC 7404");
      lcd.setCursor(0, 1);
      lcd.print(" IC 7408");
      break;
    case 4: 
      lcd.clear();
      lcd.print(" IC 7404");
      lcd.setCursor(0, 1);
      lcd.print(">IC 7408");
      break;
    case 5: 
      lcd.clear();
      lcd.print(">IC 7437");
      lcd.setCursor(0, 1);
      lcd.print(" IC 7486");
      break;
    case 6: 
      lcd.clear();
      lcd.print(" IC 7437");
      lcd.setCursor(0, 1);
      lcd.print(">IC 7486");
      break;
    case 7: 
      lcd.clear();
      lcd.print(">IC 747266");
      lcd.setCursor(0, 1);
      break;
    case 8:
      menu = 7;
      break;
  }
}

void execut_action_test_IC() {
  switch (menu) {
    case 1:
      testIC7400();
      break;
    case 2:
      testIC7402();
      break;
    case 3:
      testIC7404();
      break;
    case 4:
      testIC7408();
      break;
    case 5:
      testIC747266();
  }
}

void testIC7400() {
  // Implement test procedure for IC 7400
  // Example: Check pins, apply signals, read outputs, etc.
}

void testIC7402() {
  // Implement test procedure for IC 7402
}

void testIC7404() {
  // Implement test procedure for IC 7404
}

void testIC7408() {
  // Implement test procedure for IC 7408
}

void testIC747266() {

}