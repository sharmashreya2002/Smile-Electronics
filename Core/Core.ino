
#include <Arduino.h>
#include "labels.h"

void display_isr();

void display_temp(float T);

void display_timeleft(float t);

void display_state_machine();

float find_HLR(float Tmeasured);

float find_Qaccum(float heat_loss_rate);

float find_Q_left(float Qaccum, float Qleft);

float remaining_cool_life(float Qleft, float Qaccum, float heat_loss_rate);

float time_remaining_function();

void state_machine();




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
}


void loop(){
  delay(1000); // temperature sensor needs a delay between readings
  T = temp_sensor.readTemperature();
  time_remaining_function(); // find time_remaining
  /*Serial.print("value of t_left is: ");
  Serial.print(t_left);*/
  state_machine();
 

  if (display_active == 1){
    //Serial.println("display state machine called");
    time_since_button = millis() - t_at_display;
    display_state_machine();
   
    }

 }
    
