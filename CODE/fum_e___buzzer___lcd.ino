//affichage d'un texte sur un LCD
#include<LiquidCrystal.h>
#include <MQUnifiedsensor.h>

const int sensorPin=A0;
int smoke_level=-1; //Initialise le niveau de fumée à une valeur impossible au débogage. Si on voit la valeur -1 sur la sortie,
                      //on sait que ce n'est pas une valeur lue par analalogRead()
int pin1= 12;
int pin8=8;

LiquidCrystal lcd(2,3,4,5,6,7);


void setup(){
  lcd.begin(16,2); //initialise le nombre de colonnes et de lignes
  Serial.begin(9600);
  pinMode(sensorPin,INPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin8, OUTPUT);
  
  
  }
void loop(){
  
  smoke_level=analogRead(sensorPin); // MQ-2 est un capteur simple de résistance analogique
  Serial.println(smoke_level);
  
  
  
  if (smoke_level >300)  { //condition pour que le capteur soit en alerte     //or (light_level>...) or (hot_level>...)
    
    lcd.setCursor(0,1); //le curseur se positionne à la 1ère colonne, 2ième ligne
    lcd.clear();
    lcd.print("Alerte Fumee");
    Serial.println("Fumée détectée");


    digitalWrite(8, HIGH);
    
    tone (12, 600); // allume le buzzer actif arduino
    delay(300);
    tone(12, 900); // allume le buzzer actif arduino
    delay(200);
    tone(12, 600); // allume le buzzer actif arduino
    delay(500);
    noTone(12);  // désactiver le buzzer actif arduino
    delay(500);    
    
  }

  else{
    lcd.clear();
    lcd.setCursor(0,0); //le curseur se positionne à la 1ère colonne, 2ième ligne
    lcd.print("Taux:");
    lcd.print(smoke_level);
    lcd.setCursor(0,1);
    lcd.print("RAS");
    noTone(12);
    noTone(8);
  }
  delay(100);
  
  
  
}
