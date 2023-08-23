/*
 * This is the code for my soundbag power monitor.
 */


#include <INA226_asukiaaa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "battery.h"

const uint16_t ina226calib = INA226_asukiaaa::calcCalibByResistorMilliOhm(100); // Max 5120 milli ohm
// const uint16_t ina226calib = INA226_asukiaaa::calcCalibByResistorMicroOhm(2000);
INA226_asukiaaa voltCurrMeter(INA226_ASUKIAAA_ADDR_A0_GND_A1_GND, ina226calib);
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    30

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
 Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
//  display.setCursor(0, 10);
//  display.display(); 
  if (voltCurrMeter.begin() != 0) {
    Serial.println("Failed to begin INA226");
  }
}

void loop() {
  int16_t ma, mv, mw;
  float volts = 0.0;
  display.clearDisplay();
  if (voltCurrMeter.readMV(&mv) == 0) {
    volts = (float)mv / 1000;
    display.println(String(volts) + "V");
  } else {
    display.println("Cannot read voltage.");
  }
/*
  if (voltCurrMeter.readMW(&mw) == 0) {
    //volts = (float)mv / 1000;
    display.println(String(-mw) + "mW");
  } else {
    display.println("Cannot read watts.");
  }
*/
  if (voltCurrMeter.readMA(&ma) == 0) {
    //volts = (float)mv / 1000;
    display.println(String(-ma) + "mA");
  } else {
    display.println("Cannot read amps");
  }
 

  if(volts <= 7.64)//7.64 is 50 percent.
  {
    //if the battery is less than 50%
    if(volts <= 6.60)//fully flat is 6.62v
    {
       display.println("0% WARNING");
         display.drawBitmap(98, 0, battery_0pc, LOGO_WIDTH, LOGO_HEIGHT, 1);
    }
    else
    {
      if (volts <= 7.48)
      {
       //  display.println("20%");
           display.drawBitmap(98, 0, battery_25pc, LOGO_WIDTH, LOGO_HEIGHT, 1);
      }
      else
      {
       //  display.println("50%");
         display.drawBitmap(98, 0, battery_50pc, LOGO_WIDTH, LOGO_HEIGHT, 1);
      }
    }
  }
  else
  {
    //The battery is greater than 50%
    if(volts >= 8.1)//fully charged is 8.4v
    {
     //  display.println("100%");
       display.drawBitmap(98, 0, battery_100pc, LOGO_WIDTH, LOGO_HEIGHT, 1);
    }
    else
    {
    //   display.println("75%");
         display.drawBitmap(98, 0, battery_75pc, LOGO_WIDTH, LOGO_HEIGHT, 1);
    }
  }
    
 //   display.println("voltage:" +String(mw) + "mW");
    display.setCursor(0,0);
  display.display();
  delay(1000);
}
