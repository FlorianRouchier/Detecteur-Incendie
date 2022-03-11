   
//affichage d'un texte sur un LCD

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(11,13);

/*capteur particules*/

unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 1000;//sampe 1s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
/* ----------- */

int ana2;// potentiomètre 
int ana3; // luminosité 
const int sensorPin=A0;
int smoke_level=-1; //Initialise le niveau de fumée à une valeur impossible au débogage. Si on voit la valeur -1 sur la sortie,
                      //on sait que ce n'est pas une valeur lue par analalogRead()
int pin1= 12;

int pin8= 8;// pin buzzer
int pin3=3;// pin capteur particules

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


int ThermPin = 2;
int V; // tension analogique 
float R1 = 10000; // valeur de la Resistance en série 
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07; //coeficients trouvés sur internet

void setup(){
  mySerial.begin(9600);
  pinMode(3,INPUT);
  lcd.init(); //initialise le nombre de colonnes et de lignes
  lcd.backlight(); // active le rétro éclairage
  Serial.begin(9600);
  pinMode(sensorPin,INPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin8, OUTPUT);
  starttime = millis();//get the current time;
  
  
  }
void loop(){
  duration = pulseIn(pin3, LOW);
  lowpulseoccupancy = lowpulseoccupancy+duration;
  ana2= analogRead(1);//lit la sortie analogique d'un potentiomètre permettant de gerer l'affichage lcd
  ana3 = analogRead(3);
  if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    Serial.print("concentration = ");
    Serial.print(concentration);
    mySerial.print(concentration);
    mySerial.print("$0");
    Serial.print(" pcs/0.01cf  -  ");

  
  
  if (ana3>650){
    mySerial.print("nuit$3");
    if (ana2>400 && ana2<600){
      
    
        lcd.clear();
        lcd.setCursor(0,0); //le curseur se positionne à la 1ère colonne, 2ième ligne
        lcd.print("Luminosite:  ");
        lcd.setCursor(0,1);
        lcd.print("nuit");
        
    }
  }
  if (ana3>250&& ana3<650){
    mySerial.print("Sombre$3");
    if (ana2>400 && ana2<600){
      
    
        lcd.clear();
        lcd.setCursor(0,0); //le curseur se positionne à la 1ère colonne, 2ième ligne
        lcd.print("Luminosite:  ");
        lcd.setCursor(0,1);
        lcd.print("Sombre");
        
    }
  }
  if (ana3<250){
    mySerial.print("jour$3");
    if (ana2>400 && ana2<600){
      
    
        lcd.clear();
        lcd.setCursor(0,0); //le curseur se positionne à la 1ère colonne, 2ième ligne
        lcd.print("Luminosite:  ");
        lcd.setCursor(0,1);
        lcd.print("jour");
        
    }
  }
  
  if( ana2>200 && ana2<400){
          
          lcd.clear();
          lcd.setCursor(0,0); //le curseur se positionne à la 1ère colonne, 2ième ligne
          lcd.print("Nb part: ");
          lcd.print(concentration);
          lcd.setCursor(0,1);
          lcd.print("RAS");
        }
    if (concentration<20000){
      mySerial.print("RAS$5");
    }
      if (concentration > 20000 && concentration < 315000) {
        mySerial.print("FUMEE!$5");
        if( ana2>200 && ana2<400){
          Serial.println("Fumée alumettes detectée");
          lcd.clear();
          lcd.setCursor(0,0); //le curseur se positionne à la 1ère colonne, 2ième ligne
          lcd.print("Nb part: ");
          lcd.print(concentration);
          lcd.setCursor(0,1);
          lcd.print("Fumee allumette!");
          
        } 
     
    }
      if (concentration > 315000) {
        mySerial.print("DANGER!$5");
        if( ana2>200 && ana2<400){
          Serial.println("Fumée importante danger");
          lcd.clear();
          lcd.setCursor(0,0); //le curseur se positionne à la 1ère colonne, 2ième ligne
          lcd.print("Nb part: ");
          lcd.print(concentration);
          lcd.setCursor(0,1);
          lcd.print("DANGER ");
          
        } 
        buzzer();
      
     
  }
    
    
  
  
  
  smoke_level=analogRead(sensorPin); // MQ-2 est un capteur simple de résistance analogique
  Serial.println(smoke_level);
  mySerial.print(smoke_level);
  mySerial.print("$1");
  
  
  if (smoke_level >600)  { //condition pour que le capteur soit en alerte 
    mySerial.print("Fumée!$6");    
    if(ana2<200){
    lcd.setCursor(0,1); //le curseur se positionne à la 1ère colonne, 2ième ligne
    lcd.clear();
    
    lcd.print("Alerte Fumee");
    
    lcd.clear();}
    
    
    Serial.println("Fumée détectée");
    buzzer();       // fonction buzzer plus pour eviter de repeter du code
     
    
  }

  else{
    mySerial.print("RAS$6");
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
  mySerial.print(T);
  mySerial.print("$2");
  if(ana2>600){ 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(T);
  lcd.setCursor(0,1);
  lcd.print("RAS");
  
  
  }
  if(T>22){
    mySerial.print("CHAUD!$7");
    
    if(ana2>600){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Temp: ");
      lcd.print(T);
      lcd.setCursor(0,1);
      lcd.print("CHAUD DEVANT");
      }
    buzzer();
  }
  else{
    mySerial.print("RAS$7");
  }
  Serial.print("Temperature: "); 
  Serial.print(T);
  Serial.println(" C"); 

  delay(50);


  if ((smoke_level >600) && (T>28)) {

    doublebuzzer();

  }
  lowpulseoccupancy = 0;
  starttime = millis();
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
  
