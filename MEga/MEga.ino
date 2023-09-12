//Communication bidirectionnel entre le arduino
//Mega et Uno
//2023-09-05
//Port Série Sérial1 pour communiqué avec le uno pin 18 et 19 (RX et TX)

#include "Wire.h"
#include "SHT31.h"

#define DEBUG

char console;
char maison;
char bluetooth;
bool fanPartie = false;

const float tempMinimumPourLaFan = 23.0;

SHT31 sht31 = SHT31();
void setup() {

#ifdef DEBUG
  Serial.begin(9600); //Console
#endif   
  Serial1.begin(9600);//Uno

  Serial2.begin(9600);//Bluetooth

  sht31.begin();
}

void loop() {
  //Écrire dans la console du uno à partir du mega
  if(Serial.available() > 0){
   console = Serial.read();
   Serial.print(" de la console ");
   Serial.print(console);
   Serial1.write(console);

  }
  //Détecte si le uno envoie un message
  if (Serial1.available() > 0) {
   maison = Serial1.read();
   Serial.print(maison);
   Serial2.write(maison);

  }
  //Bluetooth 
  if(Serial2.available() > 0){
   bluetooth = Serial2.read();
   Serial.print(" du bluetooth ");
   Serial.println(bluetooth);
   Serial1.write(bluetooth);
  
  }
  if(millis() % 1000 == 0){
    float temp = sht31.getTemperature();
    float hum = sht31.getHumidity();

    Serial.print("Temp: ");
    Serial.println(temp);
    Serial.print("Hum: ");
    Serial.println(hum);
    if (temp >= tempMinimumPourLaFan && !fanPartie) {
      Serial.println("Fan partie");
      Serial1.write("r");
      fanPartie = true;
    }
    if(fanPartie && temp < tempMinimumPourLaFan){
      Serial1.write("s");
      Serial.println("Fan éteint");
      fanPartie = false;
    
    }
  }


}
