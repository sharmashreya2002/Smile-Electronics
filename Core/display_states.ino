//May

/*
 * This file contains functions relating to the display operation
 */

 
// Define ISR which is triggered when display button is pressed
void display_isr(){ 
  display_active = 1; 
  t_at_display = millis(); // used to track how long screen should now stay on for
}

// Function to display temperature
void display_temp(float T){
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.clearDisplay();
  u8g2.setFont(u8g2_font_helvB24_tf);
  u8g2.setCursor(31, 100); // Set cursor position. For some reason there is an offset of about 30 pixels in the x direction - suspect faulty board but can't be sure
  u8g2.print(T, 1); // print temperature value to 1 decimal place
  u8g2.setCursor(95, 100);
  u8g2.print("°C");
  u8g2.sendBuffer(); // send buffer to display!

}
// Function to display remaining time
void display_timeleft(float t){
  u8g2.clearDisplay(); 
  u8g2.setCursor(31, 100);
  u8g2.print(t, 1);
  u8g2.sendBuffer();
  }


// Display State Machine
void display_state_machine(){
  
    switch(displaystate){
      case zero : {
        Serial.println("case zero");
        float chamber_temp = find_chamber_temp(heat_loss_rate); // find estimate of chamber temperature
          display_temp(chamber_temp); // Update display on transition
          displaystate = one; // Move to next state    
        break;    
        }
       case one:{
        Serial.println("case one");
        if (time_since_button > 5000){ // 5 second display time, probably longer than needed
          display_timeleft(t_left); // Update display with remaining cool life
          displaystate = two; // Move to next state
        }
       }
        break;
       case two: {
        Serial.println("case two");
        if (time_since_button > 10000 ){
          u8g2.clearDisplay(); // 10 seconds is up so clear display
          u8g2.clearBuffer();
          displaystate = zero;
          Serial.println("display state set to zero");
          display_active = 0; // next run through main loop will not call the display state machine
        }
        
       }
      }
       
    }
