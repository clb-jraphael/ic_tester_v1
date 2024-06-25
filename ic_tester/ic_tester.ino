/**
 * @file IC_Tester.ino
 * @brief Arduino-based IC Tester with automatic and manual testing modes.
 * 
 * This project is an IC tester using an Arduino, an LCD screen, and buttons for navigation.
 * It supports both automatic and manual IC testing and displays results on an LCD screen.
 * 
 * @version 1.0
 * @date 2024-06-25
 * 
 * 
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

const byte PIN_RGBLED_R = 6;
const byte PIN_RGBLED_G = 4;
const byte PIN_RGBLED_B = 2;
const byte PIN_LED1 = 8;

const int rs = A10, en = A12, d4 = 28, d5 = 29, d6 = 30, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//pin definitions/configurations 
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

// Defined structure to hold IC test patterns
struct IC_TestPatterns {
  const char* icType;             // IC type (e.g., "7400", "7402", etc.)
  byte pinCount;
  byte numTestCases;              // Number of test cases for this IC
  const char* testPatterns[36];   // Array to hold test patterns (adjust size as needed)
};

// GLOBAL VARIABLES
byte menu = 1, submenu = 1, submenuAuto = 1, num = 1, currentModelIndex = 0, passedCount = 0;
String passedModels[5];
unsigned long lastMillis = 0; // Variable to store last time LED was updated

bool btnDownPressed = false;
bool btnUpPressed = false;
bool btnOkPressed = false;
bool btnCancelPressed = false;

// test input data
IC_TestPatterns testPatterns[] = 
{
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
  }},

  {"7421", 14, 4, {
    "11X11HGH11X11V",
    "01X01LGL01X01V",
    "10X10LGL10X10V",
    "00X00LGL00X00V"
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

  {"7437", 14, 4, {
    "00H00HGH00H00V",
    "10H10HGH10H10V",
    "01H01HGH01H01V",
    "11L11LGL11L11V"
  }},

  {"74190", 16, 9, {
    "0LL00LLG000LHC0V",
    "0LL10LLG001LHC0V",
    "0LH00LLG001LHC0V",
    "0HL00LLG001LHC0V",
    "0HH00LLG001LHC0V",
    "0LL00HLG001LHC0V",
    "0HH01LLG001LHC0V",
    "0HL01LLG001LHC0V",
    "0LH01LLG001LHC0V"
  }},

  {"74193", 16, 9, {
    "1HL11LHG100HH00V",
    "0LH11HLG010HH01V",
    "1HH11HHG110HH01V",
    "1HLC1HHG111HH01V",
    "1LHC1HHG111HH01V",
    "1LLC1HHG111HH01V",
    "1HHC1LHG111HH01V",
    "1HLC1LHG111HH01V",
    "1LHC1LHG111HH01V",
    "1LLC1LHG111HH01V",
    "1HHC1HLG111HH01V",
    "1HLC1HLG111HH01V",
    "1LHC1HLG111HH01V",
    "1LLC1HLG111HH01V",
    "1HHC1LLG111HH01V",
    "1HLC1LLG111HH01V",
    "1LHC1LLG111HH01V",
    "1LL11LLG111HH01V",
    "1LL01LLG111HL01V",
    "1HH11HHG111HH01V",
    "1HH10HHG111LH01V",
    "1LL11LLG111HH01V",
    "1LH1CLLG111HH01V",
    "1HL1CLLG111HH01V",
    "1HH1CLLG111HH01V",
    "1LL1CHLG111HH01V",
    "1LH1CHLG111HH01V",
    "1HL1CHLG111HH01V",
    "1HH1CHLG111HH01V",
    "1LL1CLHG111HH01V",
    "1LL11LLG111HHC1V"
  }},

  {"74192", 16, 7, {
    "0LL11LLG1011110V",
    "0LL11LLG1011100V",
    "0LL11LHG1001100V",
    "0LL11LHG1011100V",
    "0LL10LHG1011100V",
    "0LH11LHG1011100V",
    "0LH10LHG1010100V"
  }},

  {"74195", 16, 7, {
    "0000000G0CHLLLLV",
    "1000000G0CHLLLLV",
    "1000000G1CHLLLLV",
    "1010000G1CHLLLLV",
    "1000000G1CHLLLLV",
    "1110000G1CHLLLHV",
    "1100000G1CHLLHLV"
  }},

  {"7410", 14, 8, {
    "00000HGH000H0V",
    "00100HGH100H1V",
    "10010HGH010H0V",
    "10110HGH110H1V",
    "01001HGH001H0V",
    "01101HGH101H1V",
    "11011HGH011H0V",
    "11111LGL111L1V"
  }},

   {"7411", 14, 8, {
    "00000LGL000L0V",
    "00001LGL001L1V",
    "01010LGL010L0V",
    "10100LGL100L0V",
    "11110LGL110L0V",
    "01011LGL011L1V",
    "10101LGL101L1V",
    "11111HGH111H1V",
  }},

  {"74112", 16, 8, {
    "C000HLLGH000C11V",
    "C110HLLGH011C11V",
    "C001LHHGL100C00V",
    "C111LHHGL100C00V",
    "C001LHHGL100C11V",
    "C011HLLGH110C11V",
    "C101LHHGL101C11V",
    "C111HLLGH111C11V"
  }},

  {"74125", 14, 8, {
    "01H01HGH10H10V",
    "00L00LGL00L00V",
    "10H10HGH01H01V",
    "11H11HGH11H11V",
    "10H11HGH01H11V",
    "11H10HGH11H01V",
    "11H10HGH01H11V",
    "10H11HGH11H01V"
  }},

  {"74138", 16, 12, {
    "111111HGHHHHHHHV",
    "000110HGHHHHHHHV",
    "111110HGHHHHHHHV",
    "000000HGHHHHHHHV",
    "000001HGHHHHHHLV",
    "100001HGHHHHHLHV",
    "010001HGHHHHLHHV",
    "110001HGHHHLHHHV",
    "001001HGHHLHHHHV",
    "101001HGHLHHHHHV",
    "011001HGLHHHHHHV",
    "111001LGHHHHHHHV"
  }},

  {"7447", 16, 10, {
    "001H100GLLLLLHLV",
    "001H001GHHLLHHHV",
    "101H100GLLHLLLHV",
    "101H001GHLLLLLHV",
    "011H100GHHLLHLLV",
    "011H001GHLLHLLLV",
    "111H100GLLLHHLLV",
    "111H001GHHLLLHHV",
    "001H110GLLLLLLLV",
    "001H011GHHLLLLLV"
  }},

  {"74173", 16, 15, {
    "00LLLLCG0000001V",
    "00LLLLCG1111111V",
    "00LHLHCG0010100V",
    "00LHLHCG1001010V",
    "00LHLHCG0101010V",
    "00LHLHCG1101010V",
    "00HLHLCG0001010V",
    "00HLHLCG1010100V",
    "00HLHLCG0110100V",
    "00HLHLCG1110100V",
    "00LHLHCG0010100V",
    "00LLHHCG0011000V",
    "00LLLLCG1111111V",
    "00HHLLCG0000110V",
    "00LLLLCG1111111V"
  }},
  
  // Add more IC test patterns here as needed
  {"4070", 14, 4, {
    "00LL00G00LL00V", // A = 0, B = 0, Y = 1
    "10HH10G10HH10V", // A = 0, B = 1, Y = 0
    "01HH01G01HH01V", // A = 1, B = 0, Y = 0
    "11LL11G11LL11V"  // A = 1, B = 1, Y = 1
  }},

  {"4071", 14, 4, {
    "00LH11G11HL00V",
    "10HH10G10HH10V",
    "01HH01G01HH01V",
    "11HL00G00LH11V"
  }},

  {"4017", 16, 12, {
    "LLHLLLLGLLLH0C1V", 
    "LHLLLLLGLLLH0C0V", 
    "LLLHLLLGLLLH0C0V", 
    "LLLLLLHGLLLH0C0V", 
    "LLLLLLLGLHLH0C0V", 
    "HLLLLLLGLLLL0C0V", 
    "LLLLHLLGLLLL0C0V", 
    "LLLLLHLGLLLL0C0V", 
    "LLLLLLLGHLLL0C0V", 
    "LLLLLLLGLLHL0C0V", 
    "LLHLLLLGLLLH0C0V", 
    "LLHLLLLGLLLH1C0V"
  }},

  {"4511", 16, 5, {
    "0001000GHHHHHHHV",
    "0011001GLLHHLLLV",
    "0111001GLHHLHHHV",
    "0011000GHHHHHLHV",
    "0010000GLLLLLLLV"
  }},

  {"4081", 14, 4, {
    "00LH11G11HL00V",
    "10LL10G10LL10V",
    "01LL01G01LL01V",
    "11HL00G00LH11V"
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
  }}
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
      else submenu = 33; // Wrap around to last option
      manual_user_interface();
    }
  
    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenu < 33) submenu++;
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
  }
}

/**
 * Displays placeholder text and tests an IC based on the current submenu selection.
 */
void display_placeholder_text() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  if (submenu >= 1 && submenu <= 33) {
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

/**
 * Executes the action corresponding to the current menu selection.
 */
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
    default:
      submenuAuto = 1;
      automatic_user_interface();
      break;
  }
}

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
      lcd.print(F(">IC 7437        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 74190       "));
      break;
    case 14:
      lcd.print(F(" IC 7437        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 74190       "));
      break;
    case 15:
      lcd.print(F(">IC 74193       "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 74195       "));
      break;
    case 16:
      lcd.print(F(" IC 74193       "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 74195       "));
      break;
    case 17:
      lcd.print(F(">IC 7410        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 7411        "));
      break;
    case 18:
      lcd.print(F(" IC 7410        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 7411        "));
      break;
    case 19:
      lcd.print(F(">IC 74125       "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 74138       "));
      break;
    case 20:
      lcd.print(F(" IC 74125       "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 74138       "));
      break;
    case 21:
      lcd.print(F(">IC 7447        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 74173       "));
      break;
    case 22:
      lcd.print(F(" IC 7447        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 74173       "));
      break;
    case 23:
      lcd.print(F(">IC 4070        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 4071        "));
      break;
    case 24:
      lcd.print(F(" IC 4070        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 4071        "));
      break;
    case 25:
      lcd.print(F(">IC 4017        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 4511        "));
      break;
    case 26:
      lcd.print(F(" IC 4017        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 4511        "));
      break;
    case 27:
      lcd.print(F(">IC 4081        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 4077        "));
      break;
    case 28:
      lcd.print(F(" IC 4081        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 4077        "));
      break;
    case 29:
      lcd.print(F(">IC 4068        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 4069        "));
      break;
    case 30:
      lcd.print(F(" IC 4068        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 4069        "));
      break;
    case 31:
      lcd.print(F(">IC 4066        "));
      lcd.setCursor(0, 1);
      lcd.print(F(" IC 4094        "));
      break;
    case 32:
      lcd.print(F(" IC 4066        "));
      lcd.setCursor(0, 1);
      lcd.print(F(">IC 4094        "));
      break;
    case 33:
      lcd.print(F(">IC 74112       "));
      lcd.setCursor(0, 1);
      lcd.print(F("                "));
      break;
    default:
      submenu = 1;
      manual_user_interface();
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
 * Configures the pins for a test pattern.
 * 
 * @param testPattern The pattern of the test.
 * @param pins The array of pin numbers.
 * @param pinCount The number of pins.
 */
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
        pinMode(pins[i], INPUT);
        break;
      default:
        break;
    }
  }
}

/**
 * Tests an IC based on its test patterns. Used for manual testing.
 * 
 * @param icPattern The test patterns for the IC.
 * @param pins The array of pin numbers.
 * @return true if all tests pass, false otherwise.
 */
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

/**
 * Detects the number of pins connected to the IC.
 * 
 * @return The number of detected pins.
 */
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

boolean testCase(const char* test, const byte* pins, int pinCount) {
  boolean result = true;
  int clkPin = -1;

  Serial.println("SignalIn : " + String(test));
  Serial.print("Response : ");

  // Setting Vcc, GND and INPUTS
  for (int i = 0; i < pinCount; i++) {
    switch (test[i]) {
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
    }
  }

  delay(5);

  // Setting Input Signals
  for (int i = 0; i < pinCount; i++) {
    switch (test[i]) {
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
        clkPin = pins[i];
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], LOW);
        break;
    }
  }

  if (clkPin != -1) {
    // Clock Trigger
    pinMode(clkPin, INPUT_PULLUP);
    delay(10);
    pinMode(clkPin, OUTPUT);
    digitalWrite(clkPin, LOW);
  }

  delay(5);

  // Reading Outputs
  for (int i = 0; i < pinCount; i++) {
    switch (test[i]) {
      case 'H':
        if (!digitalRead(pins[i])) {
          result = false;
          Serial.print('L');
        } else {
          Serial.print(' ');
        }
        break;
      case 'L':
        if (digitalRead(pins[i])) {
          result = false;
          Serial.print('H');
        } else {
          Serial.print(' ');
        }
        break;
      default:
        Serial.print(' ');
        break;
    }
  }
  Serial.println(";");

  return result;
}

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
    if (testPatterns[i].pinCount == pins) {
      Serial.println("\nTesting IC Model: " + String(testPatterns[i].icType));
      for (int j = 0; j < testPatterns[i].numTestCases; j++) {
        if (!testCase(testPatterns[i].testPatterns[j], pins == 14 ? PINS_14 : PINS_16, pins)) {
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
