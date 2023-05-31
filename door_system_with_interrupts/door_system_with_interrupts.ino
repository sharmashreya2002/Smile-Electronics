# include <Wire.h>
const int buzzerPin = 8;
const int interruptPin = 3;
volatile bool state_door = LOW;
const int check_pin = A0;

unsigned long time_pressed;

int state_of_switch;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), Switch1, CHANGE); 

}

void loop() {
  // put your main code here, to run repeatedly:

  if(state_door == LOW){
    
    timer_buzzer();
  }
  
}

void Switch1(){
  
  //state_door = !state_door;
  //state_door = digitalRead(interruptPin);
  time_pressed = millis();
  Serial.println("state door : " + String(state_door));
  digitalWrite(buzzerPin, LOW);
  state_of_switch = analogRead(check_pin);
  Serial.println(state_of_switch);
  if(state_of_switch > 30){
    state_door = 1;
  }
  else{
    state_door = 0;
  }
  
  
}

void timer_buzzer(){
  if (millis()- time_pressed > 3000){
      digitalWrite(buzzerPin, HIGH);
    }

}
