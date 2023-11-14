/*
Gestion de la maison connectée
23-08-29 version 1 Samuel Gascon
Ne pas oublier mettre à jour la version du code en constante

------Fonctionalité-------
Maison intéligente qui inclue les fonctionalité suivantes
  - Ventialteur
    - Peut être activer à l'aide de la commande w[0-255]#
  - Détecteur de pluie
    - S'il mouille l'application affiche 'rain'
  - Détecteur de lumière
    - Voir description détecteur de mouvement
  - Détecteur de mouvement
      - Pour activer le détecteur de mouvent il faut qu'il fasse sombre et qu'il ait du mouvement devant le détecteur ce qui allume la lumière blanche devanat la maison
  - Lumière blanche
    -Détecteur de mouvement
  - Lumière jaune
    - Peut être activer par l'application ou par la commandce v[0-255]#

*/

//----------------------Include------------------------
//#include <Servo.h>
//#include <Wire.h> Inclus dans la librairie par default
#include <SoftwareSerial.h>  //Nécessaire pour détourner les communication bluetooth
#include <LiquidCrystal_I2C.h>

//------------------------------------------------------

//Adresse I2C de l'écran lcd est 0x27, qui a 2 ligne de 16 charactères
LiquidCrystal_I2C mylcd(0x27, 16, 2);



//Définition des constantes--------------------------
const int version = 1;
const int gasMaximal = 300;
const int lumiereMin = 300;
const int eauMax = 600;
const int solHumidMax = 50;
const int solHumidMin = 10;

//---------------------------------------------------
String fans_char;
int fans_val;
int gas;
int infrar;
String led2;
int lumiere;
bool gazBool;
bool pluieBool;
bool solBool;
bool megaBool = false;
bool consoleBool = false;
bool boutonGauche;
bool boutonDroit;
String buffer;
String reception;
bool lumiereAllumerParLaCommande;
String commande[8];
int commandeIndex = 0;
int historyPos = 0;
bool estDansLesCommandes = true;
String option[] = {"Temperature", "Humdite","Lumiere", "Eau","Gaz","Sol"};
int indexOption = 0;
int sol;
int val;
int value_led2;
int water;
int tonepin = 3;
float temperature;
float humid;

//Définition des Pins--------------------------------
const int gasPin = A0;
const int buzzerPin = 3;
const int rxPin = 9;
const int txPin = 10;
const int moteurDirectionPin = 7;
const int moteurVitessePin = 6;
const int boutonGauchePin = 4;  //Chiffre 3 sur la maison
const int boutonDroitPin = 8;   //Chiffre 4 sur la maison
const int detectMovePin = 2;
const int lumierePin = A1;      //Pin analogue, en lecture
const int DELBlanchePin = 13;   //DEL sur le panneau Arduino et la blanche à l'extérieur de la maison(numéro 1)
const int detectEauPin = A3;    //Détecteur de pluie sur le toit de la maison
const int detectHumidPin = A2;  //Détecteur d'humidité dans le sol
const int relaisPin = 12;       //Relais devant la maison
const int DELJaunePin = 5;      //Del près du toit

SoftwareSerial mega(rxPin, txPin);

void setup() {
  Serial.begin(9600);  //Initialiser la console a 9600 baud
  Serial.println("Senseur Maison keyes Version " + version);
  mega.begin(9600);  //Initialiser le port série pour le bluetooth ou pour un autre module en série

  mylcd.init();           //Initialiser le panneau lcd
  mylcd.backlight();      //Allumer la DEL du panneau lcd

  mylcd.setCursor(0, 0);  //Placer le cureseur sur la première colonne, premier caractère
  mylcd.print("passcord:");
  mylcd.clear();


  //Définir les pinModes ------------------------------------- S'assurer que tout les pins sont défénit en output ou en input*************
  pinMode(moteurDirectionPin, OUTPUT);
  pinMode(moteurVitessePin, OUTPUT);
  digitalWrite(moteurDirectionPin, HIGH);  //dans le sens des aiguilles d'une montre vue de l'arrière *****test*****
  digitalWrite(moteurVitessePin, HIGH);    //À tester, mettre à zéro

  pinMode(rxPin, INPUT);   //Définir la pin rx du deuxième port série
  pinMode(txPin, OUTPUT);  //Définir la pin tx du deuxième port série
  pinMode(boutonGauchePin, INPUT);
  pinMode(boutonDroitPin, INPUT);
  pinMode(detectMovePin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(gasPin, INPUT);  //Pin de la lecture de la quantité de gas présent
  pinMode(lumierePin, INPUT);
  pinMode(DELBlanchePin, OUTPUT);
  pinMode(detectEauPin, INPUT);
  pinMode(detectHumidPin, INPUT);
  pinMode(relaisPin, OUTPUT);
  pinMode(DELJaunePin, OUTPUT);
}

void loop() {
  lireSenseurs();  //On veut lire les senseurs le plus souvent possible et avoir l'état de la maison le plus souvent possible
  if (mega.available() > 0) {
    val = mega.read();
      reception = (" du mega ");
      megaBool = true;
      commande[commandeIndex] = val;
      mylcd.setCursor(0, 0);
      mylcd.print("Commande: ");
      mylcd.print(val);
      commandeIndex++;
      if (commandeIndex > 8) {
      commandeIndex = 0;
      }
    

  }
  if (Serial.available() > 0) {
    val = Serial.read();  //Définir la valeur de val à la valeur de la commmande
    reception = ("Console");
    consoleBool = true;
    mylcd.clear();
    commande[commandeIndex] = val;
    mylcd.setCursor(0, 0);
    mylcd.print("Commande: ");
    mylcd.print(val);
    commandeIndex++;
   
    if (commandeIndex > 8) {
    commandeIndex = 0;
    }
    delay(100);
  }
  //Commande recu
  if (megaBool || consoleBool) {
    //digitalWrite(DELJaunePin,HIGH);
    tone(buzzerPin, 440,2);
  }
  Menu();

  switch (val) {
    case 'a':
      digitalWrite(DELBlanchePin, HIGH);  //Allumer la led blanche
      Serial.println("Del blanche allumé " );
      lumiereAllumerParLaCommande = true;
      break;
    case 'b':
      digitalWrite(DELBlanchePin, LOW);  //Éteindre la led blanche
      Serial.println("Del blanche éteint " );
      lumiereAllumerParLaCommande = false;
      break;
    case 'c':
      Serial.println("Relais allumé ");
      digitalWrite(relaisPin, HIGH);  //Connecter le relais
      break;
    case 'd':
      Serial.println("Relais fermer ");
      digitalWrite(relaisPin, LOW);  //Éteindre le relais
      break;
    case 'e':
      //music1();
      break;
    case 'f':
      //music2();
      break;
    case 'g':
      //noTone(3);
      break;
    case 'h':
      Serial.print(" Valeur de la lumière ");  //Envoyer la valeur de la lumière dans la console
      Serial.println(lumiere);
      if (reception == " du mega ") {
        mega.print(" Valeur de la lumière ");
        mega.println(lumiere);
      }
      delay(100);
      break;
    case 'i':  //Envoyer la valeur du gaz dans la console
      Serial.print("Valeur du gaz ");  //Envoyer la valeur du gas dans la console
      Serial.println(gas);
      if (reception == " du mega ") {
        mega.print("Valeur du gaz ");  //Envoyer la valeur du gas dans la console
        mega.println(gas);
      }
      delay(100);
      break;
    case 'j':  //Envoyer la valeur de l'humidité du sol dans la console
      Serial.print(" Valeur du sol ");  //Envoyer la valeur du sol dans la console
      Serial.println(sol);

      if (reception == " du mega ") {
        mega.print("Valeur du sol ");
        mega.println(sol);
      }
      delay(100);
      break;
    case 'k':
      Serial.print(" Valeur de l'eau ");  //Envoyer la valeur de la lumière dans la console
      Serial.println(water);
      if (reception == " du mega ") {
        mega.print("Valeur de l'eau ");
        mega.println(water);
      }
      delay(100);
      break;
    case 'l':
      // servo_9.write(180);
      break;
    case 'm':
      // servo_9.write(0);
      break;  //exit loop
    case 'n':
      // servo_10.write(180);
      break;
    case 'o':
      // servo_10.write(0);
      break;
    case 'p':
      digitalWrite(DELJaunePin, HIGH);  //Allumer et éteindre la led
      
      break;
    case 'q':
      digitalWrite(DELJaunePin, LOW);
      break;
    case 'r':
      digitalWrite(moteurDirectionPin, LOW);
      digitalWrite(moteurVitessePin, HIGH);
      break;
    case 's':
      digitalWrite(moteurDirectionPin, LOW);
      digitalWrite(moteurVitessePin, LOW);  //Arrêter la fanne
      break;
    case 't':
      buffer = Serial.readStringUntil('#');  //Vider le buffer (tampon du port série)
      // servo1_angle = String(servo1).toInt();
      // servo_9.write(servo1_angle);
      //delay(300);
      break;
    case 'u':
      buffer = Serial.readStringUntil('#');  //Vider le buffer (tampon du port série)
      // servo2_angle = String(servo2).toInt();
      // servo_10.write(servo2_angle);
      //delay(300);
      break;
    case 'v':
      if(megaBool){
       led2 = mega.readStringUntil('#');
       megaBool = false;
      }
      else{
       led2 = Serial.readStringUntil('#');
       consoleBool = false;
      }
      value_led2 = String(led2).toInt();  //Points supplémentaires pour ceux qui vont faire la logique de validation
      analogWrite(DELJaunePin, value_led2);
     
      break;
    case 'w':  //Points supplémentaires pour ceux qui vont faire la logique de validation
      if(megaBool){
       fans_char = mega.readStringUntil('#');
       megaBool = false;
      }
      else{
       fans_char = Serial.readStringUntil('#');
       consoleBool = false;
      }
      fans_val = String(fans_char).toInt();
      digitalWrite(moteurDirectionPin, LOW);
      analogWrite(moteurVitessePin, fans_val);
      break;
    case 'x':
       delay(5); //délais nécessaire si on utilise la deuxième méthode (plusieurs print) pour envoyer les donnés

      while (mega.available() > 0) {
        // trouver un INT dans le stream dans le buffer du port série
        // Parse int laisse tomber ce qui précède le INT, et les délimiteeurs.
        long tmp = mega.parseInt();
        // Encore
        long hmd = mega.parseInt();
        
        long response = 0;

        // look for the newline. That's the end of your sentence:
        if (mega.read() == '\n') {
         
          Serial.print(" Réponse: ");
          temperature = tmp / 100.0;
          humid = hmd / 100.0;
          Serial.print(temperature);
          Serial.print(" ");
          Serial.println(humid);
    
        }
      }
      break;
  }
  val = ' ';
}
//-----------------------------------------------------------------------------------------------------------------
//      Menu(Commande, senseur) pour changer et acceder aux sous menu appuyer sur le bouton de gauche de la maison
//-----------------------------------------------------------------------------------------------------------------
void Menu(){
  if(!digitalRead(boutonGauchePin)){
    estDansLesCommandes= !estDansLesCommandes;
    mylcd.clear();
  }
  if(estDansLesCommandes){

    mylcd.setCursor(0, 0);
    mylcd.print("Commande: ");
    mylcd.print(commande[historyPos]);
    if (!digitalRead(boutonDroitPin)) {
      if (historyPos > 7) {
      historyPos = -1;
      }
      mylcd.clear();
      mylcd.setCursor(0, 0);
      mylcd.print("Commande: ");
      historyPos ++;
      mylcd.print(commande[historyPos]); 
      mylcd.print(" ");
      mylcd.print(historyPos);
      delay(1000);

    }
  }else{
    mylcd.setCursor(0, 0);  
    mylcd.print(option[indexOption]); 
    if (!digitalRead(boutonDroitPin)){
      indexOption++;
      mylcd.clear();
      delay(500);
      if(indexOption > 4){
        indexOption = 0;
      }
      option[0] = "Temp:";
      option[0] += temperature;
      option[1] = "Humidite:";
      option[1] += humid;
      option[2] = "Lumiere:";
      option[2] += lumiere;
      option[3] = "Eau:";
      option[3] += water;
      option[4] = "Gas:";
      option[4] += gas;
      option[5] = "Sol:";
      option[5] += sol;
    }
  }

}
//Lire l'état des senseurs de la maison
void lireSenseurs() {
  gas = analogRead(gasPin);
  if (gas > gasMaximal && !gazBool) {         //Tester avec une valeur réelles
    Serial.println("danger gaz élevé");       //Pour débugger sur la console
    mega.println("Danger gaz");  //*****test
    gazBool = true;
  } else {
    if (gas <= gasMaximal) {
      gazBool = false;
    }
  }
  //--------------Lumière----------------------
  lumiere = analogRead(lumierePin);
  if (lumiere < lumiereMin && !lumiereAllumerParLaCommande) {
      
    Serial.println("Lumière insuffisante");  //*********************tester si nécessaire sur blutooth
    mega.println("Lumière insuffisante");
    if (digitalRead(detectMovePin)) {
      digitalWrite(DELBlanchePin, HIGH);  //Allumer la lumière m
    } else {
      digitalWrite(DELBlanchePin, LOW);  //Fermer la lumière
    }
  } else {  //               
    if (!lumiereAllumerParLaCommande) {
      digitalWrite(DELBlanchePin, LOW);
    }
  }
  //--------------------------------------------
  //-------Détecter l'eau-----------------------
  //Si il détecte de l'eau pour la première fois, le programme affiche qu'il pleut ou 'rain' sur le bluetooth et mets la variable pluieBool à true pour que la console n'affiche pas à l'infinie
  water = analogRead(detectEauPin);
  if (water > eauMax && !pluieBool) {
    Serial.println("il pleut");         //Pour débugger sur la console
    mega.println("rain");  //Doit être en anglais pour le programme sur le téléphone
    pluieBool = true;

  } else {
    if (water <= eauMax) {
      pluieBool = false;
    }
  }
  //--------------------------------------------
  //----------Détecteur d'humidité--------------
  sol = analogRead(detectHumidPin);
  if (sol > solHumidMax) {
    Serial.println("sol trop humide");        //Pour débugger sur la console
    mega.println("hydroponia");  //Doit être en anglais pour le programme sur le téléphone
    solBool = true;
  } else {
    if (sol <= solHumidMax && sol >= solHumidMin) {
      solBool = false;
    } else {
      if (sol < solHumidMin && !solBool) {
        Serial.println("Sol trop sec");
        mega.println("Sol trop sec");
        solBool = true;
      }
    }
  }
  //-------------------------------------------
}
