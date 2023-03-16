/*
 * Viper7RCMaster.ino 3/12 0
 *
 * Sketch by Viper7Gamer
 * For using Xbox Series X Controller
 * 
 * Use the Left Stick for Steering
 * Use Right Trigger for Throttle
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








void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXBOX ONE USB Library Started"));
  Serial.println();

  // Set up the LCD's number of columns and rows and display static text
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("ST: 0           ");
  lcd.setCursor(0, 1);
  lcd.print("TT: 0           ");


lcd.clear()
# Set LCD color to red
lcd.color = [100, 0, 0]
time.sleep(1)
# Print two line message
lcd.message = "Hello\nCircuitPython"
# Wait 5s
time.sleep(5)
# Set LCD color to blue
lcd.color = [0, 100, 0]
time.sleep(1)
# Set LCD color to green
lcd.color = [0, 0, 100]
time.sleep(1)
# Set LCD color to purple
lcd.color = [50, 0, 50]
time.sleep(1)
lcd.clear()
# Print two line message right to left
lcd.text_direction = lcd.RIGHT_TO_LEFT
lcd.message = "Hello\nCircuitPython"
# Wait 5s
time.sleep(5)
# Return text direction to left to right
lcd.text_direction = lcd.LEFT_TO_RIGHT
# Display cursor
lcd.clear()
lcd.cursor = True
lcd.message = "Cursor! "
# Wait 5s
time.sleep(5)
# Display blinking cursor
lcd.clear()
lcd.blink = True
lcd.message = "Blinky Cursor!"
# Wait 5s
time.sleep(5)
lcd.blink = False
lcd.clear()
# Create message to scroll
scroll_msg = "<-- Scroll"
lcd.message = scroll_msg
# Scroll to the left
for i in range(len(scroll_msg)):
 time.sleep(0.5)
 lcd.move_left()
lcd.clear()
time.sleep(1)
lcd.message = "Going to sleep\nCya later!"
time.sleep(5)
# Turn off LCD backlights and clear text
lcd.color = [0, 0, 0]
lcd.clear()
  
  
  

}



void loop() {
  Usb.Task();
  if (Xbox.XboxOneConnected) {
    if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500 || Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500 || Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500 || Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
      if (Xbox.getAnalogHat(LeftHatX) > 7500 || Xbox.getAnalogHat(LeftHatX) < -7500) {
        Serial.print(F("LeftHatX: "));
        Serial.print(Xbox.getAnalogHat(LeftHatX));
        Serial.print("\t");
        nSVal = Xbox.getAnalogHat(LeftHatX);
        ShowSteering();
      }
      if (Xbox.getAnalogHat(LeftHatY) > 7500 || Xbox.getAnalogHat(LeftHatY) < -7500) {
        Serial.print(F("LeftHatY: "));
        Serial.print(Xbox.getAnalogHat(LeftHatY));
        Serial.print("\t");
      }
      if (Xbox.getAnalogHat(RightHatX) > 7500 || Xbox.getAnalogHat(RightHatX) < -7500) {
        Serial.print(F("RightHatX: "));
        Serial.print(Xbox.getAnalogHat(RightHatX));
        Serial.print("\t");
      }
      if (Xbox.getAnalogHat(RightHatY) > 7500 || Xbox.getAnalogHat(RightHatY) < -7500) {
        Serial.print(F("RightHatY: "));
        Serial.print(Xbox.getAnalogHat(RightHatY));
        nSVal=Xbox.getAnalogHat(RightHatY);
        ShowThrottle();       
      }
      Serial.println();
    }

    // Get RT for Throttle
    // Get LT for Brake/Reverse
    if (Xbox.getButtonPress(LT) > 0 || Xbox.getButtonPress(RT) > 0) {
      if (Xbox.getButtonPress(LT) > 0) {
        Serial.print(F("LT: "));
        Serial.print(Xbox.getButtonPress(LT));
        Serial.print("\t");
      }
      if (Xbox.getButtonPress(RT) > 0) {
        Serial.print(F("RT: "));
        Serial.print(Xbox.getButtonPress(RT));
        Serial.print("\t");
        nTVal = Xbox.getButtonPress(RT);
        ShowThrottle();
      }
      Serial.println();
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
      Serial.println(F("Throttle Trim Up"));
      throttleTrim = throttleTrim + 1;
      throttleTrimRefreshLCD();
    }
    
    // Dpad Down - Adjust Throttle Trim Down   
    if (Xbox.getButtonClick(DOWN)) {
      Serial.println(F("Throttle Trim Down"));
      throttleTrim = throttleTrim - 1;
      throttleTrimRefreshLCD();
    }
    
    // Dpad Left - Adjust Steering Trim Left   
    if (Xbox.getButtonClick(LEFT)) {
      Serial.println(F("Steering Trim Left"));
      steeringTrim = steeringTrim - 1;
      steeringTrimRefreshLCD();
    }
    
   // Adjust Steering Trim Right   
    if (Xbox.getButtonClick(RIGHT)) {
      Serial.println(F("Steering Trim Right"));
      steeringTrim = steeringTrim + 1;
      steeringTrimRefreshLCD();
    }
    
    // Throttle Trim Reset
    if (Xbox.getButtonClick(START)) {
      Serial.println(F("Throttle Trim Reset"));
      throttleTrim = 0;
      throttleTrimRefreshLCD();
    }
    
    // Steering Trim Reset
    if (Xbox.getButtonClick(BACK)) {
      Serial.println(F("Steering Trim Reset"));
      steeringTrim = 0;
      steeringTrimRefreshLCD();
    }
    
    if (Xbox.getButtonClick(XBOX))
      Serial.println(F("Xbox"));
    if (Xbox.getButtonClick(SYNC))
      Serial.println(F("Sync"));
    if (Xbox.getButtonClick(SHARE))
      Serial.println(F("Share"));

    if (Xbox.getButtonClick(LB))
      Serial.println(F("LB"));
    if (Xbox.getButtonClick(RB))
      Serial.println(F("RB"));
    if (Xbox.getButtonClick(LT))
      Serial.println(F("LT"));
    if (Xbox.getButtonClick(RT))
      Serial.println(F("RT"));
    if (Xbox.getButtonClick(L3))
      Serial.println(F("L3"));
    if (Xbox.getButtonClick(R3))
      Serial.println(F("R3"));


    if (Xbox.getButtonClick(A)) {
      Serial.println(F("A"));
      nTVal = Xbox.getButtonPress(RT);
      ShowThrottle();

      nSVal = Xbox.getAnalogHat(LeftHatX);
      ShowSteering();
    }
   
    if (Xbox.getButtonClick(B))
      Serial.println(F("B"));
    if (Xbox.getButtonClick(X))
      Serial.println(F("X"));
    if (Xbox.getButtonClick(Y))
      Serial.println(F("Y"));
  }
  delay(1);
}

// Function to update steering trim setting information on LCD
// Converts steeringTrimLCD from negative and positive numbers to L and R
void steeringTrimRefreshLCD() {
  
  lcd.setBacklight(GREEN);
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
  lcd.setBacklight(RED);
  
  throttleTrimLCD = throttleTrim;
  throttleTrimActual = throttleTrim * throttleTrimIncrement;
  
  lcd.setCursor(4, 1);
  lcd.print(String(throttleTrimLCD) + "   ");
}

void ShowSteering() {

  lcd.setCursor(9, 0);
  lcd.print(String(nSVal) + "        ");

}

void ShowThrottle() {

  lcd.setCursor(9, 1);
  lcd.print(String(nTVal) + "        ");

}
