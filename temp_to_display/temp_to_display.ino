#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 128 // OLED display height, in pixels
#define OLED_RESET -1     // can set an oled reset pin if desired
Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);

void setup() {
  Serial.begin(9600);
  delay(250);
  display.begin(0x3D, true); // Address 0x3D default
  display.display();
  delay(2000); 
  display.clearDisplay(); // this is to clear the buffer but is this necessary?
  
}
void write_temp(){
  display.setCursor(0,0);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(10);
  display.setTextWrap(false);
  display.cp437(true);
  display.print("a number "); // this will actually be the temp sensor value
  display.write(248); // degree sign
  display.print(" C");

 

}
void loop() {
  write_temp();
}
