# include <Wire.h>

// where the buzzer is placed
const int buzzerPin = 8;
const int switchPin = 7;

// time before buzzer goes off once switch is closed

const long interval = 2000;

//set up timer variable
//int counter;

// set up a variable for a counter

int timer;
unsigned long time_pressed;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  // initialise timer to 0
  timer = 0;
  time_pressed = 0;

}

void loop() {
  // put your main code here, to run repeatedly:
  int state;

  state = digitalRead(switchPin);
  
  if (state == LOW){
    digitalWrite(buzzerPin, LOW);

    // update time_pressed 
    time_pressed = millis();
    Serial.println(String(time_pressed));
  }else{
    // this still has an issue with overflow
    if (millis() - time_pressed > 3000){
      digitalWrite(buzzerPin, HIGH);
    }
    Serial.println( String(millis() - time_pressed));
  }

  
  
  delay(100);
  

}
