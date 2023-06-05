//Lizzie
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
