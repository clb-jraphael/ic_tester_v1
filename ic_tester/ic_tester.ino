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
const float HIGH_THRESHOLD = 2.4;
const float LOW_THRESHOLD = 0.8;

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
  PIN_IC_PIN1, PIN_IC_PIN2, PIN_IC_PIN3, PIN_IC_PIN4, PIN_IC_PIN5, PIN_IC_PIN6, PIN_IC_PIN7, PIN_IC_PIN8, PIN_IC_PIN9, PIN_IC_PIN10, 
  PIN_IC_PIN11, PIN_IC_PIN12, PIN_IC_PIN13, PIN_IC_PIN14, PIN_IC_PIN15, PIN_IC_PIN16, PIN_IC_PIN17, PIN_IC_PIN18, PIN_IC_PIN19, PIN_IC_PIN20
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

const byte PINS_20[20]{PIN_IC_PIN1, PIN_IC_PIN2, PIN_IC_PIN3, PIN_IC_PIN4, PIN_IC_PIN5, PIN_IC_PIN6, PIN_IC_PIN7, PIN_IC_PIN8, PIN_IC_PIN9, PIN_IC_PIN10, PIN_IC_PIN11, PIN_IC_PIN12, PIN_IC_PIN13, PIN_IC_PIN14, PIN_IC_PIN15, PIN_IC_PIN16, PIN_IC_PIN17, PIN_IC_PIN18, PIN_IC_PIN19, PIN_IC_PIN20};

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

byte menu = 1, submenu = 1, submenuAuto = 1, submenuProbe = 1, submenuPulse = 1, submenuSWave = 1, submenuPx = 1, submenuDCycle = 1, num = 1, currentModelIndex = 0, passedCount = 0;
String passedModels[5];
unsigned long lastMillis = 0; // Variable to store last time LED was updated

bool btnDownPressed = false;
bool btnUpPressed = false;
bool btnOkPressed = false;
bool btnCancelPressed = false;

// Test pattern strings stored in PROGMEM
//1
const char str_7400_1[] PROGMEM = "00H00HGH00H00V";
const char str_7400_2[] PROGMEM = "01H01HGH01H01V";
const char str_7400_3[] PROGMEM = "10H10HGH10H10V";
const char str_7400_4[] PROGMEM = "11L11LGL11L11V";
const char* const testPatterns_7400[] PROGMEM = {str_7400_1, str_7400_2, str_7400_3, str_7400_4};
//2
const char str_7402_1[] PROGMEM = "H00H00G00H00HV";
const char str_7402_2[] PROGMEM = "L10L10G10L10LV";
const char str_7402_3[] PROGMEM = "L01L01G01L01LV";
const char str_7402_4[] PROGMEM = "L11L11G11L11LV";
const char* const testPatterns_7402[] PROGMEM = {str_7402_1, str_7402_2, str_7402_3, str_7402_4};
//3
const char str_7404_1[] PROGMEM = "0H0H0HGH0H0H0V";
const char str_7404_2[] PROGMEM = "1L1L1LGL1L1L1V";
const char* const testPatterns_7404[] PROGMEM = {str_7404_1, str_7404_2};
//4
const char str_7408_1[] PROGMEM = "00L00LGL00L00V";
const char str_7408_2[] PROGMEM = "01L01LGL01L01V";
const char str_7408_3[] PROGMEM = "10L10LGL10L10V";
const char str_7408_4[] PROGMEM = "11H11HGH11H11V";
const char* const testPatterns_7408[] PROGMEM = {str_7408_1, str_7408_2, str_7408_3, str_7408_4};
//5
const char str_7432_1[] PROGMEM =  "00L00LGL00L00V";
const char str_7432_2[] PROGMEM =  "01H01HGH01H01V"; 
const char str_7432_3[] PROGMEM =  "10H10HGH10H10V"; 
const char str_7432_4[] PROGMEM =  "11H11HGH11H11V";  
const char* const testPatterns_7432[] PROGMEM = {str_7432_1, str_7432_2, str_7432_3, str_7432_4};
//6
const char str_7486_1[] PROGMEM =  "00L00LGL00L00V"; 
const char str_7486_2[] PROGMEM =  "01H01HGH01H01V"; 
const char str_7486_3[] PROGMEM =  "10H10HGH10H10V"; 
const char str_7486_4[] PROGMEM =  "11L11LGL11L11V";   
const char* const testPatterns_7486[] PROGMEM = {str_7486_1, str_7486_2, str_7486_3, str_7486_4};
//7
const char str_747266_1[] PROGMEM = "00HH00G00HH00V";  
const char str_747266_2[] PROGMEM = "10LL10G10LL10V"; 
const char str_747266_3[] PROGMEM = "01LL01G01LL01V"; 
const char str_747266_4[] PROGMEM = "11HH11G11HH11V";    
const char* const testPatterns_747266[] PROGMEM = {str_747266_1, str_747266_2, str_747266_3, str_747266_4};
//8
const char str_7427_1[] PROGMEM = "00000HGH000H0V";  
const char str_7427_2[] PROGMEM = "00100LGL100L1V"; 
const char str_7427_3[] PROGMEM = "10010LGL010L0V"; 
const char str_7427_4[] PROGMEM = "10110LGL110L1V"; 
const char str_7427_5[] PROGMEM = "01001LGL001L0V"; 
const char str_7427_6[] PROGMEM = "10101LGL101L1V";
const char str_7427_7[] PROGMEM = "11011LGL011L0V";
const char str_7427_8[] PROGMEM = "11111LGL111L1V"; 
const char* const testPatterns_7427[] PROGMEM = {str_7427_1, str_7427_2, str_7427_3, str_7427_4, str_7427_5, str_7427_6, str_7427_7, str_7427_8};
//9
const char str_74151_1[] PROGMEM =  "0101LH1G1010101V"; 
const char str_74151_2[] PROGMEM =  "1010LH1G0101010V";
const char str_74151_3[] PROGMEM =  "1110LH0G0001111V";
const char str_74151_4[] PROGMEM =  "0001HL0G0000000V";
const char str_74151_5[] PROGMEM =  "1101LH0G0011111V";
const char str_74151_6[] PROGMEM =  "0010HL0G0010000V";
const char str_74151_7[] PROGMEM =  "1011LH0G0101111V";
const char str_74151_8[] PROGMEM =  "0100HL0G0100000V";
const char str_74151_9[] PROGMEM =  "0111LH0G0111111V";
const char str_74151_10[] PROGMEM = "1000HL0G0110000V";  
const char str_74151_11[] PROGMEM = "1111LH0G1001110V"; 
const char str_74151_12[] PROGMEM = "0000HL0G1000001V"; 
const char str_74151_13[] PROGMEM = "1111LH0G1011101V"; 
const char str_74151_14[] PROGMEM = "0000HL0G1010010V"; 
const char str_74151_15[] PROGMEM = "1111LH0G1101011V";
const char str_74151_16[] PROGMEM = "0000HL0G1100100V";
const char str_74151_17[] PROGMEM = "1111LH0G1110111V"; 
const char str_74151_18[] PROGMEM = "0000HL0G1111000V";
const char* const testPatterns_74151[] PROGMEM = {str_74151_1, str_74151_2, str_74151_3, str_74151_4, str_74151_5, str_74151_6, str_74151_7, str_74151_8, str_74151_9, str_74151_10, str_74151_11, str_74151_12, str_74151_13, str_74151_14, str_74151_15, str_74151_16, str_74151_17, str_74151_18};
//10
const char str_7421_1[] PROGMEM = "11X11HGH11X11V";  
const char str_7421_2[] PROGMEM = "01X01LGL01X01V"; 
const char str_7421_3[] PROGMEM = "10X10LGL10X10V"; 
const char str_7421_4[] PROGMEM = "00X00LGL00X00V";    
const char* const testPatterns_7421[] PROGMEM = {
  str_7421_1, str_7421_2, str_7421_3, str_7421_4
};

//11
const char str_74192_1[] PROGMEM = "0LL11LLG1011110V"; // Initial reset
const char str_74192_2[] PROGMEM = "0LL11LLG1011100V"; // Load value
const char str_74192_3[] PROGMEM = "0LL11LHG1001100V"; // Count up (clock pulse)
const char str_74192_4[] PROGMEM = "0LL11LHG1011100V"; // Count up (clock pulse)
const char str_74192_5[] PROGMEM = "0LL10LHG1011100V"; // Count down (clock pulse)
const char str_74192_6[] PROGMEM = "0LH11LHG1011100V"; // Count down (clock pulse)
const char str_74192_7[] PROGMEM = "0LH10LHG1010100V"; // Count down (clock pulse)
const char* const testPatterns_74192[] PROGMEM = {
  str_74192_1, str_74192_2, str_74192_3, str_74192_4, str_74192_5, str_74192_6, str_74192_7
};

//12
const char str_7474_1[] PROGMEM = "01C1LHGHL1000V";
const char str_7474_2[] PROGMEM = "10C0HLGHL1000V";
const char str_7474_3[] PROGMEM = "10C1LHGHL1000V";
const char str_7474_4[] PROGMEM = "11C1HLGHL1000V";
const char str_7474_5[] PROGMEM = "0001LHGHL1C10V";
const char str_7474_6[] PROGMEM = "0001LHGLH0C01V";
const char str_7474_7[] PROGMEM = "0001LHGHL1C01V";
const char str_7474_8[] PROGMEM = "0001LHGLH1C11V";
const char* const testPatterns_7474[] PROGMEM = {str_7474_1, str_7474_2, str_7474_3, str_7474_4, str_7474_5, str_7474_6, str_7474_7, str_7474_8};
//13
const char str_74190_1[] PROGMEM = "0LL00LLG000LHC0V";
const char str_74190_2[] PROGMEM = "0LL10LLG001LHC0V";
const char str_74190_3[] PROGMEM = "0LH00LLG001LHC0V";
const char str_74190_4[] PROGMEM = "0HL00LLG001LHC0V";
const char str_74190_5[] PROGMEM = "0HH00LLG001LHC0V";
const char str_74190_6[] PROGMEM = "0LL00HLG001LHC0V";
const char str_74190_7[] PROGMEM = "0HH01LLG001LHC0V";
const char str_74190_8[] PROGMEM = "0HL01LLG001LHC0V";
const char str_74190_9[] PROGMEM = "0LH01LLG001LHC0V";
const char* const testPatterns_74190[] PROGMEM = {str_74190_1, str_74190_2, str_74190_3, str_74190_4, str_74190_5, str_74190_6, str_74190_7, str_74190_8, str_74190_9};
//14
const char str_74193_1[] PROGMEM =  "1HL11LHG100HH00V";
const char str_74193_2[] PROGMEM =  "0LH11HLG010HH01V";
const char str_74193_3[] PROGMEM =  "1HH11HHG110HH01V";
const char str_74193_4[] PROGMEM =  "1HLC1HHG111HH01V";
const char str_74193_5[] PROGMEM =  "1LHC1HHG111HH01V";
const char str_74193_6[] PROGMEM =  "1LLC1HHG111HH01V";
const char str_74193_7[] PROGMEM =  "1HHC1LHG111HH01V";
const char str_74193_8[] PROGMEM =  "1HLC1LHG111HH01V";
const char str_74193_9[] PROGMEM =  "1LHC1LHG111HH01V";
const char str_74193_10[] PROGMEM = "1LLC1LHG111HH01V";
const char str_74193_11[] PROGMEM = "1HHC1HLG111HH01V";
const char str_74193_12[] PROGMEM = "1HLC1HLG111HH01V";
const char str_74193_13[] PROGMEM = "1LHC1HLG111HH01V";
const char str_74193_14[] PROGMEM = "1LLC1HLG111HH01V";
const char str_74193_15[] PROGMEM = "1HHC1LLG111HH01V";
const char str_74193_16[] PROGMEM = "1HLC1LLG111HH01V";
const char str_74193_17[] PROGMEM = "1LHC1LLG111HH01V";
const char str_74193_18[] PROGMEM = "1LL11LLG111HH01V";
const char str_74193_19[] PROGMEM = "1LL01LLG111HL01V";
const char str_74193_20[] PROGMEM = "1HH11HHG111HH01V";
const char str_74193_21[] PROGMEM = "1HH10HHG111LH01V";
const char str_74193_22[] PROGMEM = "1LL11LLG111HH01V";
const char str_74193_23[] PROGMEM = "1LH1CLLG111HH01V";
const char str_74193_24[] PROGMEM = "1HL1CLLG111HH01V";
const char str_74193_25[] PROGMEM = "1HH1CLLG111HH01V";
const char str_74193_26[] PROGMEM = "1LL1CHLG111HH01V";
const char str_74193_27[] PROGMEM = "1LH1CHLG111HH01V";
const char str_74193_28[] PROGMEM = "1HL1CHLG111HH01V";
const char str_74193_29[] PROGMEM = "1HH1CHLG111HH01V";
const char str_74193_30[] PROGMEM = "1LL1CLHG111HH01V";
const char str_74193_31[] PROGMEM = "1LL11LLG111HHC1V";
const char* const testPatterns_74193[] PROGMEM = {
  str_74193_1, str_74193_2, str_74193_3, str_74193_4, str_74193_5, str_74193_6, str_74193_7, str_74193_8, str_74193_9,
  str_74193_10, str_74193_11, str_74193_12, str_74193_13, str_74193_14, str_74193_15, str_74193_16, str_74193_17, str_74193_18,
  str_74193_19, str_74193_20, str_74193_21, str_74193_22, str_74193_23, str_74193_24, str_74193_25, str_74193_26, str_74193_27,
  str_74193_28, str_74193_29, str_74193_30, str_74193_31
};
//15
const char str_74195_1[] PROGMEM = "0000000G0CHLLLLV";
const char str_74195_2[] PROGMEM = "1000000G0CHLLLLV";
const char str_74195_3[] PROGMEM = "1000000G1CHLLLLV";
const char str_74195_4[] PROGMEM = "1010000G1CHLLLLV";
const char str_74195_5[] PROGMEM = "1000000G1CHLLLLV";
const char str_74195_6[] PROGMEM = "1110000G1CHLLLHV";
const char str_74195_7[] PROGMEM = "1100000G1CHLLHLV";
const char* const testPatterns_74195[] PROGMEM = {
  str_74195_1, str_74195_2, str_74195_3, str_74195_4, str_74195_5, str_74195_6, str_74195_7
};
//16
const char str_7410_1[] PROGMEM = "00000HGH000H0V";
const char str_7410_2[] PROGMEM = "00100HGH100H1V";
const char str_7410_3[] PROGMEM = "10010HGH010H0V";
const char str_7410_4[] PROGMEM = "10110HGH110H1V";
const char str_7410_5[] PROGMEM = "01001HGH001H0V";
const char str_7410_6[] PROGMEM = "01101HGH101H1V";
const char str_7410_7[] PROGMEM = "11011HGH011H0V";
const char str_7410_8[] PROGMEM = "11111LGL111L1V";
const char* const testPatterns_7410[] PROGMEM = {str_7410_1, str_7410_2, str_7410_3, str_7410_4, str_7410_5, str_7410_6, str_7410_7, str_7410_8};
//17
const char str_7411_1[] PROGMEM = "00000LGL000L0V";
const char str_7411_2[] PROGMEM = "00001LGL001L1V";
const char str_7411_3[] PROGMEM = "01010LGL010L0V";
const char str_7411_4[] PROGMEM = "10100LGL100L0V";
const char str_7411_5[] PROGMEM = "11110LGL110L0V";
const char str_7411_6[] PROGMEM = "01011LGL011L1V";
const char str_7411_7[] PROGMEM = "10101LGL101L1V";
const char str_7411_8[] PROGMEM = "11111HGH111H1V";
const char* const testPatterns_7411[] PROGMEM = {str_7411_1, str_7411_2, str_7411_3, str_7411_4, str_7411_5, str_7411_6, str_7411_7, str_7411_8};
//18
const char str_74125_1[] PROGMEM = "01H01HGH10H10V";
const char str_74125_2[] PROGMEM = "00L00LGL00L00V";
const char str_74125_3[] PROGMEM = "10H10HGH01H01V";
const char str_74125_4[] PROGMEM = "11H11HGH11H11V";
const char str_74125_5[] PROGMEM = "10H11HGH01H11V";
const char str_74125_6[] PROGMEM = "11H10HGH11H01V";
const char str_74125_7[] PROGMEM = "11H10HGH01H11V";
const char str_74125_8[] PROGMEM = "10H11HGH11H01V";
const char* const testPatterns_74125[] PROGMEM = {str_74125_1, str_74125_2, str_74125_3, str_74125_4, str_74125_5, str_74125_6, str_74125_7, str_74125_8};
//19
const char str_74138_1[] PROGMEM =  "111111HGHHHHHHHV";
const char str_74138_2[] PROGMEM =  "000110HGHHHHHHHV";
const char str_74138_3[] PROGMEM =  "111110HGHHHHHHHV";
const char str_74138_4[] PROGMEM =  "000000HGHHHHHHHV";
const char str_74138_5[] PROGMEM =  "000001HGHHHHHHLV";
const char str_74138_6[] PROGMEM =  "100001HGHHHHHLHV";
const char str_74138_7[] PROGMEM =  "010001HGHHHHLHHV";
const char str_74138_8[] PROGMEM =  "110001HGHHHLHHHV";
const char str_74138_9[] PROGMEM =  "001001HGHHLHHHHV";
const char str_74138_10[] PROGMEM = "101001HGHLHHHHHV";
const char str_74138_11[] PROGMEM = "011001HGLHHHHHHV";
const char str_74138_12[] PROGMEM = "111001LGHHHHHHHV";
const char* const testPatterns_74138[] PROGMEM = {str_74138_1, str_74138_2, str_74138_3, str_74138_4, str_74138_5, str_74138_6, str_74138_7, str_74138_8, str_74138_9, str_74138_10 , str_74138_11, str_74138_12};
//20
const char str_7447_1[] PROGMEM =   "001H100GLLLLLHLV";
const char str_7447_2[] PROGMEM =   "001H001GHHLLHHHV";
const char str_7447_3[] PROGMEM =   "101H100GLLHLLLHV";
const char str_7447_4[] PROGMEM =   "101H001GHLLLLLHV";
const char str_7447_5[] PROGMEM =   "011H100GHHLLHLLV";
const char str_7447_6[] PROGMEM =   "011H001GHLLHLLLV";
const char str_7447_7[] PROGMEM =   "111H100GLLLHHLLV";
const char str_7447_8[] PROGMEM =   "111H001GHHLLLHHV";
const char str_7447_9[] PROGMEM =   "001H110GLLLLLLLV";
const char str_7447_10[] PROGMEM =  "001H011GHHLLLLLV";
const char* const testPatterns_7447[] PROGMEM = {str_7447_1, str_7447_2, str_7447_3, str_7447_4, str_7447_5, str_7447_6, str_7447_7, str_7447_8, str_7447_9, str_7447_10};
//21
const char str_74173_1[] PROGMEM =  "00LLLLCG0000001V";
const char str_74173_2[] PROGMEM =  "00LLLLCG1111111V";
const char str_74173_3[] PROGMEM =  "00LHLHCG0010100V";
const char str_74173_4[] PROGMEM =  "00LHLHCG1001010V";
const char str_74173_5[] PROGMEM =  "00LHLHCG0101010V";
const char str_74173_6[] PROGMEM =  "00LHLHCG1101010V";
const char str_74173_7[] PROGMEM =  "00HLHLCG0001010V";
const char str_74173_8[] PROGMEM =  "00HLHLCG1010100V";
const char str_74173_9[] PROGMEM =  "00HLHLCG0110100V";
const char str_74173_10[] PROGMEM = "00HLHLCG1110100V";
const char str_74173_11[] PROGMEM = "00LHLHCG0010100V";
const char str_74173_12[] PROGMEM = "00LLHHCG0011000V";
const char str_74173_13[] PROGMEM = "00LLLLCG1111111V";
const char str_74173_14[] PROGMEM = "00HHLLCG0000110V";
const char str_74173_15[] PROGMEM = "00LLLLCG1111111V";
const char* const testPatterns_74173[] PROGMEM = {
  str_74173_1, str_74173_2, str_74173_3, str_74173_4, str_74173_5, 
  str_74173_6, str_74173_7, str_74173_8, str_74173_9, str_74173_10,
  str_74173_11, str_74173_12, str_74173_13, str_74173_14, str_74173_15
};
//22
const char str_4070_1[] PROGMEM = "00LL00G00LL00V";
const char str_4070_2[] PROGMEM = "10HH10G10HH10V";
const char str_4070_3[] PROGMEM = "01HH01G01HH01V";
const char str_4070_4[] PROGMEM = "11LL11G11LL11V";
const char* const testPatterns_4070[] PROGMEM = {str_4070_1, str_4070_2, str_4070_3, str_4070_4};
//23
const char str_4071_1[] PROGMEM = "00LH11G11HL00V";
const char str_4071_2[] PROGMEM = "10HH10G10HH10V";
const char str_4071_3[] PROGMEM = "01HH01G01HH01V";
const char str_4071_4[] PROGMEM = "11HL00G00LH11V";
const char* const testPatterns_4071[] PROGMEM = {str_4071_1, str_4071_2, str_4071_3, str_4071_4};
//24
const char str_4017_1[] PROGMEM = "LLHLLLLGLLLH0C1V";
const char str_4017_2[] PROGMEM = "LHLLLLLGLLLH0C0V";
const char str_4017_3[] PROGMEM = "LLLHLLLGLLLH0C0V";
const char str_4017_4[] PROGMEM = "LLLLLLHGLLLH0C0V";
const char str_4017_5[] PROGMEM = "LLLLLLLGLHLH0C0V";
const char str_4017_6[] PROGMEM = "HLLLLLLGLLLL0C0V";
const char str_4017_7[] PROGMEM = "LLLLHLLGLLLL0C0V";
const char str_4017_8[] PROGMEM = "LLLLLHLGLLLL0C0V";
const char str_4017_9[] PROGMEM = "LLLLLLLGHLLL0C0V";
const char str_4017_10[] PROGMEM = "LLLLLLLGLLHL0C0V";
const char str_4017_11[] PROGMEM = "LLHLLLLGLLLH0C0V";
const char str_4017_12[] PROGMEM = "LLHLLLLGLLLH1C0V";
const char* const testPatterns_4017[] PROGMEM = {
  str_4017_1, str_4017_2, str_4017_3, str_4017_4,
  str_4017_5, str_4017_6, str_4017_7, str_4017_8,
  str_4017_9, str_4017_10, str_4017_11, str_4017_12
};
//25
const char str_4511_1[] PROGMEM = "0001000GHHHHHHHV";
const char str_4511_2[] PROGMEM = "0011001GLLHHLLLV";
const char str_4511_3[] PROGMEM = "0111001GLHHLHHHV";
const char str_4511_4[] PROGMEM = "0011000GHHHHHLHV";
const char str_4511_5[] PROGMEM = "0010000GLLLLLLLV";
const char* const testPatterns_4511[] PROGMEM = {
  str_4511_1, str_4511_2, str_4511_3, str_4511_4,
  str_4511_5, 
};
//26
const char str_4081_1[] PROGMEM = "00LH11G11HL00V";
const char str_4081_2[] PROGMEM = "10LL10G10LL10V";
const char str_4081_3[] PROGMEM = "01LL01G01LL01V";
const char str_4081_4[] PROGMEM = "11HL00G00LH11V";
const char* const testPatterns_4081[] PROGMEM = {
  str_4081_1, str_4081_2, str_4081_3, str_4081_4
};
//27
const char str_4077_1[] PROGMEM = "00HH00G00HH00V";
const char str_4077_2[] PROGMEM = "10LL10G10LL10V";
const char str_4077_3[] PROGMEM = "01LL01G01LL01V";
const char str_4077_4[] PROGMEM = "11HH11G11HH11V";
const char* const testPatterns_4077[] PROGMEM = {
  str_4077_1, str_4077_2, str_4077_3, str_4077_4
};
//28
const char str_4068_1[] PROGMEM = "011110G01111LV";
const char str_4068_2[] PROGMEM = "010100G01010HV";
const char str_4068_3[] PROGMEM = "001010G00101HV";
const char str_4068_4[] PROGMEM = "000110G00011HV";
const char str_4068_5[] PROGMEM = "011000G01100HV";
const char str_4068_6[] PROGMEM = "000000G01111HV";
const char str_4068_7[] PROGMEM = "000000G00000HV";
const char str_4068_8[] PROGMEM = "011110G01110HV";
const char str_4068_9[] PROGMEM = "011010G01111HV";
const char str_4068_10[] PROGMEM ="001110G00111HV";
const char* const testPatterns_4068[] PROGMEM = {
  str_4068_1, str_4068_2, str_4068_3, str_4068_4,
  str_4068_5, str_4068_6, str_4068_7, str_4068_8,
  str_4068_9, str_4068_10
};
//29
const char str_4069_1[] PROGMEM = "0H0H0HGH0H0H0V";
const char str_4069_2[] PROGMEM = "1L1L1LGL1L1L1V";
const char* const testPatterns_4069[] PROGMEM = {
  str_4069_1, str_4069_2
};
//30
const char str_4066_1[] PROGMEM = "0HH000G0HH000V";
const char str_4066_2[] PROGMEM = "1HH100G1HH100V";
const char str_4066_3[] PROGMEM = "0LL011G0LL011V";
const char str_4066_4[] PROGMEM = "1HH111G1HH111V";
const char* const testPatterns_4066[] PROGMEM = {
  str_4066_1, str_4066_2, str_4066_3, str_4066_4
};
//31
const char str_4094_1[] PROGMEM = "1X0XXXXGXXXXXX1V";
const char str_4094_2[] PROGMEM = "11CHXXXGXXXXXX1V";
const char str_4094_3[] PROGMEM = "10CLHXXGXXXXXX1V";
const char str_4094_4[] PROGMEM = "11CHLHXGXXXXXX1V";
const char str_4094_5[] PROGMEM = "10CLHLHGXXXXXX1V";
const char str_4094_6[] PROGMEM = "11CHLHLGXXXXXH1V";
const char str_4094_7[] PROGMEM = "10CLHLHGXXXXHL1V";
const char str_4094_8[] PROGMEM = "11CHLHLGXXXHLH1V";
const char str_4094_9[] PROGMEM = "10CLHLHGXXHLHL1V";
const char str_4094_10[] PROGMEM ="11CHLHLGLXLHLH1V";
const char str_4094_11[] PROGMEM ="10CLHLHGHXHLHL1V";
const char str_4094_12[] PROGMEM ="01CLHLHGLXHLHL1V";
const char str_4094_13[] PROGMEM ="110HLHLGLXLHLH1V";
const char str_4094_14[] PROGMEM ="100HHHHGLXHHHH0V";
const char* const testPatterns_4094[] PROGMEM = {
  str_4094_1, str_4094_2, str_4094_3, str_4094_4,
  str_4094_5, str_4094_6, str_4094_7, str_4094_8,
  str_4094_9, str_4094_10, str_4094_11, str_4094_12,
  str_4094_13, str_4094_14
};
//32
const char str_74112_1[] PROGMEM =  "C000HLLGH000C11V";
const char str_74112_2[] PROGMEM =  "C110HLLGH011C11V";
const char str_74112_3[] PROGMEM =  "C001LHHGL100C00V";
const char str_74112_4[] PROGMEM =  "C111LHHGL100C00V";
const char str_74112_5[] PROGMEM =  "C001LHHGL100C11V";
const char str_74112_6[] PROGMEM =  "C011HLLGH110C11V";
const char str_74112_7[] PROGMEM =  "C101LHHGL101C11V";
const char str_74112_8[] PROGMEM =  "C111HLLGH111C11V";
const char* const testPatterns_74112[] PROGMEM = {
  str_74112_1, str_74112_2, str_74112_3, str_74112_4,
  str_74112_5, str_74112_6, str_74112_7, str_74112_8
};
//33
const char str_741_1[] PROGMEM = "~10G~LV~";
const char str_741_2[] PROGMEM = "~01G~HV~";
const char* const testPatterns_741[] PROGMEM = {
  str_741_1, str_741_2
};
//34
const char str_072_1[] PROGMEM = "L10G10HV";
const char str_072_2[] PROGMEM = "H01G01LV";
const char* const testPatterns_072[] PROGMEM = {
  str_072_1, str_072_2
};

//35 - 20 pin
const char str_74373_1[] PROGMEM ="0H11HH11HG1H11HH11HV";
const char str_74373_2[] PROGMEM ="0L00LL00LG1L00LL00LV";
const char* const testPatterns_74373[] PROGMEM = {
  str_74373_1, str_74373_2
};

//36
const char str_74155_1[] PROGMEM = "X1XHHHHGHHHHX1XV";
const char str_74155_2[] PROGMEM = "100HHHLGLHHH000V";
const char str_74155_3[] PROGMEM = "100HHLHGHLHH100V";
const char str_74155_4[] PROGMEM = "101HLHHGHHLH000V";
const char str_74155_5[] PROGMEM = "101LHHHGHHHL100V";
const char str_74155_6[] PROGMEM = "0XXHHHHGHHHHXX1V";
const char* const testPatterns_74155[] PROGMEM = {
  str_74155_1, str_74155_2, str_74155_3,
  str_74155_4, str_74155_5, str_74155_6
};

//37
const char str_7483_1[] PROGMEM = "1L00VH01H01G0HL1"; //active high
const char str_7483_2[] PROGMEM = "0H11VL10L10G1LH0"; //active low
const char* const testPatterns_7483[] PROGMEM = {
  str_7483_1, str_7483_2
};

//38 - TIMING ISSUE (07.16.24) - JOAQUIN
//cp1, mr1, mr2, nc, v, ms1, ms2
//q2, q1, g, q3, q0, nc, cp0
const char str_7490_1[] PROGMEM = "F11XV0XLLGLLXF"; //initial reset state
const char str_7490_2[] PROGMEM = "F11XVX0LLGLLXF"; //confirm reset state
const char str_7490_3[] PROGMEM = "FXXXV11LLGHHXF"; //start counting
const char str_7490_4[] PROGMEM = "FXXXVXXLLGLLXF"; //count 0 - failed
//TO-DO: count sequence
//clock should be enabled 
//set inputs to low when counting
const char* const testPatterns_7490[] PROGMEM = {
  str_7490_1, str_7490_2, str_7490_3, str_7490_4
};

//39 - unfinished
//1clk, 1clr, 1k, v, 2clk, 2clr, 2j, 2qBar, 2q, 2k, G, 1q, 1qBar, 1J
const char str_7473_1[] PROGMEM = "X0XVX0XLHXGHLX"; //reset
const char str_7473_2[] PROGMEM = "";
const char* const testPatterns_7473[] PROGMEM = {
  str_7473_1
};

//40 - LOAD 1 FAILING (07.12.24) -JOAQUIN
//1clk, 1PRE(bar), 1clr(bar), in1J, V, 2clk, 2pre(bar), 2clr(bar), in2j, 2q(bar), out2Q, in2k, G, 1Q(bar), out1Q, in1k
const char str_7476_1[] PROGMEM = "F01XVF01XLHXGLHX"; //set
const char str_7476_2[] PROGMEM = "F10XVF10XHLXGHLX"; //reset
const char str_7476_3[] PROGMEM = "F111VF111XX1GXX1"; //prior setup - toggle
const char str_7476_4[] PROGMEM = "F111VF111HL1GHL1"; //toggle
const char str_7476_5[] PROGMEM = "F110VF110XX1GXX1"; //prior setup - load 0
const char str_7476_6[] PROGMEM = "F110VF110HL1GHL1"; //load 0
const char str_7476_7[] PROGMEM = "F10XVF10XHLXGHLX"; //reset
const char str_7476_8[] PROGMEM = "F111VF111XX0GXX0"; //prior setup - load 1
const char str_7476_9[] PROGMEM = "F111VF111LH0GLH0"; //load 1
const char* const testPatterns_7476[] PROGMEM = {
  str_7476_1, str_7476_2, str_7476_3, str_7476_4, str_7476_5, str_7476_6
};

//41 - TIMING ISSUE (07.16.24) - JOAQUIN
//cp1, mr1, mr2, nc, v, nc, nc, q2, q1, g, q3, q0, nc, cp0
const char str_7493_1[] PROGMEM = "F11XVXXLLGLLXF"; // mode select
const char str_7493_2[] PROGMEM = "FXXXVXXLLGLLXF"; // count 0
const char* const testPatterns_7493[] PROGMEM = {
  str_7493_1, str_7493_2  
};

//42 - sn74ls48n 7448
//B, C, LTbar, BI/RBObar, RBIbar, D, A, G
//e, d, c, b, a, g, f, v
const char str_7448_1[] PROGMEM = "0011100GHHHHHLHV"; //0
const char str_7448_2[] PROGMEM = "0011X0HGLLHHLLLV"; //1
const char str_7448_3[] PROGMEM = "XXX0XXXGLLLLLLLV"; //BI Bar - FAILING
const char* const testPatterns_7448[] PROGMEM = {
  str_7448_1, str_7448_2
};

//43 - lm358p
//out1, inverting input, non input, G, non inverting in2, inverting in 2, out2, V
const char str_358_1[] PROGMEM = "L10G01LV"; 
const char str_358_2[] PROGMEM = "H01G10GV"; 
const char* const testPatterns_358[] PROGMEM = {
  str_358_1, str_358_2
};

//cd4026be 4026
//cd4047bd 4047
//cd4033be 4033
//cd4078be 4078
//cd4013be 4013
//cd4060be 4060

const char ic_model_7400[] PROGMEM = "7400";      const char ic_model_7402[] PROGMEM = "7402";    const char ic_model_7404[] PROGMEM = "7404";
const char ic_model_7408[] PROGMEM = "7408";      const char ic_model_7432[] PROGMEM = "7432";    const char ic_model_7486[] PROGMEM = "7486";
const char ic_model_747266[] PROGMEM = "747266";  const char ic_model_7427[] PROGMEM = "7427";    const char ic_model_74151[] PROGMEM = "74151";
const char ic_model_7421[] PROGMEM = "7421";      const char ic_model_74192[] PROGMEM = "74192";  const char ic_model_7474[] PROGMEM = "7474";
const char ic_model_74190[] PROGMEM = "74190";    const char ic_model_74193[] PROGMEM = "74193";  const char ic_model_74195[] PROGMEM = "74195";
const char ic_model_7410[] PROGMEM = "7410";      const char ic_model_7411[] PROGMEM = "7411";    const char ic_model_74125[] PROGMEM = "74125";
const char ic_model_74138[] PROGMEM = "74138";    const char ic_model_7447[] PROGMEM = "7447";    const char ic_model_74173[] PROGMEM = "74173";
const char ic_model_4070[] PROGMEM = "4070";      const char ic_model_4071[] PROGMEM = "4071";    const char ic_model_4017[] PROGMEM = "4017";
const char ic_model_4511[] PROGMEM = "4511";      const char ic_model_4081[] PROGMEM = "4081";    const char ic_model_4077[] PROGMEM = "4077";
const char ic_model_4068[] PROGMEM = "4068";      const char ic_model_4069[] PROGMEM = "4069";    const char ic_model_4066[] PROGMEM = "4066";
const char ic_model_4094[] PROGMEM = "4094";      const char ic_model_74112[] PROGMEM = "74112";  const char ic_model_741[] PROGMEM = "741";
const char ic_model_072[] PROGMEM = "072";        const char ic_model_74373[] PROGMEM = "74373";  const char ic_model_74155[] PROGMEM = "74155";
const char ic_model_7483[] PROGMEM = "7483";      const char ic_model_7490[] PROGMEM = "7490";    const char ic_model_7473[] PROGMEM = "7473";
const char ic_model_7476[] PROGMEM ="7476";       const char ic_model_7493[] PROGMEM ="7493";     const char ic_model_7448[] PROGMEM = "7448";
const char ic_model_358[] PROGMEM = "358";

// Pin counts and test case numbers stored in PROGMEM
const byte pinCount8[] PROGMEM = {8}; const byte pinCount14[] PROGMEM = {14}; const byte pinCount16[] PROGMEM = {16}; const byte pinCount20[] PROGMEM = {20};

const byte numTestCases1[] PROGMEM = {1};   const byte numTestCases2[] PROGMEM = {2};   const byte numTestCases3[] PROGMEM = {3};
const byte numTestCases4[] PROGMEM = {4};   const byte numTestCases5[] PROGMEM = {5};   const byte numTestCases6[] PROGMEM = {6};
const byte numTestCases7[] PROGMEM = {7};   const byte numTestCases8[] PROGMEM = {8};   const byte numTestCases9[] PROGMEM = {9};
const byte numTestCases10[] PROGMEM = {10}; const byte numTestCases12[] PROGMEM = {12}; const byte numTestCases14[] PROGMEM = {14};
const byte numTestCases15[] PROGMEM = {15}; const byte numTestCases18[] PROGMEM = {18}; const byte numTestCases31[] PROGMEM = {31};

// Define an array of IC_TestPatterns, using PROGMEM for the array itself
const struct IC_TestPattern {
  const char* icType;
  const byte* pinCount;
  const byte* numTestCases;
  const char* const* testPatterns;
} testPatterns[] PROGMEM = {
  {reinterpret_cast<const char*>(ic_model_7400), pinCount14, numTestCases4, testPatterns_7400}, //1
  {reinterpret_cast<const char*>(ic_model_7402), pinCount14, numTestCases4, testPatterns_7402}, //2
  {reinterpret_cast<const char*>(ic_model_7404), pinCount14, numTestCases2, testPatterns_7404}, //3
  {reinterpret_cast<const char*>(ic_model_7408), pinCount14, numTestCases4, testPatterns_7408}, //4
  {reinterpret_cast<const char*>(ic_model_7432), pinCount14, numTestCases4, testPatterns_7432}, //5
  {reinterpret_cast<const char*>(ic_model_7486), pinCount14, numTestCases4, testPatterns_7486}, //6
  {reinterpret_cast<const char*>(ic_model_747266), pinCount14, numTestCases4, testPatterns_747266}, //7
  {reinterpret_cast<const char*>(ic_model_7427), pinCount14, numTestCases8, testPatterns_7427}, //8
  {reinterpret_cast<const char*>(ic_model_74151), pinCount16, numTestCases18, testPatterns_74151}, //9
  {reinterpret_cast<const char*>(ic_model_7421), pinCount14, numTestCases4, testPatterns_7421}, //10
  {reinterpret_cast<const char*>(ic_model_74192), pinCount16, numTestCases7, testPatterns_74192}, //11 BUGGED
  {reinterpret_cast<const char*>(ic_model_7474), pinCount14, numTestCases8, testPatterns_7474}, //12
  {reinterpret_cast<const char*>(ic_model_74190), pinCount16, numTestCases9, testPatterns_74190}, //13
  {reinterpret_cast<const char*>(ic_model_74193), pinCount16, numTestCases31, testPatterns_74193}, //14 BUGGED
  {reinterpret_cast<const char*>(ic_model_74195), pinCount16, numTestCases7, testPatterns_74195}, //15
  {reinterpret_cast<const char*>(ic_model_7410), pinCount14, numTestCases8, testPatterns_7410}, //16
  {reinterpret_cast<const char*>(ic_model_7411), pinCount14, numTestCases8, testPatterns_7411}, //17 
  {reinterpret_cast<const char*>(ic_model_74125), pinCount14, numTestCases8, testPatterns_74125}, //18
  {reinterpret_cast<const char*>(ic_model_74138), pinCount16, numTestCases12, testPatterns_74138}, //19
  {reinterpret_cast<const char*>(ic_model_7447), pinCount16, numTestCases10, testPatterns_7447}, //20
  {reinterpret_cast<const char*>(ic_model_74173), pinCount16, numTestCases15, testPatterns_74173}, //21
  {reinterpret_cast<const char*>(ic_model_4070), pinCount14, numTestCases4, testPatterns_4070}, //22
  {reinterpret_cast<const char*>(ic_model_4071), pinCount14, numTestCases4, testPatterns_4071}, //23
  {reinterpret_cast<const char*>(ic_model_4017), pinCount16, numTestCases12, testPatterns_4017}, //24
  {reinterpret_cast<const char*>(ic_model_4511), pinCount16, numTestCases5, testPatterns_4511}, //25
  {reinterpret_cast<const char*>(ic_model_4081), pinCount14, numTestCases4, testPatterns_4081}, //26
  {reinterpret_cast<const char*>(ic_model_4077), pinCount14, numTestCases4, testPatterns_4077}, //27
  {reinterpret_cast<const char*>(ic_model_4068), pinCount14, numTestCases10, testPatterns_4068}, //28
  {reinterpret_cast<const char*>(ic_model_4069), pinCount14, numTestCases2, testPatterns_4069}, //29
  {reinterpret_cast<const char*>(ic_model_4066), pinCount14, numTestCases4, testPatterns_4066}, //30
  {reinterpret_cast<const char*>(ic_model_4094), pinCount16, numTestCases14, testPatterns_4094}, //31
  {reinterpret_cast<const char*>(ic_model_74112), pinCount16, numTestCases8, testPatterns_74112}, //32
  {reinterpret_cast<const char*>(ic_model_741), pinCount8, numTestCases2, testPatterns_741}, //33
  {reinterpret_cast<const char*>(ic_model_072), pinCount8, numTestCases2, testPatterns_072}, //34
  {reinterpret_cast<const char*>(ic_model_74373), pinCount20, numTestCases2, testPatterns_74373}, //35
  {reinterpret_cast<const char*>(ic_model_74155), pinCount16, numTestCases6, testPatterns_74155}, //36
  {reinterpret_cast<const char*>(ic_model_7483), pinCount16, numTestCases2, testPatterns_7483}, //37
  {reinterpret_cast<const char*>(ic_model_7490), pinCount14, numTestCases4, testPatterns_7490}, //38
  {reinterpret_cast<const char*>(ic_model_7473), pinCount14, numTestCases1, testPatterns_7473}, //39
  {reinterpret_cast<const char*>(ic_model_7476), pinCount16, numTestCases6, testPatterns_7476}, //40
  {reinterpret_cast<const char*>(ic_model_7493), pinCount14, numTestCases2, testPatterns_7493}, //41
  {reinterpret_cast<const char*>(ic_model_7448), pinCount16, numTestCases2, testPatterns_7448}, //42
  {reinterpret_cast<const char*>(ic_model_358), pinCount8, numTestCases2, testPatterns_358} //43
};

byte upIndicator[] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B11011,
  B10001,
  B00000,
  B00000
};

byte downIndicator[] = {
  B00000,
  B00000,
  B10001,
  B11011,
  B01110,
  B00100,
  B00000,
  B00000
};

unsigned int potValue = 0;
boolean isRunning = false;

// To store the settings for pulse generator
float frequencyP0 = 0;
unsigned long periodP0 = 0;
int dutyCycleP0 = 0;
byte modeP0 = 0; // 0: Off, 1: Square Wave (Frequency), 2: Square Wave (Period), 3: PWM Percentage, 4: PWM 8-bit

float frequencyP1 = 0;
unsigned long periodP1 = 0;
int dutyCycleP1 = 0;
byte modeP1 = 0;

float frequencyP2 = 0;
unsigned long periodP2 = 0;
int dutyCycleP2 = 0;
byte modeP2 = 0;

float frequencyP3 = 0;
unsigned long periodP3 = 0;
int dutyCycleP3 = 0;
byte modeP3 = 0;

// FUNCTIONS

void init_ic_pins(){
  for(byte i=0;i<20;i++){
    pinMode(PINS_IC[i], INPUT);
  } 
}

/*
  TO-DO: 
    1. Check this function for logic errors causing failed tests on IC 74192 and 74193
    2. Reduce Delay Granularity: Start with smaller increments to find the optimal delay.
    3. Incremental Testing: Gradually increase the delay and observe the changes in the output.

  Checklist:
    >Structure of data: correct, no issues
    >Pin configurations: correct, no issues
    >Handling of clock pins: correct implementation on pulsing however, timing might be off.

  NOTES:
    >PROGMEM reading/extraction timing might be causing the error
    >Test with Initial Delay: Use the provided code and observe the output.
    >Adjust clockDelay: If the results are not as expected, increment clockDelay in small steps (e.g., 10-20 microseconds) until the outputs align correctly.
    >Monitor Responses: Continue to monitor the responses for each SignalIn and adjust the delay until you achieve consistent and correct outputs.   
*/
boolean testCase(PGM_P test, const byte* pins, int pinCount) {
    const int delayStep = 100; // Not sure if this delay will help reduce the noise between state transitions, still needed to be considered for fine tuning
    const int clockDelay = 200; // Starting with 100 microseconds
    const int interCycleDelay = 100; // Fixed delay between clock cycles

    boolean result = true;
    int clkPin = -1;
    bool downwardClock = false;

    char testBuffer[pinCount + 1];
    for (int i = 0; i < pinCount; i++) {
        testBuffer[i] = pgm_read_byte(test + i);
    }
    testBuffer[pinCount] = '\0';

    Serial.print("SignalIn : ");
    Serial.println(testBuffer);

    for (int i = 0; i < pinCount; i++) {
        char c = testBuffer[i];
        switch (c) {
            case 'V':
                pinMode(pins[i], OUTPUT);
                digitalWrite(pins[i], HIGH);
                delayMicroseconds(delayStep);
                break;
            case 'G':
                pinMode(pins[i], OUTPUT);
                digitalWrite(pins[i], LOW);
                delayMicroseconds(delayStep);
                break;
            case 'L':
                digitalWrite(pins[i], LOW);
                pinMode(pins[i], INPUT_PULLUP);
                delayMicroseconds(delayStep);
                break;
            case 'H':
                digitalWrite(pins[i], HIGH);
                pinMode(pins[i], INPUT_PULLUP);
                delayMicroseconds(delayStep);
                break;
            case '~':
            case 'Z':
                pinMode(pins[i], INPUT);
                delayMicroseconds(delayStep);
                break;
            
        }
    }

    delayMicroseconds(100); // stabilization delay
    // NOTE: This delay value might already be correct since most ICs can get away with it - Joaquin

    for (int i = 0; i < pinCount; i++) {
        char c = testBuffer[i];
        switch (c) {
            case 'X':
              digitalWrite(pins[i], LOW);
              pinMode(pins[i], OUTPUT);
              delayMicroseconds(delayStep);
            case '0':
                digitalWrite(pins[i], LOW);
                pinMode(pins[i], OUTPUT);
                delayMicroseconds(delayStep);
                break;
            case '1':
                digitalWrite(pins[i], HIGH);
                pinMode(pins[i], OUTPUT);
                delayMicroseconds(delayStep);
                break;
            case 'C':
                clkPin = pins[i];
                pinMode(pins[i], OUTPUT);
                digitalWrite(pins[i], LOW);
                break;
            case 'F':
                clkPin = pins[i];
                pinMode(pins[i], OUTPUT);
                digitalWrite(pins[i], HIGH);
                downwardClock = true;
                break;
        }
    }

    if (clkPin != -1) {
        pinMode(clkPin, OUTPUT);
        if (downwardClock) {
            digitalWrite(clkPin, HIGH);
            delayMicroseconds(clockDelay);
            digitalWrite(clkPin, LOW);
            delayMicroseconds(clockDelay);
            digitalWrite(clkPin, HIGH);
        } else {
            digitalWrite(clkPin, LOW);
            delayMicroseconds(clockDelay);
            digitalWrite(clkPin, HIGH);
            delayMicroseconds(clockDelay);
            digitalWrite(clkPin, LOW);
        }
    }

    delayMicroseconds(20); // Increase this delay if necessary
    // NOTE: This delay value might already be correct since most ICs can get away with it - Joaquin

    Serial.print("Response : ");
    boolean printed = false;
    for (int i = 0; i < pinCount; i++) {
        char expected = testBuffer[i];
        char actual = ' ';
        bool isOutputPin = false;

        switch (expected) {
            case 'H':
                delayMicroseconds(delayStep);
                actual = digitalRead(pins[i]) ? 'H' : 'L';
                isOutputPin = true;
                break;
            case 'L':
                delayMicroseconds(delayStep);
                actual = digitalRead(pins[i]) ? 'H' : 'L';
                isOutputPin = true;
                break;
            case 'Z':
                delayMicroseconds(delayStep);
                if (digitalRead(pins[i]) == HIGH || digitalRead(pins[i]) == LOW) {
                    actual = digitalRead(pins[i]) ? 'H' : 'L';
                } else {
                    actual = 'Z';
                }
                isOutputPin = true;
                break;
            case 'V':
                delayMicroseconds(delayStep);
                actual = digitalRead(pins[i]) ? 'V' : 'L';
                isOutputPin = false;
                break;
            case 'G':
                delayMicroseconds(delayStep);
                actual = digitalRead(pins[i]) ? 'V' : 'G';
                isOutputPin = false;
                break;
            default:
                actual = ' ';
                break;
        }

        if (isOutputPin) {
            if (expected != actual) {
                Serial.print(actual);
                printed = true;
                result = false;
            } else {
                Serial.print(' ');
            }
        }
    }

    if (!printed) {
        Serial.print(";");
    }

    Serial.println();
    reset_pin_config(pinCount);
    return result;
}


/*
  TO-DO: Review this funciton.
  
  Checklist:
    >Review this function if it is being called properly or as intended: seems to be correct.
    >Check if this function is using testCase properly and as intended: not sure.

  NOTES:
    >The values passed into these methods may be incorrect or may not be enough
*/
void get_test_case(byte icModel) {
  // Read PROGMEM pointers for the specified icModel
  PGM_P icType_p = reinterpret_cast<PGM_P>(pgm_read_ptr(&testPatterns[icModel - 1].icType));
  const byte* pinCount_p = reinterpret_cast<const byte*>(pgm_read_ptr(&testPatterns[icModel - 1].pinCount));
  byte pinCount = pgm_read_byte(pinCount_p);
  const byte* numTestCases_p = reinterpret_cast<const byte*>(pgm_read_ptr(&testPatterns[icModel - 1].numTestCases));
  byte numTestCases = pgm_read_byte(numTestCases_p);
  const char* const* testPatterns_p = reinterpret_cast<const char* const*>(pgm_read_ptr(&testPatterns[icModel - 1].testPatterns));

  // Print IC Type
  char icType_buffer[20];
  strcpy_P(icType_buffer, icType_p);
  Serial.print(F("Testing IC: "));
  Serial.println(icType_buffer);

  const byte* pins = nullptr;
  switch (pinCount) {
    case 8:
      pins = PINS_8;
      break;
    case 14:
      pins = PINS_14;
      break;
    case 16:
      pins = PINS_16;
      break;
    case 20:
      pins = PINS_20; // Assuming PINS_IC is used for 20 pin ICs
      break;
    default:
      Serial.println(F("Unsupported pin count"));
      return;
  }

  if (pins == nullptr) {
    Serial.println(F("Error: Pins not configured correctly."));
    return;
  }

  bool overallResult = true;

  for (byte i = 0; i < numTestCases; ++i) {
    const char* pattern_p = reinterpret_cast<const char*>(pgm_read_ptr(&testPatterns_p[i]));
    if (!testCase(pattern_p, pins, pinCount)) {
      overallResult = false;
    }
    delayMicroseconds(100);
    // Reset pin configurations after each test case
    reset_pin_config(pinCount);
  }

  if (overallResult) {
    Serial.println("IC Model " + String(icType_buffer) + " passed all tests.\n");
  } else {
    Serial.println("IC Model " + String(icType_buffer) + " failed.\n");
  }

  // Final reset after all test cases
  reset_pin_config(pinCount);
}

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

void reset_pin_config(byte pinCount) {
  const byte* pins = nullptr;

  switch (pinCount) {
    case 8:
      pins = PINS_8;
      break;
    case 14:
      pins = PINS_14;
      break;
    case 16:
      pins = PINS_16;
      break;
    case 20:
      pins = PINS_20;
      break;
    default:
      return; // Handle error or other pin counts as needed
  }

  for (int i = 0; i < pinCount; i++) {
    pinMode(pgm_read_byte(pins + i), INPUT);  // Reset pin mode to INPUT
    digitalWrite(pgm_read_byte(pins + i), LOW); // Reset pin state to LOW
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

  // Display up arrow if not on the first submenu
  if (submenuAuto > 2) {
    lcd.setCursor(15, 0);
    lcd.write(byte(0));
  }

  // Display down arrow if not on the last submenu
  if (submenuAuto < 3) {
    lcd.setCursor(15, 1);
    lcd.write(byte(1));
  }
}

/*
  TO-DO: None at the moment, implementation seems correct, returns submenu value which is the IC Model to be tested.
  IC Model value will be the parameter for get_test_case.
*/
void manual_user_interface() {
  lcd.clear();
  switch (submenu) {
    case 1:
      lcd.print((">IC 7400        "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 7402        "));
      break;
    case 2:
      lcd.print((" IC 7400        "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 7402        "));
      break;
    case 3:
      lcd.print((">IC 7404        "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 7408        "));
      break;
    case 4:
      lcd.print((" IC 7404        "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 7408        "));
      break;
    case 5:
      lcd.print((">IC 7432        "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 7486        "));
      break;
    case 6:
      lcd.print((" IC 7432        "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 7486        "));
      break;
    case 7:
      lcd.print((">IC 747266      "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 7427        "));
      break;
    case 8:
      lcd.print((" IC 747266      "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 7427        "));
      break;
    case 9:
      lcd.print((">IC 74151       "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 7421        "));
      break;
    case 10:
      lcd.print((" IC 74151       "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 7421        "));
      break;
    case 11:
      lcd.print((">IC 74192       "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 7474        "));
      break;
    case 12:
      lcd.print((" IC 74192       "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 7474        "));
      break;
    case 13:
      lcd.print((">IC 74190       "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 74193       "));
      break;
    case 14:
      lcd.print((" IC 74190       "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 74193       "));
      break;
    case 15:
      lcd.print((">IC 74195       "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 7410        "));
      break;
    case 16:
      lcd.print((" IC 74195       "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 7410        "));
      break;
    case 17:
      lcd.print((">IC 7411        "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 74125       "));
      break;
    case 18:
      lcd.print((" IC 7411        "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 74125       "));
      break;
    case 19:
      lcd.print((">IC 74138       "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 7447        "));
      break;
    case 20:
      lcd.print((" IC 74138       "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 7447        "));
      break;
    case 21:
      lcd.print((">IC 74173       "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 4070        "));
      break;
    case 22:
      lcd.print((" IC 74173       "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 4070        "));
      break;
    case 23:
      lcd.print((">IC 4071        "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 4017        "));
      break;
    case 24:
      lcd.print((" IC 4071        "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 4017        "));
      break;
    case 25:
      lcd.print((">IC 4511        "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 4081        "));
      break;
    case 26:
      lcd.print((" IC 4511        "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 4081        "));
      break;
    case 27:
      lcd.print((">IC 4077        "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 4068        "));
      break;
    case 28:
      lcd.print((" IC 4077        "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 4068        "));
      break;
    case 29:
      lcd.print((">IC 4069        "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 4066        "));
      break;
    case 30:
      lcd.print((" IC 4069        "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 4066        "));
      break;
    case 31:
      lcd.print((">IC 4094        "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 74112       "));
      break;
    case 32:
      lcd.print((" IC 4094        "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 74112       "));
      break;
    case 33:
      lcd.print((">IC 741         "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 072         "));
      break;
    case 34:
      lcd.print((" IC 741         "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 072         "));
      break;
    case 35:
      lcd.print((">IC 74373        "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 74155        "));
      break;
    case 36:
      lcd.print((" IC 74373        "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 74155        "));
      break;
    case 37:
      lcd.print((">IC 7483         "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 7490         "));
      break;
    case 38:
      lcd.print((" IC 7483         "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 7490         "));
      break;
    case 39:
      lcd.print((">IC 7473         "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 7476         "));
      break;
    case 40:
      lcd.print((" IC 7473         "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 7476         "));
      break;
    case 41:
      lcd.print((">IC 7493         "));
      lcd.setCursor(0, 1);
      lcd.print((" IC 7448         "));
      break;
    case 42:
      lcd.print((" IC 7493         "));
      lcd.setCursor(0, 1);
      lcd.print((">IC 7448         "));
      break;
    case 43:
      lcd.print((">IC 358          "));
      lcd.setCursor(0, 1);
      lcd.print(("                 "));
      break;
    default:
      submenu = 1;
      manual_user_interface();
      break;
  }

  // Display up arrow if not on the first submenu
  if (submenu > 2) {
    lcd.setCursor(15, 0);
    lcd.write(byte(0));
  }

  // Display down arrow if not on the last submenu
  if (submenu < 43) {
    lcd.setCursor(15, 1);
    lcd.write(byte(1));
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
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(">Probe"));
      lcd.setCursor(0, 1);
      lcd.print(F(" Pulse Generator"));
      break;
    case 4:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(" Probe"));
      lcd.setCursor(0, 1);
      lcd.print(F(">Pulse Generator"));
      break;
    default:
      menu = 1;
      update_menu();
      break;
  }

  // Display up arrow if not on the first submenu
  if (menu > 2) {
    lcd.setCursor(15, 0);
    lcd.write(byte(0));
  }

  // Display down arrow if not on the last submenu
  if (menu < 3) {
    lcd.setCursor(15, 1);
    lcd.write(byte(1));
  }
}

void probe_user_interface() {
  switch (submenuProbe) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(">Logic Probe    "));
      lcd.setCursor(0, 1);
      lcd.print(F(" Volt Meter     "));
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(" Logic Probe    "));
      lcd.setCursor(0, 1);
      lcd.print(F(">Volt Meter     "));
      break;
  }
}

void pulse_user_interface() {
  switch (submenuPulse) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(">Square Wave    "));
      lcd.setCursor(0, 1);
      lcd.print(F(" PWM            "));
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(" Square Wave    "));
      lcd.setCursor(0, 1);
      lcd.print(F(">PWM            "));
      break;
  }
}

void px_user_interface() {
  switch (submenuPx) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(">P0             "));
      lcd.setCursor(0, 1);
      lcd.print(F(" P1             "));
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(" P0             "));
      lcd.setCursor(0, 1);
      lcd.print(F(">P1             "));
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(">P2             "));
      lcd.setCursor(0, 1);
      lcd.print(F(" P3             "));
      break;
    case 4:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(" P2             "));
      lcd.setCursor(0, 1);
      lcd.print(F(">P3             "));
      break;
  }
}

void swave_user_interface() {
  switch (submenuSWave) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(">Frequency (Hz) "));
      lcd.setCursor(0, 1);
      lcd.print(F(" Period (micros)"));
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(" Frequency (Hz) "));
      lcd.setCursor(0, 1);
      lcd.print(F(">Period (micros)"));
      break;
  }
}

void dutycycle_user_interface() {
  switch (submenuDCycle) {
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(">Percentage     "));
      lcd.setCursor(0, 1);
      lcd.print(F(" 8-bit          "));
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(" Percentage     "));
      lcd.setCursor(0, 1);
      lcd.print(F(">8-bit          "));
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(">Period     "));
      break;
  }
}

void updatePassedModelsDisplay() {
  lcd.clear();
  if (passedCount > 0) {
    lcd.setCursor(0, 0);
    lcd.print(F("Passed Models:"));
    lcd.setCursor(0, 1);
    lcd.print(passedModels[currentModelIndex]);

    byte size_db = sizeof(passedModels) / sizeof(passedModels[0]);
    Serial.println(F("Passed Models:"));
    for (byte i = 0; i < size_db; i++) {
      Serial.println(passedModels[i]);
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print(F("No models passed"));
    Serial.println(F("No models passed"));
  }
}

void turnOffBacklight() {
  analogWrite(PIN_RGBLED_R, 0);
  analogWrite(PIN_RGBLED_G, 0);
  analogWrite(PIN_RGBLED_B, 0);
}

void setBacklightColor(uint8_t red, uint8_t green, uint8_t blue) {
  analogWrite(PIN_RGBLED_R, red);
  analogWrite(PIN_RGBLED_G, green);
  analogWrite(PIN_RGBLED_B, blue);
}

void logic_probe() {
  const uint8_t RED[] = {255, 0, 0}; // LOW
  const uint8_t GREEN[] = {0, 255, 0}; // HIGH
  const uint8_t CYAN[] = {0, 255, 255}; // HIGH-Z
  int analogValue = analogRead(PIN_PROBE);
  float voltage = (analogValue / 1023.0) * 5.0; // Convert to voltage

  lcd.clear();
  lcd.setCursor(0, 0);

  if (voltage >= HIGH_THRESHOLD) {
    lcd.print(F("Voltage: HIGH"));
    setBacklightColor(GREEN[0], GREEN[1], GREEN[2]);
  } else if (voltage <= LOW_THRESHOLD) {
    lcd.print(F("Voltage: LOW"));
    setBacklightColor(RED[0], RED[1], RED[2]);
  } else {
    lcd.print(F("Voltage: HIGH-Z"));
    setBacklightColor(CYAN[0], CYAN[1], CYAN[2]);
  }

  // For debugging purposes
  Serial.print("Analog Value: ");
  Serial.print(analogValue);
  Serial.print(" Voltage: ");
  Serial.println(voltage);
  delay(2000);
  turnOffBacklight();
  probe_user_interface();
}

void volt_meter() {
  while (true) {
    int analogValue = analogRead(PIN_PROBE);
    float voltage = (analogValue / 1023.0) * 5.0; // Convert to a voltage

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("Voltage:"));
    lcd.setCursor(0, 1);
    lcd.print(voltage, 2); // Two decimal places
    lcd.print(F(" V"));

    // For debugging purposes
    Serial.print("Analog Value: ");
    Serial.print(analogValue);
    Serial.print(" Voltage: ");
    Serial.println(voltage);

    delay(150);

    if (digitalRead(PIN_BTN_CANCEL) == LOW) {
      return;
    }
  }

  menu = 6;
  submenuProbe = 1;
  probe_user_interface();
}

// Function to generate a square wave with a frequency adjusted by the potentiometer
void generatePulseSquareWave(byte pin) {
  while (true) {
    unsigned int potValue = potreader();
    float frequency = map(potValue, 0, 1023, 1, 100); // Map pot value to frequency range (1Hz to 100Hz)

    if (pin == PIN_PWM_P0) {
    frequencyP0 = frequency;
    modeP0 = 1;
    } else if (pin == PIN_PWM_P1) {
      frequencyP1 = frequency;
      modeP1 = 1;
    } else if (pin == PIN_PWM_P2) {
      frequencyP2 = frequency;
      modeP2 = 1;
    } else if (pin == PIN_PWM_P3) {
      frequencyP3 = frequency;
      modeP3 = 1;
    }

    unsigned long period = 1000000UL / frequency; // Period in microseconds
    unsigned long halfPeriod = period / 2; // Half period for HIGH and LOW states

    Serial.print("Frequency: ");
    Serial.print(frequency);
    Serial.println(" Hz");
    lcd.clear();
    lcd.print("Hz: ");
    lcd.setCursor(0, 2);
    lcd.print(frequency);

    // Generate the square wave
    for (unsigned long start = millis(); millis() - start < 50;) { // Keep it non-blocking for the button check
      digitalWrite(pin, HIGH);
      delayMicroseconds(halfPeriod);
      digitalWrite(pin, LOW);
      delayMicroseconds(halfPeriod);
    }

    // Break if the cancel button is pressed
    if (digitalRead(PIN_BTN_CANCEL) == LOW) {
      digitalWrite(pin, LOW);
      break;
    }
  }
}

// Function to generate a square wave with a period adjusted by the potentiometer
void generatePulseSquareWavePeriod(byte pin) {
  while (true) {
    unsigned int potValue = potreader();
    unsigned long period = map(potValue, 0, 1023, 1, 1000); // Map pot value to period range (1ms to 1000ms)
    unsigned long halfPeriod = period * 1000UL / 2; // Half period in microseconds

    if (pin == PIN_PWM_P0) {
      periodP0 = period;
      modeP0 = 2;
    } else if (pin == PIN_PWM_P1) {
      periodP1 = period;
      modeP1 = 2;
    } else if (pin == PIN_PWM_P2) {
      periodP2 = period;
      modeP2 = 2;
    } else if (pin == PIN_PWM_P3) {
      periodP3 = period;
      modeP3 = 2;
    }

    Serial.print("Period: ");
    Serial.print(period);
    Serial.println(" microsecond");
    lcd.clear();
    lcd.print("microsecond: ");
    lcd.setCursor(0, 2);
    lcd.print(period);

    // Generate the square wave
    for (unsigned long start = millis(); millis() - start < 50;) { // Keep it non-blocking for the button check
      digitalWrite(pin, HIGH);
      delayMicroseconds(halfPeriod);
      digitalWrite(pin, LOW);
      delayMicroseconds(halfPeriod);
    }

    // Break if the cancel button is pressed
    if (digitalRead(PIN_BTN_CANCEL) == LOW) {
      digitalWrite(pin, LOW);
      break;
    }
  }
}

// Function to generate a PWM signal with duty cycle adjusted by the potentiometer (0-100%)
void generatePWMPulsePercentage(byte pin) {
  while (true) {
    unsigned int potValue = potreader();
    float dutyCycle = map(potValue, 0, 1023, 0, 100); // Map pot value to duty cycle range (0% to 100%)

    if (pin == PIN_PWM_P0) {
      dutyCycleP0 = dutyCycle;
      modeP0 = 3;
    } else if (pin == PIN_PWM_P1) {
      dutyCycleP1 = dutyCycle;
      modeP1 = 3;
    } else if (pin == PIN_PWM_P2) {
      dutyCycleP2 = dutyCycle;
      modeP2 = 3;
    } else if (pin == PIN_PWM_P3) {
      dutyCycleP3 = dutyCycle;
      modeP3 = 3;
    }
    
    analogWrite(pin, dutyCycle * 2.55); // Convert dutyCycle percentage to 0-255 range

    Serial.print("Duty Cycle: ");
    Serial.print(dutyCycle);
    Serial.println(" %");
    lcd.clear();
    lcd.print("Duty Cycle: ");
    lcd.setCursor(0, 1);
    lcd.print(dutyCycle);
    lcd.print(" %");

    // Break if the cancel button is pressed
    if (digitalRead(PIN_BTN_CANCEL) == LOW) {
      analogWrite(pin, 0);
      break;
    }
    
    delay(50); // Adjust delay as needed
  }
}

// Function to generate a PWM signal with duty cycle adjusted by the potentiometer (8-bit: 0-255)
void generatePWMPulse8Bit(byte pin) {
  while (true) {
    unsigned int potValue = potreader();
    int dutyCycle = map(potValue, 0, 1023, 0, 255); // Map pot value to duty cycle range (0 to 255)

    if (pin == PIN_PWM_P0) {
      dutyCycleP0 = dutyCycle;
      modeP0 = 4;
    } else if (pin == PIN_PWM_P1) {
      dutyCycleP1 = dutyCycle;
      modeP1 = 4;
    } else if (pin == PIN_PWM_P2) {
      dutyCycleP2 = dutyCycle;
      modeP2 = 4;
    } else if (pin == PIN_PWM_P3) {
      dutyCycleP3 = dutyCycle;
      modeP3 = 4;
    }
    
    analogWrite(pin, dutyCycle);

    Serial.print("Duty Cycle: ");
    Serial.print(dutyCycle);
    Serial.println(" (8-bit)");
    lcd.clear();
    lcd.print("Duty Cycle: ");
    lcd.setCursor(0, 1);
    lcd.print(dutyCycle);
    lcd.print(" (8-bit)");

    // Break if the cancel button is pressed
    if (digitalRead(PIN_BTN_CANCEL) == LOW) {
      analogWrite(pin, 0);
      break;
    }
    
    delay(50); // Adjust delay as needed
  }
}

// Function to generate a PWM signal with a specific period and the already set duty cycle
void generatePWMWithPeriod(byte pin) {
  while (true) {
    unsigned int potValue = potreader();

    // Map pot value to period range (1ms to 1000ms)
    unsigned long period = map(potValue, 0, 1023, 1, 1000); // Period in milliseconds

    float dutyCycle = 0;
    if (pin == PIN_PWM_P0) {
      if (dutyCycleP0 == 0) {
        dutyCycle = 50;
        periodP0 = period;
        modeP0 = 5; // New mode for custom PWM with period
      } else {
        dutyCycle = dutyCycleP0;
        periodP0 = period;
        modeP0 = 5; // New mode for custom PWM with period
      }
    } else if (pin == PIN_PWM_P1) {
      if (dutyCycleP1 == 0) {
        dutyCycle = 50;
        periodP1 = period;
        modeP1 = 5;
      } else {
        dutyCycle = dutyCycleP0;
        periodP1 = period;
        modeP1 = 5;
      }
    } else if (pin == PIN_PWM_P2) {
      if (dutyCycleP2 == 0) {
        dutyCycle = 50;
        periodP2 = period;
        modeP2 = 5;
      } else {
        dutyCycle = dutyCycleP0;
        periodP2 = period;
        modeP2 = 5;
      }
    } else if (pin == PIN_PWM_P3) {
      if (dutyCycleP3 == 0) {
        dutyCycle = 50;
        periodP3 = period;
        modeP3 = 5;
      } else {
        dutyCycle = dutyCycleP0;
        periodP3 = period;
        modeP3 = 5;
      }
    }

    // Calculate on-time and off-time in microseconds
    unsigned long onTime = (period * 1000UL * dutyCycle) / 100; // On-time in microseconds
    unsigned long offTime = (period * 1000UL) - onTime; // Off-time in microseconds

    Serial.print("Period: ");
    Serial.print(period);
    Serial.print(" ms, Duty Cycle: ");
    Serial.print(dutyCycle);
    Serial.println(" %");
    lcd.clear();
    lcd.print("Period: ");
    lcd.print(period);
    lcd.print(" ms");
    lcd.setCursor(0, 1);
    lcd.print("Duty Cycle: ");
    lcd.print(dutyCycle);
    lcd.print(" %");

    // Generate the PWM signal
    for (unsigned long start = millis(); millis() - start < 50;) { // Keep it non-blocking for the button check
      digitalWrite(pin, HIGH);
      delayMicroseconds(onTime);
      digitalWrite(pin, LOW);
      delayMicroseconds(offTime);
    }

    // Break if the cancel button is pressed
    if (digitalRead(PIN_BTN_CANCEL) == LOW) {
      digitalWrite(pin, LOW);
      break;
    }
  }
}

void generateSignal(byte pin, float frequency, unsigned long period, int dutyCycle, byte mode) {
  if (mode == 1) {
    unsigned long halfPeriod = 1000000UL / (2 * frequency);
    digitalWrite(pin, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(pin, LOW);
    delayMicroseconds(halfPeriod);
  } else if (mode == 2) {
    unsigned long halfPeriod = period * 1000UL / 2;
    digitalWrite(pin, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(pin, LOW);
    delayMicroseconds(halfPeriod);
  } else if (mode == 3) {
    analogWrite(pin, dutyCycle * 2.55); // Convert dutyCycle percentage to 0-255 range
  } else if (mode == 4) {
    analogWrite(pin, dutyCycle);
  }
}

unsigned int potreader(){
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
  return a;
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

/*
  TO-DO: Review this function if it is calling get_test_case properly
  
  Checklist:
    >None

  NOTES:
    >None
*/
void buttonScanner() {
  if (menu == 1 || menu == 2 || menu == 3 || menu == 4) {
    // Main menu navigation
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (menu > 1) menu--;
      update_menu();
    }

    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (menu < 4) menu++;
      update_menu();
    }

    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag
      if (menu == 1) {
        automatic_user_interface(); // Enter submenu
        menu = 10; // Switch to submenu mode
      } else if (menu == 2) {
        manual_user_interface();
        menu = 11;
      } else if (menu == 3) {
        probe_user_interface(); // Enter Probe submenu
        menu = 12;
      } else if (menu == 4) {
        px_user_interface(); // Enter Pulse Generator submenu
        menu = 21;
      }
    }

    if (flag_button[5]) { // CANCEL button
      flag_button[5] = false; // Reset flag
      // Implement cancel action if needed
    }
  } else if (menu == 11) {
    // Manual submenu navigation
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (submenu > 1) submenu--;
      else submenu =43; // Wrap around to last option
      manual_user_interface();
    }

    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenu < 43) submenu++;
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
  } else if (menu == 10) {
    // Automatic submenu navigation
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
  } else if (menu == 12) {
    // Probe submenu navigation
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (submenuProbe > 1) submenuProbe--;
      else submenuProbe = 2; // Wrap around to last option
      probe_user_interface();
    }

    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenuProbe < 2) submenuProbe++;
      else submenuProbe = 1; // Wrap around to first option
      probe_user_interface();
    }

    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag
      if (submenuProbe == 1) logic_probe();
      else if (submenuProbe == 2) volt_meter();
    }

    if (flag_button[5]) { // CANCEL button
      flag_button[5] = false; // Reset flag
      menu = 1; // Go back to main menu
      update_menu();
    }
  } else if (menu == 13) {
    // Pulse submenu navigation
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (submenuPulse > 1) submenuPulse--;
      else submenuPulse = 2; // Wrap around to last option
      pulse_user_interface();
    }

    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenuPulse < 2) submenuPulse++;
      else submenuPulse = 1; // Wrap around to first option
      pulse_user_interface();
    }

    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag
      if (submenuPulse == 1) {
        swave_user_interface();
        menu = 20;
      } else if (submenuPulse == 2) {
        dutycycle_user_interface();
        menu = 22;
      }
    }

    if (flag_button[5]) { // CANCEL button
      flag_button[5] = false; // Reset flag
      menu = 1; // Go back to main menu
      update_menu();
    }
  } else if (menu == 20) {
    // Square Wave submenu navigation
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (submenuSWave > 1) submenuSWave--;
      else submenuSWave = 2; // Wrap around to last option
      swave_user_interface();
    }

    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenuSWave < 2) submenuSWave++;
      else submenuSWave = 1; // Wrap around to first option
      swave_user_interface();
    }

    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag

      // Square wave frequency and period P0-P3
      if (submenuPx == 1) {
        if (submenuSWave == 1) {
          generatePulseSquareWave(PIN_PWM_P0);
        } else if (submenuSWave == 2) {
          generatePulseSquareWavePeriod(PIN_PWM_P0);
        }
      } else if (submenuPx == 2) {
        if (submenuSWave == 1) {
          generatePulseSquareWave(PIN_PWM_P1);
        } else if (submenuSWave == 2) {
          generatePulseSquareWavePeriod(PIN_PWM_P1);
        }
      } else if (submenuPx == 3) {
        if (submenuSWave == 1) {
          generatePulseSquareWave(PIN_PWM_P2);
        } else if (submenuSWave == 2) {
          generatePulseSquareWavePeriod(PIN_PWM_P2);
        }
      } else if (submenuPx == 4) {
        if (submenuSWave == 1) {
          generatePulseSquareWave(PIN_PWM_P3);
        } else if (submenuSWave == 2) {
          generatePulseSquareWavePeriod(PIN_PWM_P3);
        }
      }
    }

    if (flag_button[5]) { // CANCEL button
      flag_button[5] = false; // Reset flag
      menu = 13; // Go back to submenu
      pulse_user_interface();
    }
  } else if (menu == 21) {
    // Select Px submenu navigation (SQUARE WAVE)
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (submenuPx > 1) submenuPx--;
      else submenuPx = 4; // Wrap around to last option
      px_user_interface();
    }

    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenuPx < 4) submenuPx++;
      else submenuPx = 1; // Wrap around to first option
      px_user_interface();
    }

    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag
      if (submenuPx == 1) {
        menu = 13;
        pulse_user_interface();
      } else if (submenuPx == 2) {
        menu = 13;
        pulse_user_interface();
      } else if (submenuPx == 3) {
        menu = 13;
        pulse_user_interface();
      } else if (submenuPx == 4) {
        menu = 13;
        pulse_user_interface();
      }
    }

    if (flag_button[5]) { // CANCEL button
      flag_button[5] = false; // Reset flag
      menu = 20; // Go back to submenu
      swave_user_interface();
    }
  } else if (menu == 22) {
    // PWM Duty Cycle submenu navigation
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (submenuDCycle > 1) submenuDCycle--;
      else submenuDCycle = 3; // Wrap around to last option
      dutycycle_user_interface();
    }

    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenuDCycle < 3) submenuDCycle++;
      else submenuDCycle = 1; // Wrap around to first option
      dutycycle_user_interface();
    }

    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag

      // PWM Duty Cycle wave frequency and period P0-P3
      if (submenuPx == 1) {
        if (submenuDCycle == 1) {
          generatePWMPulsePercentage(PIN_PWM_P0);
        } else if (submenuDCycle == 2) {
          generatePWMPulse8Bit(PIN_PWM_P0);
        } else if (submenuDCycle == 3) {
          generatePWMWithPeriod(PIN_PWM_P0);
        }
      } else if (submenuPx == 2) {
        if (submenuDCycle == 1) {
          generatePWMPulsePercentage(PIN_PWM_P1);
        } else if (submenuDCycle == 2) {
          generatePWMPulse8Bit(PIN_PWM_P1);
        } else if (submenuDCycle == 3) {
          generatePWMWithPeriod(PIN_PWM_P0);
        }
      } else if (submenuPx == 3) {
        if (submenuDCycle == 1) {
          generatePWMPulsePercentage(PIN_PWM_P2);
        } else if (submenuDCycle == 2) {
          generatePWMPulse8Bit(PIN_PWM_P2);
        } else if (submenuDCycle == 3) {
          generatePWMWithPeriod(PIN_PWM_P0);
        }
      } else if (submenuPx == 4) {
        if (submenuDCycle == 1) {
          generatePWMPulsePercentage(PIN_PWM_P3);
        } else if (submenuDCycle == 2) {
          generatePWMPulse8Bit(PIN_PWM_P3);
        } else if (submenuDCycle == 3) {
          generatePWMWithPeriod(PIN_PWM_P0);
        }
      }
    }

    if (flag_button[5]) { // CANCEL button
      flag_button[5] = false; // Reset flag
      menu = 13; // Go back to submenu
      dutycycle_user_interface();
    }
  } else if (menu == 23) {
    // Select Px submenu navigation (SQUARE WAVE)
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (submenuPx > 1) submenuPx--;
      else submenuPx = 4; // Wrap around to last option
      px_user_interface();
    }

    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (submenuPx < 4) submenuPx++;
      else submenuPx = 1; // Wrap around to first option
      px_user_interface();
    }

    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag
      if (submenuPx == 1) {
        menu = 13;
        px_user_interface();
      } else if (submenuPx == 2) {
        menu = 13;
        px_user_interface();
      } else if (submenuPx == 3) {
        menu = 13;
        px_user_interface();
      } else if (submenuPx == 4) {
        menu = 13;
        px_user_interface();
      }
    }

    if (flag_button[5]) { // CANCEL button
      flag_button[5] = false; // Reset flag
      menu = 22; // Go back to submenu
      dutycycle_user_interface();
    }
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
  pinMode(PIN_RGBLED_G, OUTPUT); 
  pinMode(PIN_RGBLED_B, OUTPUT);
  pinMode(PIN_LED1, OUTPUT);

  lcd.createChar(0, upIndicator);
  lcd.createChar(1, downIndicator);

  pinMode(PIN_PWM_P0, OUTPUT);
  pinMode(PIN_PWM_P1, OUTPUT);
  pinMode(PIN_PWM_P2, OUTPUT);
  pinMode(PIN_PWM_P3, OUTPUT);

  pinMode(PIN_PROBE, INPUT);

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
  //heartbeatLED();
  potreader();

  generateSignal(PIN_PWM_P0, frequencyP0, periodP0, dutyCycleP0, modeP0);
  generateSignal(PIN_PWM_P1, frequencyP1, periodP1, dutyCycleP1, modeP1);
  generateSignal(PIN_PWM_P2, frequencyP2, periodP2, dutyCycleP2, modeP2);
  generateSignal(PIN_PWM_P3, frequencyP3, periodP3, dutyCycleP3, modeP3);

  if (Serial.available()) {
    char val = Serial.read();

    // For automatic search
    if (val == '0') {
      autoSearch(14);
    }
    if (val == '1') {
      autoSearch(16);
    }
    if (val == '2') {
      autoSearch(8);
    }
    if (val == '3') {
      autoSearch(20);
    }

    // For manual testing
    if (val == '4') { // 7400
      Serial.println("Manual test: 7400");
      get_test_case(1);
    }
    if (val == '5') { // 7402
      Serial.println("Manual test: 7402");
      get_test_case(2);
    }
    if (val == '6') { // 7404
      Serial.println("Manual test: 7404");
      get_test_case(3);
    }
    if (val == '7') { // 7408
      Serial.println("Manual test: 7408");
      get_test_case(4);
    }
    if (val == '8') { // 7432
      Serial.println("Manual test: 7432");
      get_test_case(5);
    }
    if (val == '9') { // 7486
      Serial.println("Manual test: 7486");
      get_test_case(6);
    }
    if (val == 'a') { // 747266
      Serial.println("Manual test: 747266");
      get_test_case(7);
    }
    if (val == 'b') { // 7427
      Serial.println("Manual test: 7427");
      get_test_case(8);
    }
    if (val == 'c') { // 74151
      Serial.println("Manual test: 74151");
      get_test_case(9);
    }
    if (val == 'd') { // 7421
      Serial.println("Manual test: 7421");
      get_test_case(10);
    }
    if (val == 'e') { // 74192
      Serial.println("Manual test: 74192");
      get_test_case(11);
    }
    if (val == 'f') { // 7474
      Serial.println("Manual test: 7474");
      get_test_case(12);
    }
    if (val == 'g') { // 74190
      Serial.println("Manual test: 74190");
      get_test_case(13);
    }
    if (val == 'h') { // 74193
      Serial.println("Manual test: 74193");
      get_test_case(14);
    }
    if (val == 'i') { // 74195
      Serial.println("Manual test: 74195");
      get_test_case(15);
    }
    if (val == 'j') { // 7410
      Serial.println("Manual test: 7410");
      get_test_case(16);
    }
    if (val == 'k') { // 7411
      Serial.println("Manual test: 7411");
      get_test_case(17);
    }
    if (val == 'l') { // 74125
      Serial.println("Manual test: 74125");
      get_test_case(18);
    }
    if (val == 'm') { // 74138
      Serial.println("Manual test: 74138");
      get_test_case(19);
    }
    if (val == 'n') { // 7447
      Serial.println("Manual test: 7447");
      get_test_case(20);
    }
    if (val == 'o') { // 74173
      Serial.println("Manual test: 74173");
      get_test_case(21);
    }
    if (val == 'p') { // 4070
      Serial.println("Manual test: 4070");
      get_test_case(22);
    }
    if (val == 'q') { // 4071
      Serial.println("Manual test: 4071");
      get_test_case(23);
    }
    if (val == 'r') { // 4017
      Serial.println("Manual test: 4017");
      get_test_case(24);
    }
    if (val == 's') { // 4511
      Serial.println("Manual test: 4511");
      get_test_case(25);
    }
    if (val == 't') { // 4081
      Serial.println("Manual test: 4081");
      get_test_case(26);
    }
    if (val == 'u') { // 4077
      Serial.println("Manual test: 4077");
      get_test_case(27);
    }
    if (val == 'v') { // 4068
      Serial.println("Manual test: 4068");
      get_test_case(28);
    }
    if (val == 'w') { // 4069
      Serial.println("Manual test: 4069");
      get_test_case(29);
    }
    if (val == 'x') { // 4066
      Serial.println("Manual test: 4066");
      get_test_case(30);
    }
    if (val == 'y') { // 4094
      Serial.println("Manual test: 4094");
      get_test_case(31);
    }
    if (val == 'z') { // 74112
      Serial.println("Manual test: 74112");
      get_test_case(32);
    }
    if (val == 'A') { // 741
      Serial.println("Manual test: 741");
      get_test_case(33);
    }
    if (val == 'B') { // 072
      Serial.println("Manual test: 072");
      get_test_case(34);
    }
    if (val == 'C') { // 74373
      Serial.println("Manual test: 74373");
      get_test_case(35);
    }
  }
}
