# include <Wire.h>
const int buzzerPin = 8;
const int interruptPin = 2;
volatile bool state = HIGH;

unsigned long time_pressed;

int counter;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), Switch1, CHANGE); //falling means goes from open to closed (HIGH to LOW)

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("working");
  
  //state = digitalRead(interruptPin);
  if(state == LOW){
    Serial.println("about to turn light on");
    if (millis()- time_pressed > 3000){
      Serial.println("turning light on");
      digitalWrite(buzzerPin, HIGH);
    }
    else{
      digitalWrite(buzzerPin, LOW);
    }
  }
  else{
    digitalWrite(buzzerPin, LOW);
  }
  
}

void Switch1(){
  Serial.println("button pressed");
  state = !state;
  time_pressed = millis();
  Serial.println(state);
  
  
}
