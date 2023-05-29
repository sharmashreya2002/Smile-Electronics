// First, define all relevant calculated quantities that remain fixed 

// Calculated overall thermal resistance for SMILE backpack
float Rth = 9.715;

// Area over which the heat is lost, relevant to flux calculations (from prev.) 
float area = 0.179;

// Energy needed to melt ice in ice pack (from prev.)
float Q_total = 868400;

// Time interval between temperature measurements (convert from minutes to seconds)
int t = 5 * 60;

// Value to be measured with temperature sensor, keep dummy value of 24 for now
float Tmeasured = 24; // change to temperature measurement 

float Qaccum;
float Qleft;
float heat_loss_rate;
float cool_life;

// Calculate instantaneous heat loss rate (W)
float find_HLR(float Tmeasured){
  float heat_loss_rate = Tmeasured/Rth; 
  return heat_loss_rate;  
}

// Find heat loss in the time interval, for a given heat loss rate
float find_Qaccum(float heat_loss_rate){

  // Heat lost from ice pack in this time interval
  float Qaccum = heat_loss_rate * t;
  return Qaccum;
}

float find_Q_left(float Qaccum, float Qleft) {
  // Q left for first iteration is Q total, every iteration following this has less
  return Qleft - Qaccum;
}

// Calculate remaining cool life in hours
float remaining_cool_life(float Qleft, float Qaccum, float heat_loss_rate) {
  // Predict remaining cool life, assuming that the instantaneous heat flux will remain constant until the end
  return (Qleft - Qaccum) / (heat_loss_rate * 3600);
}

void setup() {
  Serial.begin(9600); // Initialize the serial communication
  // add temperature begin sequence
  // First iteration of the algorithm: -  initialise with start values
    
  heat_loss_rate = find_HLR(Tmeasured);
  Qaccum = find_Qaccum(heat_loss_rate);
  Qleft = find_Q_left(Qaccum, Q_total);
  cool_life = remaining_cool_life(Qleft, Qaccum, heat_loss_rate);
   

  Serial.print("Qaccum");
  Serial.println(Qaccum);

  Serial.print("heat_loss_rate");
  Serial.println(heat_loss_rate);
  
  /*Serial.println("Qleft");
  Serial.println(Qleft);

  Serial.print("cool_life");
  Serial.println(cool_life);*/

}

void loop() {


  // Every iteration after the first will follow the following format until no more time is left
  // Print statements for debugging purposes
  heat_loss_rate = find_HLR(Tmeasured);
  Qaccum = find_Qaccum(heat_loss_rate);
  Qleft = find_Q_left(Qaccum, Qleft);
  cool_life = remaining_cool_life(Qleft, Qaccum, heat_loss_rate);
  //Serial.println(heat_loss_rate, 6);
  //Serial.println(Qaccum);
  Serial.print("Qleft: ");
  Serial.println(Qleft);
  Serial.print("Cool Life: ");
  Serial.println(cool_life);
  
  delay(1000); // Delay for 1 second
}
