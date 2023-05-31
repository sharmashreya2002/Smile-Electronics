#include "DHT.h"
#include <Wire.h>
#include <Arduino.h>
#include <U8g2lib.h>
#define DHTPIN 2 
#define DHTTYPE DHT22

U8G2_SH1107_128X128_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);
DHT temp_sensor(DHTPIN, DHTTYPE);


// User interface connections
// Have interrupt for display and reset
const int reset_pin = 4;
const int display_button = 5;
//volatile bool display_state = 0;
bool reset_activated = 0;
volatile bool reset_state = 0;
volatile bool display_active = 0; // set to 1 if the display is on

// Display state machine
enum DisplayStates {zero, one, two};
DisplayStates displaystate = zero;

// Temperature state machine 
enum tempstates {start, safe_region, limited_time, unsafe};
tempstates state = start; // check this
float time_since_button = 0;

//Parameters
float T = 0;
float t_left = 20;
float t_at_reset = 0;
float t_at_display = 0; 
void setup() {
  pinMode(reset_pin, INPUT);
  pinMode(display_button, INPUT);
  u8g2.begin();
  u8g2.enableUTF8Print(); // allows us to print the degrees symbol
  attachInterrupt(digitalPinToInterrupt(reset_pin), reset_isr, FALLING); 
  attachInterrupt(digitalPinToInterrupt(display_button), display_isr, FALLING);
  Serial.begin(9600);
  //Serial.println("serial connection");
  temp_sensor.begin();

}
void reset_isr(){
  reset_state = 1;
  t_at_reset = millis();
  Serial.println("reset isr called");
}
void display_isr(){
  display_active = 1;
  t_at_display = millis(); // used to track how long screen should now stay on for
  Serial.println("display isr called");
}

void display_temp(float T){
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.clearDisplay();
  u8g2.setFont(u8g2_font_helvB24_tf);
  u8g2.setCursor(31, 100);
  //Serial.print(u8g2.tx); // check cursor position
  u8g2.print(T, 1);
  u8g2.setCursor(95, 100);
  u8g2.print("°C");
  u8g2.sendBuffer(); // send buffer to display!

}

void display_timeleft(float t){
  u8g2.clearDisplay();
  u8g2.setCursor(31, 100);
  u8g2.print(t, 1);
  u8g2.sendBuffer();
  }

void display_state_machine(){
  if (reset_state == 1) {
    displaystate =  zero;
    u8g2.clearDisplay();
    u8g2.clearBuffer();
    
  }
  else{
    switch(displaystate){
      case zero : {
        Serial.println("case zero");
        if (reset_state == 0){
          display_temp(T); // change on transition. Should display to 0.1 deg C (accuracy of sensor)
          displaystate = one;
        }
        break;    
        }
       case one:{
        Serial.println("case one");
        if (time_since_button > 5000){ // 5 second display time, probably longer than needed
          display_timeleft(t_left);
          displaystate = two;
        }
       }
        break;
       case two: {
        Serial.println("case two");
        if (time_since_button > 10000 ){
          u8g2.clearDisplay();
          u8g2.clearBuffer();
          displaystate = zero;
          Serial.println("display state set to zero");
          display_active = 0; // next run through main loop will not have activate screen set to 1
        }
        
       }
      }
       
    }
}




void state_machine() {
  if (reset_state == 1){
    state = start;
  }
  else {
  
    switch (state) {

      case start: {
        Serial.println("start state");
        if (t_left > 40 and T < 8) {
          Serial.println("changing to safe_region");
          state = safe_region;
        }
        else {
          state = limited_time;
          Serial.println("changing to limited time");
        }      
      }
      case safe_region:{ 
        Serial.println("safe region");
        if (T > 8){
          Serial.println("changing to unsafe");
          state = unsafe;
        }
        if (t_left <= 40) {
          state = limited_time;
          //Serial.println("changing to limited time");
        }
        break;
      }
      case limited_time:{
        Serial.println("limited time");
        //if (t_left <= 10){ // probably want to flash the amber led now
        if (T > 8){
          state = unsafe;
          Serial.println("changing to unsafe");
          
        }
        break;
      case unsafe:{
        // do nothing, maybe flash red led
      }
      
      }
          
  }
  
}

}

  

/*void display_T_and_time(float Temp, float time_left, int stage ) { // Takes 3 inputs - temp to display, time to display, whether to show T, t_left or switch off
  Serial.println("Display function called");
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.clearDisplay();
  u8g2.setFont(u8g2_font_logisoso50_tf);
  if (stage == 0){
    u8g2.setCursor(31, 100);
    u8g2.print(Temp);
    u8g2.setCursor(80, 100);
    u8g2.print("°C");
    u8g2.sendBuffer(); }// send buffer to display!
  if (stage == 1) {
    u8g2.clearDisplay();
    u8g2.setCursor(31, 100);
    u8g2.print(time_left);
    u8g2.sendBuffer();}
  if (stage == 2) {
    u8g2.clearDisplay();
    u8g2.clearBuffer();
    display_state = 0; 
  }
  else {
    Serial.println("Invalid stage reached");
  }
  
}
*/



float time_remaining_function(){ 
  int t = millis()/1000;
  if (t < 61) {
    return 60 - t;
  }
  else {
    return 0;   
  }
  

  }
 
  
void loop(){
  delay(1000); // temperature sensor needs a delay between readings
  T = temp_sensor.readTemperature();
  t_left = time_remaining_function(); // find time_remaining
  state_machine();
  if (display_active == 1){
    Serial.println("display state machine called");
    time_since_button = millis() - t_at_display;
    display_state_machine();
     
    }
  if (reset_state == 1){
    Serial.print("reset state at end of main loop: ");
    Serial.println(reset_state);
    if (reset_activated == 0){ // reset ISR was called during this iteration of the main loop
      reset_activated = 1; // 
    }
    else {
      reset_activated = 0; // set back to zero for next reset
      reset_state = 0; // switch reset_state back to zero so we're not stuck in the reset state
    }
    
  } 
 }
     
 

 
     
