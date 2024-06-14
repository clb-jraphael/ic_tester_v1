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

const int PIN_LCD_CONTRAST = A10;

const int rs = A10, en = A12, d4 = 28, d5 = 29, d6 = 30, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);





// GLOBAL VARIABLES
byte menu = 1, submenu = 1, num = 1;
unsigned long lastButtonPressTime = 0;
const byte debounceDelay = 255;
unsigned long lastMillis = 0; // Variable to store last time LED was updated

bool btnDownPressed = false;
bool btnUpPressed = false;
bool btnOkPressed = false;
bool btnCancelPressed = false;

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

void checkKeyAvailable() {
  if (!btnDownPressed && !digitalRead(PIN_BTN_DOWN)) {
    btnDownPressed = true;
    buzz_for_feedback();
    // Process key press action here
  } else if (btnDownPressed && digitalRead(PIN_BTN_DOWN)) {
    btnDownPressed = false;
  }

  if (!btnUpPressed && !digitalRead(PIN_BTN_UP)) {
    btnUpPressed = true;
    buzz_for_feedback();
    // Process key press action here
  } else if (btnUpPressed && digitalRead(PIN_BTN_UP)) {
    btnUpPressed = false;
  }

  if (!btnOkPressed && !digitalRead(PIN_BTN_OK)) {
    btnOkPressed = true;
    buzz_for_feedback();
    // Process key press action here
  } else if (btnOkPressed && digitalRead(PIN_BTN_OK)) {
    btnOkPressed = false;
  }

  if (!btnCancelPressed && !digitalRead(PIN_BTN_CANCEL)) {
    btnCancelPressed = true;
    buzz_for_feedback();
    // Process key press action here
  } else if (btnCancelPressed && digitalRead(PIN_BTN_CANCEL)) {
    btnCancelPressed = false;
  }
}

void buzz_for_feedback() {
  digitalWrite(PIN_BUZZ, HIGH);
  delay(50);
  digitalWrite(PIN_BUZZ, LOW);
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
  button_scanner();
  potreader();
  checkKeyAvailable();
  heartbeatLED();
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
  }
}


void display_placeholder_text() {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (submenu) {
    case 1:
      lcd.print(F("Testing IC 7400"));
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
      lcd.print(F("Testing IC 7437"));
      testIC7437();
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

void button_scanner() {
  unsigned long currentTime = millis();
  if (currentTime - lastButtonPressTime > debounceDelay) {
    if (!digitalRead(PIN_BTN_DOWN) && num == 1){
      buzz_for_feedback();
      menu++;
      update_menu();
      lastButtonPressTime = currentTime;
      while (!digitalRead(PIN_BTN_DOWN) && num == 1);
    }
    if (!digitalRead(PIN_BTN_UP) && num == 1){
      buzz_for_feedback();
      menu--;
      update_menu();
      lastButtonPressTime = currentTime;
      while(!digitalRead(PIN_BTN_UP) && num == 1);
    }
    if (!digitalRead(PIN_BTN_OK) && num == 1){
      buzz_for_feedback();
      execute_action();
      lastButtonPressTime = currentTime;
      while (!digitalRead(PIN_BTN_OK) && num == 1);
    }
    if (!digitalRead(PIN_BTN_DOWN) && num == 2){
      buzz_for_feedback();
      if (submenu < 7) submenu++;
      manual_user_interface();
      lastButtonPressTime = currentTime;
      while (!digitalRead(PIN_BTN_DOWN) && num == 2);
    }
    if (!digitalRead(PIN_BTN_UP) && num == 2){
      buzz_for_feedback();
      if (submenu > 1) submenu--;
      manual_user_interface();
      lastButtonPressTime = currentTime;
      while (!digitalRead(PIN_BTN_UP) && num == 2);
    }
    if (!digitalRead(PIN_BTN_OK) && num == 2) {
      buzz_for_feedback();
      display_placeholder_text();
      lastButtonPressTime = currentTime;
      while (!digitalRead(PIN_BTN_OK) && num == 2);
    }
    if (!digitalRead(PIN_BTN_CANCEL)) {
      buzz_for_feedback();
      if (num == 2) {
        num = 1;
        update_menu();
        lastButtonPressTime = currentTime;
        while (!digitalRead(PIN_BTN_CANCEL));
      }
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
      lcd.print(F(">Automatic"));
      lcd.setCursor(0, 1);
      lcd.print(F(" Manual"));
      break;
    case 2:
      lcd.clear();
      lcd.print(F(" Automatic"));
      lcd.setCursor(0, 1);
      lcd.print(F(">Manual"));
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
      // TO DO: Implement automatic through a different function
      break;
    case 2:
      manual_user_interface();
      num++;
      break;
  }
}

void manual_user_interface() {
  lcd.clear();
  switch (submenu) {
    case 0:
      submenu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(F(">IC 7400        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 7402        "));
      break;
    case 2:
      lcd.clear();
      lcd.print(F(" IC 7400        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7402        "));
      break;
    case 3:
      lcd.clear();
      lcd.print(F(">IC 7404        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 7408        "));
      break;
    case 4: 
      lcd.clear();
      lcd.print(F(" IC 7404        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7408        "));
      break;
    case 5: 
      lcd.clear();
      lcd.print(F(">IC 7437        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 7486        "));
      break;
    case 6: 
      lcd.clear();
      lcd.print(F(" IC 7437        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7486        "));
      break;
    case 7: 
      lcd.clear();
      lcd.print(F(">IC 747266      "));
      lcd.setCursor(0, 1);
      break;
    case 8:
      submenu = 7;
      break;
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

void testIC7437() {
  // Implement test procedure for IC 7437
}

void testIC7486() {
  // Implement test procedure for IC 7486
}

void testIC747266() {
  // Implement test procedure for IC 747266
}

