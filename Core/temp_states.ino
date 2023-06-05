//May

/*
 * This file contains the temperature state machine
 */
void state_machine() {
    switch (state) {
      case start: {
        Serial.println("start state");
        if (t_left > 10) { // More than 10 hours remaining
          Serial.println("changing to safe_region");
          state = safe_region;
        }
        else { // Fewer than 10 hours remaining - would not expect this
          state = limited_time;
          Serial.println("changing to limited time");
        }      
      }
      case safe_region:{ 
        Serial.println("safe region");
        if (t_left < 10){ // 
          Serial.println("changing to limited time");
          state = limited_time;
        }
      /*  if (t_left <= 40) {
          state = limited_time;
          Serial.println("changing to limited time");
        } */
        break;
      }
      case limited_time:{
        Serial.println("limited time");
        // Might want to have a flashing indicator LED in this state
        if (t_left = 0){ // No longer know what state the vaccines are in
          state = unsafe;
          Serial.println("changing to unsafe");
          
        }
        break;
      case unsafe:{
        // Currently no output but could flash a red led
      }
      
      
          
  }
  
}
}
