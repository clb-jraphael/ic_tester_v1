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

byte menu = 1, submenu = 1, submenuAuto = 1, submenuProbe = 1, num = 1, currentModelIndex = 0, passedCount = 0;
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
const char* const testPatterns_7421[] PROGMEM = {str_7421_1, str_7421_2, str_7421_3, str_7421_4};
//11
const char str_74192_1[] PROGMEM = "0LL11LLG1011110V"; 
const char str_74192_2[] PROGMEM = "0LL11LLG1011100V";
const char str_74192_3[] PROGMEM = "0LL11LHG1001100V";
const char str_74192_4[] PROGMEM = "0LL11LHG1011100V";
const char str_74192_5[] PROGMEM = "0LL10LHG1011100V";
const char str_74192_6[] PROGMEM = "0LH11LHG1011100V";
const char str_74192_7[] PROGMEM = "0LH10LHG1010100V";
const char* const testPatterns_74192[] PROGMEM = {str_74192_1, str_74192_2, str_74192_3, str_74192_4, str_74192_5, str_74192_6, str_74192_7};
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
const char str_74193_1[] PROGMEM =    "1HL11LHG100HH00V";
const char str_74193_2[] PROGMEM =    "0LH11HLG010HH01V";
const char str_74193_3[] PROGMEM =    "1HH11HHG110HH01V";
const char str_74193_4[] PROGMEM =    "1HLC1HHG111HH01V";
const char str_74193_5[] PROGMEM =    "1LHC1HHG111HH01V";
const char str_74193_6[] PROGMEM =    "1LLC1HHG111HH01V";
const char str_74193_7[] PROGMEM =    "1HHC1LHG111HH01V";
const char str_74193_8[] PROGMEM =    "1HLC1LHG111HH01V";
const char str_74193_9[] PROGMEM =    "1LHC1LHG111HH01V";
const char str_74193_10[] PROGMEM =   "1LLC1LHG111HH01V";
const char str_74193_11[] PROGMEM =   "1HHC1HLG111HH01V";
const char str_74193_12[] PROGMEM =   "1HLC1HLG111HH01V";
const char str_74193_13[] PROGMEM =   "1LHC1HLG111HH01V";
const char str_74193_14[] PROGMEM =   "1LLC1HLG111HH01V";
const char str_74193_15[] PROGMEM =   "1HHC1LLG111HH01V";
const char str_74193_16[] PROGMEM =   "1HLC1LLG111HH01V";
const char str_74193_17[] PROGMEM =   "1LHC1LLG111HH01V";
const char str_74193_18[] PROGMEM =   "1LL11LLG111HH01V";
const char str_74193_19[] PROGMEM =   "1LL01LLG111HL01V";
const char str_74193_20[] PROGMEM =   "1HH11HHG111HH01V";
const char str_74193_21[] PROGMEM =   "1HH10HHG111LH01V";
const char str_74193_22[] PROGMEM =   "1LL11LLG111HH01V";
const char str_74193_23[] PROGMEM =   "1LH1CLLG111HH01V";
const char str_74193_24[] PROGMEM =   "1HL1CLLG111HH01V";
const char str_74193_25[] PROGMEM =   "1HH1CLLG111HH01V";
const char str_74193_26[] PROGMEM =   "1LL1CHLG111HH01V";
const char str_74193_27[] PROGMEM =   "1LH1CHLG111HH01V";
const char str_74193_28[] PROGMEM =   "1HL1CHLG111HH01V";
const char str_74193_29[] PROGMEM =   "1HH1CHLG111HH01V";
const char str_74193_30[] PROGMEM =   "1LL1CLHG111HH01V";
const char str_74193_31[] PROGMEM =   "1LL11LLG111HHC1V";
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
const char str_4068_1[] PROGMEM =  "011110G01111LV";
const char str_4068_2[] PROGMEM =  "010100G01010HV";
const char str_4068_3[] PROGMEM =  "001010G00101HV";
const char str_4068_4[] PROGMEM =  "000110G00011HV";
const char str_4068_5[] PROGMEM =  "011000G01100HV";
const char str_4068_6[] PROGMEM =  "000000G01111HV";
const char str_4068_7[] PROGMEM =  "000000G00000HV";
const char str_4068_8[] PROGMEM =  "011110G01110HV";
const char str_4068_9[] PROGMEM =  "011010G01111HV";
const char str_4068_10[] PROGMEM = "001110G00111HV";
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
//35
const char str_071_1[] PROGMEM = "~10G~HV~";
const char str_071_2[] PROGMEM = "~01G~HV~";
const char* const testPatterns_071[] PROGMEM = {
  str_071_1, str_071_2
};

// IC model strings stored in PROGMEM
//1
const char ic_model_7400[] PROGMEM = "7400";
//2
const char ic_model_7402[] PROGMEM = "7402";
//3
const char ic_model_7404[] PROGMEM = "7404";
//4
const char ic_model_7408[] PROGMEM = "7408";
//5
const char ic_model_7432[] PROGMEM = "7432";
//6
const char ic_model_7486[] PROGMEM = "7486";
//7
const char ic_model_747266[] PROGMEM = "747266";
//8
const char ic_model_7427[] PROGMEM = "7427";
//9
const char ic_model_74151[] PROGMEM = "74151";
//10
const char ic_model_7421[] PROGMEM = "7421";
//11
const char ic_model_74192[] PROGMEM = "74192";
//12
const char ic_model_7474[] PROGMEM = "7474";
//13
const char ic_model_74190[] PROGMEM = "74190";
//14
const char ic_model_74193[] PROGMEM = "74193";
//15
const char ic_model_74195[] PROGMEM = "74195";
//16
const char ic_model_7410[] PROGMEM = "7410";
//17
const char ic_model_7411[] PROGMEM = "7411";
//18
const char ic_model_74125[] PROGMEM = "74125";
//19
const char ic_model_74138[] PROGMEM = "74138";
//20
const char ic_model_7447[] PROGMEM = "7447";
//21
const char ic_model_74173[] PROGMEM = "74173";
//22
const char ic_model_4070[] PROGMEM = "4070";
//23
const char ic_model_4071[] PROGMEM = "4071";
//24
const char ic_model_4017[] PROGMEM = "4017";
//25
const char ic_model_4511[] PROGMEM = "4511";
//26
const char ic_model_4081[] PROGMEM = "4081";
//27
const char ic_model_4077[] PROGMEM = "4077";
//28
const char ic_model_4068[] PROGMEM = "4068";
//29
const char ic_model_4069[] PROGMEM = "4069";
//30
const char ic_model_4066[] PROGMEM = "4066";
//31
const char ic_model_4094[] PROGMEM = "4094";
//32
const char ic_model_74112[] PROGMEM = "74112";
//33
const char ic_model_741[] PROGMEM = "741";
//34
const char ic_model_072[] PROGMEM = "072";
//35
const char ic_model_071[] PROGMEM = "071";

// Pin counts and test case numbers stored in PROGMEM
const byte pinCount8[] PROGMEM = {8};
const byte pinCount14[] PROGMEM = {14};
const byte pinCount16[] PROGMEM = {16};
const byte pinCount20[] PROGMEM = {20};

const byte numTestCases2[] PROGMEM = {2};
const byte numTestCases4[] PROGMEM = {4};
const byte numTestCases5[] PROGMEM = {5};
const byte numTestCases7[] PROGMEM = {7};
const byte numTestCases8[] PROGMEM = {8};
const byte numTestCases9[] PROGMEM = {9};
const byte numTestCases10[] PROGMEM = {10};
const byte numTestCases12[] PROGMEM = {12};
const byte numTestCases14[] PROGMEM = {14};
const byte numTestCases15[] PROGMEM = {15};
const byte numTestCases18[] PROGMEM = {18};
const byte numTestCases31[] PROGMEM = {31};

// Define an array of IC_TestPatterns, using PROGMEM for the array itself
const struct IC_TestPattern {
  const char* icType;
  const byte* pinCount;
  const byte* numTestCases;
  const char* const* testPatterns;
}

testPatterns[] PROGMEM = {

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

  {reinterpret_cast<const char*>(ic_model_74192), pinCount16, numTestCases7, testPatterns_74192}, //11
  {reinterpret_cast<const char*>(ic_model_7474), pinCount14, numTestCases8, testPatterns_7474}, //12
  {reinterpret_cast<const char*>(ic_model_74190), pinCount16, numTestCases9, testPatterns_74190}, //13
  {reinterpret_cast<const char*>(ic_model_74193), pinCount16, numTestCases31, testPatterns_74193}, //14
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
  {reinterpret_cast<const char*>(ic_model_071), pinCount8, numTestCases2, testPatterns_071}  //35

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

  // Read the test string into a buffer
  char testBuffer[pinCount + 1]; // +1 for null terminator
  for (int i = 0; i < pinCount; i++) {
    testBuffer[i] = pgm_read_byte(test + i);
  }
  testBuffer[pinCount] = '\0'; // Ensure null-termination

  Serial.print("SignalIn : ");
  Serial.println(testBuffer);

  // Phase 1: Pin Configuration
  for (int i = 0; i < pinCount; i++) {
    char c = testBuffer[i];
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

  // Very short delay for stabilization
  delayMicroseconds(10);

  // Phase 2: Special Pin Configurations ('X', '0', '1', 'C')
  for (int i = 0; i < pinCount; i++) {
    char c = testBuffer[i];
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

  // Toggle clock pins to simulate clock signal
  for (int i = 0; i < clkPinCount; i++) {
    digitalWrite(clkPins[i], HIGH);
    delayMicroseconds(100); // Adjust as needed for your IC timing requirements
    digitalWrite(clkPins[i], LOW);
    delayMicroseconds(100);
  }

  delay(10);

  // Phase 3: Verification of Outputs
  Serial.print("Response : ");
  boolean printed = false; // Track if any response is printed
  for (int i = 0; i < pinCount; i++) {
    char expected = testBuffer[i];
    char actual = ' ';
    bool isOutputPin = false;

    switch (expected) {
      case 'H':
        actual = digitalRead(pins[i]) ? 'H' : 'L';
        isOutputPin = true;
        break;
      case 'L':
        actual = digitalRead(pins[i]) ? 'H' : 'L';
        isOutputPin = true;
        break;
      case 'V':
        actual = digitalRead(pins[i]) ? 'V' : 'L';
        isOutputPin = false; // Do not print power pin results
        break;
      case 'G':
        actual = digitalRead(pins[i]) ? 'V' : 'G';
        isOutputPin = false; // Do not print ground pin results
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

  // Reset all pins to input mode
  reset_pin_config(pinCount);
  return result;
}


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
      pins = PINS_IC; // Assuming PINS_IC is used for 20 pin ICs
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
      pins = PINS_IC;
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

  // Display up arrow if not on the first submenu
  if (submenu > 2) {
    lcd.setCursor(15, 0);
    lcd.write(byte(0));
  }

  // Display down arrow if not on the last submenu
  if (submenu < 35) {
    lcd.setCursor(15, 1);
    lcd.write(byte(1));
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
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(F(">Probe"));
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
 * Turns off any light emitting from the LED.
 */
void turnOffBacklight() {
  analogWrite(PIN_RGBLED_R, 0);
  analogWrite(PIN_RGBLED_G, 0);
  analogWrite(PIN_RGBLED_B, 0);
}

/**
 * Sets the color of the backlight.
 */
void setBacklightColor(uint8_t red, uint8_t green, uint8_t blue) {
  analogWrite(PIN_RGBLED_R, red);
  analogWrite(PIN_RGBLED_G, green);
  analogWrite(PIN_RGBLED_B, blue);
}


/**
 * @brief Checks the voltage level at the probe pin and updates the LCD display.
 *
 * Reads the analog value from the probe pin, converts it to a voltage level,
 * and displays whether the voltage is HIGH, LOW, or HIGH-Z based on predefined thresholds.
 */
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

/**
 * @brief Continuously measures and displays the voltage from the probe pin on the LCD.
 * 
 * Reads the analog value from the probe pin (PIN_PROBE), converts it to a voltage level,
 * and displays the voltage on the LCD. The function runs in an infinite loop and keeps updating the
 * voltage reading. The loop can be exited by pressing the cancel button, which will return the user
 * to the probe submenu.
 */
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

    delay(500);

    if (digitalRead(PIN_BTN_CANCEL) == LOW) {
      break;
    }
  }

  menu = 6;
  submenuProbe = 1;
  probe_user_interface();
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
  if (menu == 1 || menu == 2 || menu == 3) {
    // Main menu navigation
    if (flag_button[0]) { // UP button
      flag_button[0] = false; // Reset flag
      if (menu > 1) menu--;
      update_menu();
    }

    if (flag_button[1]) { // DOWN button
      flag_button[1] = false; // Reset flag
      if (menu < 3) menu++;
      update_menu();
    }

    if (flag_button[4]) { // OK button
      flag_button[4] = false; // Reset flag
      if (menu == 1) {
        automatic_user_interface(); // Enter submenu
        menu = 4; // Switch to submenu mode
      } else if (menu == 2) {
        manual_user_interface();
        menu = 7;
      } else if (menu == 3) {
        probe_user_interface(); // Enter Probe submenu
        menu = 6;
      }
    }

    if (flag_button[5]) { // CANCEL button
      flag_button[5] = false; // Reset flag
      // Implement cancel action if needed
    }
  } else if (menu == 7) {
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
  } else if (menu == 6) {
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

/**
 * Arduino main loop function, continuously called after setup.
 */
void loop() {
  buttonDebounce();
  buttonScanner();
  //heartbeatLED();
}
