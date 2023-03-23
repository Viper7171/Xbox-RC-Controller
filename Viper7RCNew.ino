/*
 * Viper7RCNew.ino 3/17
 *
 * Sketch by Viper7Gamer
 * For using Xbox Series X Controller
 * 
 * Use the Left Stick for Steering
 * Use Right Trigger for Throttle
 * Use Left Trigger for Reverse
 * 
 * Dpad Left, Dpad Right to set Steering Trim
 * Dpad Up, Dpad Down to set Throttle Trim
 * 
 * Back/Select button Reset Steering Trim
 * Start button Reset Throttle Trim
 * 
 * 
 * Using:
 *  Sketch by David Simpson
 *  AssistiveTechStuff/AdaptiveRCController
 * 
 *  Example sketch for the Xbox ONE USB library - by guruthree, based on work by
 *  Kristian Lauszus.
 *
 *
 * PPM Output is on Pin 3 
 * 
 * Download USB Host Shield Library First 
 * https://github.com/felis/USB_Host_Shield_2.0/archive/master.zip
 * 
 * Channel Info Currently Setup AS AETR you can change this by changing the PPM Value in the main loop
 * 
 * Ch1 A (Steering) ==  ppm[0]
 * Ch2 E (Throttle) ==  ppm[1]
 * Ch3 T (NOT USED) ==  ppm[2]
 * Ch4 R (NOT USED) ==  ppm[3]
 * Ch5 AUX1 (NOT USED) == ppm[4]
 * Ch6 AUX2 (NOT USED) == ppm[5]
 * Ch7 AUX3 (NOT USED) == ppm[6]
 * Ch8 AUX4 (NOT USED) == ppm[7]
 *
 */

#include <XBOXONE.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXONE Xbox(&Usb);

// ------------------------ V7G
// Includes and Defines for Adafruit RGB LCD Shield
// https://github.com/adafruit/Adafruit-RGB-LCD-Shield-Library/archive/refs/heads/master.zip
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
// Define RGB LCD Colours
#define RED 0x1
#define GREEN 0x2

// Setup some variables
int steeringTrim = 0;
int steeringTrimLCD = 0;
int steeringTrimActual = 0;
int steeringTrimIncrement = 100;

int throttleTrim = 0;
int throttleTrimLCD = 0;
int throttleTrimActual = 0;
int throttleTrimIncrement = 50;

int speedSetting = 1;
int speedMax = 2000;
int speedMin = 1000;

int nSVal = 0;
int nTVal = 0;

////////////// PPM 
/*
 * PPM generator originally written by David Hasko
 * on https://code.google.com/p/generate-ppm-signal/ 
 */

//////////////////////CONFIGURATION///////////////////////////////
#define CHANNEL_NUMBER 8  //set the number of chanels
#define CHANNEL_DEFAULT_VALUE 1500  //set the default servo value
#define FRAME_LENGTH 22500  //set the PPM frame length in microseconds (1ms = 1000µs)
#define PULSE_LENGTH 300  //set the pulse length
#define onState 1  //set polarity of the pulses: 1 is positive, 0 is negative
#define sigPin 3  //set PPM signal output pin on the arduino
//////////////////////////////////////////////////////////////////

#define SWITCH_PIN 16
#define CHANNEL_TO_MODIFY 11
#define SWITCH_STEP 100

byte previousSwitchValue;

/*this array holds the servo values for the ppm signal
 change these values in your code (usually servo values move between 1000 and 2000)*/
int ppm[CHANNEL_NUMBER];

int currentChannelStep;
bool printAngle;
uint8_t state = 0;
/////////////






void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
//    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
//  Serial.print(F("\r\nXBOX ONE USB Library Started"));
//  Serial.println();

 
 ///////////////////////////////
  previousSwitchValue = HIGH;
  
  //initiallize default ppm values
  for(int i=0; i<CHANNEL_NUMBER; i++){
    if (i == 2 || i == CHANNEL_TO_MODIFY) {
      ppm[i] = 1000;
    } else {
      ppm[i]= CHANNEL_DEFAULT_VALUE;
    }
  }
  
  pinMode(sigPin, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  digitalWrite(sigPin, !onState);  //set the PPM signal pin to the default state (off)

 
  // Set up the LCD's number of columns and rows and display static text
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("ST: 0           ");
  lcd.setCursor(0, 1);
  lcd.print("TT: 0           ");


  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;
  
  OCR1A = 100;  // compare match register, change this
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();

  currentChannelStep = SWITCH_STEP; 
  
  
}


void loop() {
  Usb.Task();
  if (Xbox.XboxOneConnected) {
    if (Xbox.getAnalogHat(LeftHatX) > 2500 || Xbox.getAnalogHat(LeftHatX) < -2500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
      if (Xbox.getAnalogHat(LeftHatX) > 2500 || Xbox.getAnalogHat(LeftHatX) < -2500) {
        ppm[0] = map(Xbox.getAnalogHat(LeftHatX), -32768 , 32768, 1000, 2000) + steeringTrimActual;
      } else {
        ppm[0] = 1500 + steeringTrimActual;
         
        //Serial.print(F("LeftHatX: "));
        //Serial.print(Xbox.getAnalogHat(LeftHatX));
        //Serial.print("\t");
        //nSVal = Xbox.getAnalogHat(LeftHatX);
        //ShowSteering();
      }
//      if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
//        Serial.print(F("LeftHatY: "));
//        Serial.print(Xbox.getAnalogHat(LeftHatY));
//        Serial.print("\t");
//      }
//      if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
//        Serial.print(F("RightHatX: "));
//        Serial.print(Xbox.getAnalogHat(RightHatX));
//        Serial.print("\t");
//      }
//      if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
//        Serial.print(F("RightHatY: "));
//        Serial.print(Xbox.getAnalogHat(RightHatY));
//        nSVal=Xbox.getAnalogHat(RightHatY);
//        ShowThrottle();       
//      }
//      Serial.println();
    }

    // Get RT for Throttle
    // Get LT for Brake/Reverse
    if (Xbox.getButtonPress(LT) > 0 || Xbox.getButtonPress(RT) > 0) {
      if (Xbox.getButtonPress(LT) > 0) {
         ppm[1] = map(Xbox.getButtonPress(LT), 0 , 1023, 1000, 1500);
      } else {
        ppm[1] = 1500;
          
        
        //Serial.print(F("LT: "));
        //Serial.print(Xbox.getButtonPress(LT));
        //Serial.print("\t");
        //nTVal = Xbox.getButtonPress(LT) * (-1);
        //ShowThrottle();
      }
      if (Xbox.getButtonPress(RT) > 0) {
         ppm[1] = map(Xbox.getButtonPress(RT), 0 , 1023, 1500, 2000);
      } else {
        ppm[1] = 1500;
          
        //Serial.print(F("RT: "));
        //Serial.print(Xbox.getButtonPress(RT));
        //Serial.print("\t");
        //nTVal = Xbox.getButtonPress(RT);
        //ShowThrottle();
      }
//      Serial.println();
    }

   // V7G Commented out the Trigger Rumble 
   // Set rumble effect 
   // static uint16_t oldLTValue, oldRTValue;
   // if (Xbox.getButtonPress(LT) != oldLTValue || Xbox.getButtonPress(RT) != oldRTValue) {
   //   oldLTValue = Xbox.getButtonPress(LT);
   //   oldRTValue = Xbox.getButtonPress(RT);
   //   uint8_t leftRumble = map(oldLTValue, 0, 1023, 0, 255); // Map the trigger values into a byte
   //   uint8_t rightRumble = map(oldRTValue, 0, 1023, 0, 255);
   //   if (leftRumble > 0 || rightRumble > 0)
   //     // I turned the Trigger Rumble off
   //     Xbox.setRumbleOff();
   //     //Xbox.setRumbleOn(leftRumble, rightRumble, leftRumble, rightRumble);
   //   else
   //     Xbox.setRumbleOff();
   // }

    // Dpad Up - Adjust Throttle Trim Up
    if (Xbox.getButtonClick(UP)) {
//      Serial.println(F("Throttle Trim Up"));
      throttleTrim = throttleTrim + 1;
      throttleTrimRefreshLCD();
    }
    
    // Dpad Down - Adjust Throttle Trim Down   
    if (Xbox.getButtonClick(DOWN)) {
//      Serial.println(F("Throttle Trim Down"));
      throttleTrim = throttleTrim - 1;
      throttleTrimRefreshLCD();
    }
    
    // Dpad Left - Adjust Steering Trim Left   
    if (Xbox.getButtonClick(LEFT)) {
//      Serial.println(F("Steering Trim Left"));
      steeringTrim = steeringTrim - 1;
      steeringTrimRefreshLCD();
    }
    
   // Adjust Steering Trim Right   
    if (Xbox.getButtonClick(RIGHT)) {
//      Serial.println(F("Steering Trim Right"));
      steeringTrim = steeringTrim + 1;
      steeringTrimRefreshLCD();
    }
    
    // Throttle Trim Reset
    if (Xbox.getButtonClick(START)) {
//      Serial.println(F("Throttle Trim Reset"));
      throttleTrim = 0;
      throttleTrimRefreshLCD();
    }
    
    // Steering Trim Reset
    if (Xbox.getButtonClick(BACK)) {
//      Serial.println(F("Steering Trim Reset"));
      steeringTrim = 0;
      steeringTrimRefreshLCD();
    }
    
//    if (Xbox.getButtonClick(XBOX))
//      Serial.println(F("Xbox"));
//    if (Xbox.getButtonClick(SYNC))
//      Serial.println(F("Sync"));
//    if (Xbox.getButtonClick(SHARE))
//      Serial.println(F("Share"));

//    if (Xbox.getButtonClick(LB))
//      Serial.println(F("LB"));
//    if (Xbox.getButtonClick(RB))
//      Serial.println(F("RB"));
//    if (Xbox.getButtonClick(LT))
//      Serial.println(F("LT"));
//    if (Xbox.getButtonClick(RT))
//      Serial.println(F("RT"));
//    if (Xbox.getButtonClick(L3))
//      Serial.println(F("L3"));
//    if (Xbox.getButtonClick(R3))
//      Serial.println(F("R3"));

//    if (Xbox.getButtonClick(A)) {
//      Serial.println(F("A"));
//      nTVal = Xbox.getButtonPress(RT);
//      ShowThrottle();
//
//      nSVal = Xbox.getAnalogHat(LeftHatX);
//      ShowSteering();
//    }
   
//    if (Xbox.getButtonClick(B))
//      Serial.println(F("B"));
//    if (Xbox.getButtonClick(X))
//      Serial.println(F("X"));
//    if (Xbox.getButtonClick(Y))
//      Serial.println(F("Y"));
  }
 
  if (Xbox.XboxOneConnected == false){
    //lcd.setBacklight(RED);
    ppm[1] = 1500; // if controller is disconnected for some reason, throttle gets zeroed
  }
    
   delay(1);
} 
// End Loop


ISR(TIMER1_COMPA_vect){  //leave this alone
  static boolean state = true;
  
  TCNT1 = 0;
  
  if (state) {  //start pulse
    digitalWrite(sigPin, onState);
    OCR1A = PULSE_LENGTH * 2;
    state = false;
  } else{  //end pulse and calculate when to start the next pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;
  
    digitalWrite(sigPin, !onState);
    state = true;

    if(cur_chan_numb >= CHANNEL_NUMBER){
      cur_chan_numb = 0;
      calc_rest = calc_rest + PULSE_LENGTH;// 
      OCR1A = (FRAME_LENGTH - calc_rest) * 2;
      calc_rest = 0;
    }
    else{
      OCR1A = (ppm[cur_chan_numb] - PULSE_LENGTH) * 2;
      calc_rest = calc_rest + ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}

// Function to update steering trim setting information on LCD
// Converts steeringTrimLCD from negative and positive numbers to L and R
void steeringTrimRefreshLCD() {
  
 // lcd.setBacklight(GREEN);
  steeringTrimLCD = steeringTrim; //= map(steeringTrim, -100, 100, -20, 20); // Remap steering trim adjustment to single increments

  steeringTrimActual = steeringTrim * steeringTrimIncrement;

  if (steeringTrimLCD < 0) {
    steeringTrimLCD = abs(steeringTrimLCD);
    lcd.setCursor(4, 0);
    lcd.print("L" + String(steeringTrimLCD) + "   ");    
  } else if (steeringTrimLCD > 0) {
    lcd.setCursor(4, 0);
    lcd.print("R" + String(steeringTrimLCD) + "   ");
  } else {
    lcd.setCursor(4, 0);
    lcd.print("" + String(steeringTrimLCD) + "   ");
  }
}
void throttleTrimRefreshLCD() {
  //lcd.setBacklight(RED);
  
  throttleTrimLCD = throttleTrim;
  throttleTrimActual = throttleTrim * throttleTrimIncrement;
  
  lcd.setCursor(4, 1);
  lcd.print(String(throttleTrimLCD) + "   ");
}

//void ShowSteering() {
//
//  lcd.setCursor(9, 0);
//  lcd.print(String(nSVal) + "        ");
//
//}
//
//void ShowThrottle() {
//
//  lcd.setCursor(9, 1);
//  lcd.print(String(nTVal) + "        ");
//
//}
