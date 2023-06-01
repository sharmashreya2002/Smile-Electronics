

// Define ISR triggered when display button is pressed
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
        Serial.println("case zero");
          display_temp(T); // change on transition. Should display to 0.1 deg C (accuracy of sensor)
          displaystate = one;
        
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
