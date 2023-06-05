//May
 #ifndef _labels_H
#define _labels_H

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
float error_margin;
float Rth_2 = 2;

// User interface connections
const int display_button = 5; 
volatile bool display_active = 0; // This is set to 1 if the display button is pushed. It is then reset to zero ten seconds later

// Display state machine
enum DisplayStates {zero, one, two}; // Define the possible display states 
DisplayStates displaystate = zero; // Initialise at start state 

// Temperature state machine
enum tempstates {start, safe_region, limited_time, unsafe}; // Define the possible temperature states
tempstates state = start; // Initialise at the start state
float time_since_button = 0; // This holds the time since the display button was pressed

//Parameters
float T = 0; // Temperature read from the DHT22
float t_left = 20; // Remaining cool life in hours, initialised at an arbitrary value
float t_at_display = 0; // time at which the display button was pressed. This is reset each time the button is pressed, and is used to calculate the time elapsed since the press


// Setup temperature sensor & display
U8G2_SH1107_128X128_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 8);
DHT temp_sensor(DHTPIN, DHTTYPE);

// Door switch
const int buzzerPin = 8;
const int interruptPin = 3;
volatile bool state_door = LOW;
const int check_pin = A0;
unsigned long time_pressed; 
int state_of_switch;

#endif
