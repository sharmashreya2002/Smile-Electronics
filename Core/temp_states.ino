
void state_machine() {
 /* if (reset_state == 1){
    state = start;
  }
  else {*/
  
    switch (state) {

      case start: {
        Serial.println("start state");
        if (t_left > 40 and T < 80) {
          Serial.println("changing to safe_region");
          state = safe_region;
        }
        else {
          state = limited_time;
          Serial.println("changing to limited time");
        }      
      }
      case safe_region:{ 
        Serial.println("safe region");
        if (T > 80){
          Serial.println("changing to unsafe");
          state = unsafe;
        }
        if (t_left <= 40) {
          state = limited_time;
          Serial.println("changing to limited time");
        }
        break;
      }
      case limited_time:{
        Serial.println("limited time");
        if (t_left <= 10){ // probably want to flash the amber led now
        if (T > 8){
          state = unsafe;
          Serial.println("changing to unsafe");
          
        }
        break;
      case unsafe:{
        // do nothing, maybe flash red led
      }
      
      }
          
  }
  
}
}
