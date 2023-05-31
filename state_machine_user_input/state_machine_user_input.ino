
#include "DHT.h"
#define DHTPIN 2 
#define DHTTYPE DHT22
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 128 // OLED display height, in pixels
#define OLED_RESET -1     // can set an oled reset pin if desired
Adafruit_SH1107 display = Adafruit_SH1107(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, 1000000, 100000);
DHT temp_sensor(DHTPIN, DHTTYPE);
bool R = 0;
const int reset_pin = 2;
const int temp_pin = A0;
const int display_button = 3; 
float t_left = 20; // time left in hours (random value picked)
float T; // current chamber temperature
enum {start, safe_region, limited_time};
unsigned char state = start;
volatile bool user_input = 0; // normally low

void setup() {
  pinMode(reset_pin,INPUT_PULLUP); // Active low
  pinMode(display_button, INPUT_PULLUP); // Active low
  Serial.begin(9600); // For debugging
  attachInterrupt(digitalPinToInterrupt(3), display_button_isr, FALLING); 
  
}
  // put your setup code here, to run once:
  


void state_machine() {
  if(R = 0){
    state = start;
  }
  else{
    switch (state) {
      case start: {
        if (t_left > 10) {
          state = safe_region;
        }
        else {
          state = limited_time;
        }      
      }
      case safe_region:{ 
        if (t_left < 10) {
          state = limited_time;
        }
      }
      case limited_time:{ // potentially want an alert now not just rely on them pressing the button
      }
          
  }

}
}
void loop(){
  R = digitalRead(reset_pin);
  T  = temp_sensor.readTemperature();
  state_machine();
  if (user_input){ // put this in the interrupt instead?
    display_temp_and_time_left();
  }
}

void display_button_isr() {
  user_input = 1;
}

void display_temp_and_time_left(){ // Does the screen just turn on as soon as it is powered up? Could control power to display via a switch?
  display_setup(); // turns on & sets up display - how long does this take?
  display.setCursor(0,0);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(10);
  display.setTextWrap(false);
  display.cp437(true);
  display.print(T); // this will actually be the temp sensor value
  display.write(248); // degree sign
  display.print(" C");
  delay(3000); // again do this better
  display.clearDisplay();// do the parameters need to be reset?
  display.drawChar(0, 0, char(t_left), SH110X_WHITE, SH110X_BLACK, 10); // maybe want this on RHS, temperature on LHS and marks in plastic to show meaning 
  delay(3000); 
  display.clearDisplay();
  user_input = 0;
  // how do we turn display off?
 
  
}

void display_setup() {
  display.begin(0x3D, true); // Address 0x3D default.
  display.display();
  delay(2000); // is this necessary?
  display.clearDisplay(); // this is to clear the buffer but is this necessary?  
}
