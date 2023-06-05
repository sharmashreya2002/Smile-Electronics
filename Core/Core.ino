//All
/* 
 *  This file contains the protoypes for all of the functions used, as well as the setup() and loop() functions. The person responsible for the code is shown at the top of each file.
 */
#include <Arduino.h>
#include "labels.h"

// List of function prototypes  - this is included to avoid compilation errors

void display_isr();

void display_temp(float T);

void display_timeleft(float t);

void display_state_machine();

float find_HLR(float Tmeasured);

float find_Qaccum(float heat_loss_rate);

float find_Q_left(float Qaccum, float Qleft);

float remaining_cool_life(float Qleft, float Qaccum, float heat_loss_rate);

float time_remaining_function();

float find_chamber_temp(float heat_loss_rate);

void state_machine();

void Switch1();

void timer_buzzer();


void setup() {

  // Display setup
  pinMode(display_button, INPUT); 
  u8g2.begin(); //
  u8g2.enableUTF8Print(); // allows us to print the degrees symbol to the display
  attachInterrupt(digitalPinToInterrupt(display_button), display_isr, RISING);
  
  Serial.begin(9600); // To enable debugging
  
  // First iteration of the algorithm: -  initialise with start values
  temp_sensor.begin();
  T = temp_sensor.readTemperature();
  heat_loss_rate = find_HLR(T);
  Qaccum = find_Qaccum(heat_loss_rate);
  Qleft = find_Q_left(Qaccum, Q_total);
  t_left = remaining_cool_life(Qleft, Qaccum, heat_loss_rate);
  error_margin = 0.05*t_left;

  // Set up door switch
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), Switch1, CHANGE); 
}


void loop(){
  delay(1000); // temperature sensor needs a delay between readings
  T = temp_sensor.readTemperature(); // Read the temperature sensor
  time_remaining_function(); // find time_remaining

  state_machine(); // Iterate the temperature state machine
 

  if (display_active == 1){ // Display button was pressed within the last ten seconds
    time_since_button = millis() - t_at_display;
    display_state_machine(); // Iterate display state machine
   
    }
  // Check door
  if(state_door == LOW){  
    timer_buzzer();

 }
}
    
