


// Calculate instantaneous heat loss rate (W)
float find_HLR(float Tmeasured) {
  float heat_loss_rate = T/ Rth;
  return heat_loss_rate;
}

// Find heat loss in the time interval, for a given heat loss rate
float find_Qaccum(float heat_loss_rate) {

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

float time_remaining_function() {
  // Every iteration after the first will follow the following format until no more time is left
  // Print statements for debugging purposes
  heat_loss_rate = find_HLR(T);
  Qaccum = find_Qaccum(heat_loss_rate);
  Qleft = find_Q_left(Qaccum, Qleft);
  t_left = remaining_cool_life(Qleft, Qaccum, heat_loss_rate) - error_margin;


}
