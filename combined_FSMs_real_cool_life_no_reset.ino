#include "DHT.h"
#include <Wire.h>
#include <Arduino.h>
#include <U8g2lib.h>
#define DHTPIN 2
#define DHTTYPE DHT22

// Got rid of resets for the individual state machines - will just use arduino reset button

// Fixed parameters for the remaining cool life algorithm
float Rth = 9.715; // Overall thermal resistance
float area = 0.179; // Area over which the heat is lost, relevant to flux calculations (from prev.)
float Q_total = 868400; // Energy needed to melt ice in ice pack (from prev.)
int t = 5 * 60;// Time interval between temperature measurements (convert from minutes to seconds)
// Dynamic variables for remaning cool life algorithm
//float T = 24; // change to temperature measurement
float Qaccum;
float Qleft;
float heat_loss_rate;
//float cool_life;

// Door switch parameters
const int buzzerPin = 8;
const int interruptPin = 3;
volatile bool state_door = LOW;
unsigned long time_pressed;


// User interface connections

//const int reset_pin = 4;
const int display_button = 5;
//volatile bool display_state = 0;
//bool reset_activated = 0;
//volatile bool reset_state = 0;
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
//float t_at_reset = 0;
float t_at_display = 0;


// Setup temp sensor & display
U8G2_SH1107_128X128_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);
DHT temp_sensor(DHTPIN, DHTTYPE);







// Calculate instantaneous heat loss rate (W)
float find_HLR(float Tmeasured) {
  float heat_loss_rate = T/ Rth;
  return heat_loss_rate;
}

// Find heat loss in the time interval, for a given heat loss rate
float find_Qaccum(float heat_loss_rate) {

  // Heat lost from ice pack in this time interval
  float Qaccum = heat_loss_rate * t;
  return Qaccum;
}

float find_Q_left(float Qaccum, float Qleft) {
  // Q left for first iteration is Q total, every iteration following this has less
  return Qleft - Qaccum;
}

// Calculate remaining cool life in hours
float remaining_cool_life(float Qleft, float Qaccum, float heat_loss_rate) {
  // Predict remaining cool life, assuming that the instantaneous heat flux will remain constant until the end
  return (Qleft - Qaccum) / (heat_loss_rate * 3600);
}

void setup() {
  //pinMode(reset_pin, INPUT);
  pinMode(display_button, INPUT);
  u8g2.begin();
  u8g2.enableUTF8Print(); // allows us to print the degrees symbol
 // attachInterrupt(digitalPinToInterrupt(reset_pin), reset_isr, FALLING); 
  attachInterrupt(digitalPinToInterrupt(display_button), display_isr, RISING);
  Serial.begin(9600);
  //Serial.println("serial connection");
  temp_sensor.begin();
  T = temp_sensor.readTemperature();

    // First iteration of the algorithm: -  initialise with start values

  heat_loss_rate = find_HLR(T);
  Qaccum = find_Qaccum(heat_loss_rate);
  Qleft = find_Q_left(Qaccum, Q_total);
  t_left = remaining_cool_life(Qleft, Qaccum, heat_loss_rate);
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), Switch1, FALLING); //falling means goes from open to closed (HIGH to LOW)


}

float time_remaining_function() {


  // Every iteration after the first will follow the following format until no more time is left
  // Print statements for debugging purposes
  heat_loss_rate = find_HLR(T);
  Qaccum = find_Qaccum(heat_loss_rate);
  Qleft = find_Q_left(Qaccum, Qleft);
  t_left = remaining_cool_life(Qleft, Qaccum, heat_loss_rate);
  //Serial.println(heat_loss_rate, 6);
  //Serial.println(Qaccum);
  /*Serial.print("Qleft: ");
  Serial.println(Qleft);
  Serial.print("Cool Life: ");
  Serial.println(t_left);*/


}

/*void reset_isr(){
  reset_state = 1;
  t_at_reset = millis();
  Serial.println("reset isr called");
}*/

void Switch1(){
  Serial.println("button pressed");
  state_door = 0;
  time_pressed = millis();
  Serial.println(state_door);
  
  
}

void display_isr(){
  display_active = 1;
  t_at_display = millis(); // used to track how long screen should now stay on for
  //Serial.println("display isr called");
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
  /*if (reset_state == 1) {
    displaystate =  zero;
    u8g2.clearDisplay();
    u8g2.clearBuffer();
    
  }
  else{*/
    switch(displaystate){
      case zero : {
        //Serial.println("case zero");
          display_temp(T); // change on transition. Should display to 0.1 deg C (accuracy of sensor)
          displaystate = one;
        
        break;    
        }
       case one:{
        //Serial.println("case one");
        if (time_since_button > 5000){ // 5 second display time, probably longer than needed
          display_timeleft(t_left);
          displaystate = two;
        }
       }
        break;
       case two: {
        //Serial.println("case two");
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





void state_machine() {
 /* if (reset_state == 1){
    state = start;
  }
  else {*/
  
    switch (state) {

      case start: {
       // Serial.println("start state");
        if (t_left > 40 and T < 80) {
          //Serial.println("changing to safe_region");
          state = safe_region;
        }
        else {
          state = limited_time;
          //Serial.println("changing to limited time");
        }      
      }
      case safe_region:{ 
        //Serial.println("safe region");
        if (T > 80){
         // Serial.println("changing to unsafe");
          state = unsafe;
        }
        if (t_left <= 40) {
          state = limited_time;
          //Serial.println("changing to limited time");
        }
        break;
      }
      case limited_time:{
        //Serial.println("limited time");
        //if (t_left <= 10){ // probably want to flash the amber led now
        if (T > 8){
          state = unsafe;
        //  Serial.println("changing to unsafe");
          
        }
        break;
      case unsafe:{
        // do nothing, maybe flash red led
      }
      
      }
          
  }
  
}

void timer_buzzer(){
  if (digitalRead(buzzerPin) == 1){ 
      if (millis()- time_pressed > 10000){
          Serial.println(millis()-time_pressed);

          digitalWrite(buzzerPin, HIGH);
      
         }
        else{
            // Serial.println("started counting");
             digitalWrite(buzzerPin, LOW);
         }}
   else {
   Serial.println("setting door state to 1");
    state_door = 1;
   }
   
}






void loop(){
  delay(1000); // temperature sensor needs a delay between readings
  T = temp_sensor.readTemperature();
  time_remaining_function(); // find time_remaining
  /*Serial.print("value of t_left is: ");
  Serial.print(t_left);*/
  state_machine();
  
  Serial.print("state door: ");
  Serial.println(state_door);
  if(state_door == LOW){
    
    
    timer_buzzer();
  }
  else{
    Serial.println("switching buzzer off");
    digitalWrite(buzzerPin, LOW);
  }
  if (display_active == 1){
    //Serial.println("display state machine called");
    time_since_button = millis() - t_at_display;
    display_state_machine();
   
    }
  /*if (reset_state == 1){
    //Serial.print("reset state at end of main loop: ");
    //Serial.println(reset_state);
    if (reset_activated == 0){ // reset ISR was called during this iteration of the main loop
      reset_activated = 1; // 
    }
    else {
      reset_activated = 0; // set back to zero for next reset
      reset_state = 0; // switch reset_state back to zero so we're not stuck in the reset state
    }
    
  }*/
 }
    
