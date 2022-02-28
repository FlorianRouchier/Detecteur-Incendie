//affichage d'un texte sur un LCD

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


int ana2;

const int sensorPin=A0;
int smoke_level=-1; //Initialise le niveau de fumée à une valeur impossible au débogage. Si on voit la valeur -1 sur la sortie,
                      //on sait que ce n'est pas une valeur lue par analalogRead()
int pin1= 12;

int pin8= 8;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


int ThermPin = 2;
int V; // tension analogique 
float R1 = 10000; // valeur de la Resistance en série 
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07; //coeficients trouvés sur internet

void setup(){
  lcd.init(); //initialise le nombre de colonnes et de lignes
  Serial.begin(9600);
  pinMode(sensorPin,INPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin8, OUTPUT);
  
  
  }
void loop(){
  ana2= analogRead(1);//lit la sortie analogique d'un potentiomètre permettant de gerer l'affichage lcd
  
  smoke_level=analogRead(sensorPin); // MQ-2 est un capteur simple de résistance analogique
  //Serial.println(smoke_level);
  
  
  
  if (smoke_level >550)  { //condition pour que le capteur soit en alerte     
    if(ana2<200){
    lcd.setCursor(0,1); //le curseur se positionne à la 1ère colonne, 2ième ligne
    lcd.clear();
    
    lcd.print("Alerte Fumee");
    
    lcd.clear();}
    
    
    Serial.println("Fumée détectée");
    buzzer();       // fonction buzzer plus pour eviter de repeter du code
     
    
  }

  else{
    if(ana2<200){
    lcd.clear();
    lcd.setCursor(0,0); //le curseur se positionne à la 1ère colonne, 2ième ligne
    lcd.print("Taux:");
    lcd.print(smoke_level);
    lcd.setCursor(0,1);
    lcd.print("RAS");}
    digitalWrite(8,LOW);
    noTone(12);
  }
  delay(100);

  // partie capteur temperature
  
  V = analogRead(ThermPin);
  R2 = R1 * (1023.0 / (float)V - 1.0);// valeur Thermistance
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // formule pour trouver T  
  T = T - 273.15;
  
  if(200<ana2){ 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(T);
  lcd.setCursor(0,1);
  lcd.print("RAS");
  
  }
  if(T>28){
    if(200<ana2){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temp: ");
      lcd.print(T);
      lcd.setCursor(0,1);
      lcd.print("CHAUD DEVANT");}
    buzzer();
  }
  Serial.print("Temperature: "); 
  Serial.print(T);
  Serial.println(" C"); 

  delay(50);


  if ((smoke_level >550) && (T>28)) {

    doublebuzzer();

  }

    
    
  
  
}
void buzzer(){
    tone (12, 600); // allume le buzzer actif arduino
    delay(300);
    tone(12, 900); // allume le buzzer actif arduino
    delay(200);
    tone(12, 600); // allume le buzzer actif arduino
    delay(500);
    noTone(12);  // désactiver le buzzer actif arduino
    delay(500); } 

void doublebuzzer(){
    digitalWrite(pin8,HIGH);
    
    tone (12, 600); // allume le buzzer actif arduino
    delay(300);
    tone(12, 900); // allume le buzzer actif arduino
    delay(200);
    tone(12, 600); // allume le buzzer actif arduino
    digitalWrite(pin8,HIGH);
    delay(500);
    noTone(12);  // désactiver le buzzer actif arduino
    delay(500); } 
  
