

//Fréquence du buzzer
const int D0 = -1;
const int D1 = 262;
const int D2 = 293;
const int D3 = 329;
const int D4 = 349;
const int D5 = 392;
const int D6 = 440;
const int D7 = 494;
const int M1 = 523;
const int M2 = 586;
const int M3 = 658;
const int M4 = 697;
const int M5 = 783;
const int M6 = 879;
const int M7 = 987;
const int H1 = 1045;
const int H2 = 1171;
const int H3 = 1316;
const int H4 = 1393;
const int H5 = 1563;
const int H6 = 1755;
const int H7 = 1971;

const int WHOLE = 1;
const int HALF = 0.5;
const int QUARTER = 0.25;
const int EIGHTH = 0.25;
const int SIXTEENTH = 0.625;

//set sound play frequency
int tune[] = {
  M3, M3, M4, M5,
  M5, M4, M3, M2,
  M1, M1, M2, M3,
  M3, M2, M2,
  M3, M3, M4, M5,
  M5, M4, M3, M2,
  M1, M1, M2, M3,
  M2, M1, M1,
  M2, M2, M3, M1,
  M2, M3, M4, M3, M1,
  M2, M3, M4, M3, M2,
  M1, M2, D5, D0,
  M3, M3, M4, M5,
  M5, M4, M3, M4, M2,
  M1, M1, M2, M3,
  M2, M1, M1
};
int length = sizeof(tune) / sizeof(tune[0]);  //Paramètre des tounes

//set music beat
float durt[] = {
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1 + 0.5,
  0.5,
  1 + 1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1 + 0.5,
  0.5,
  1 + 1,
  1,
  1,
  1,
  1,
  1,
  0.5,
  0.5,
  1,
  1,
  1,
  0.5,
  0.5,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0.5,
  0.5,
  1,
  1,
  1,
  1,
  1 + 0.5,
  0.5,
  1 + 1,
};
////////////////////////set birthday song//////////////////////////////////
void birthday() {
  tone(3, 294);  //digital 3 outputs 294HZ sound
  delay(250);    //delay in 250ms
  tone(3, 440);
  delay(250);
  tone(3, 392);
  delay(250);
  tone(3, 532);
  delay(250);
  tone(3, 494);
  delay(500);
  tone(3, 392);
  delay(250);
  tone(3, 440);
  delay(250);
  tone(3, 392);
  delay(250);
  tone(3, 587);
  delay(250);
  tone(3, 532);
  delay(500);
  tone(3, 392);
  delay(250);
  tone(3, 784);
  delay(250);
  tone(3, 659);
  delay(250);
  tone(3, 532);
  delay(250);
  tone(3, 494);
  delay(250);
  tone(3, 440);
  delay(250);
  tone(3, 698);
  delay(375);
  tone(3, 659);
  delay(250);
  tone(3, 532);
  delay(250);
  tone(3, 587);
  delay(250);
  tone(3, 532);
  delay(500);
}
// Birthday song
void music1() {
  birthday();
}
//Ode to joy
void music2() {
  Ode_to_Joy();
}
void Ode_to_Joy()  //play Ode to joy song
{
  for (int x = 0; x < length; x++) {
    tone(tonepin, tune[x]);
    delay(300 * durt[x]);
  }
}