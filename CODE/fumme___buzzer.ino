const int sensorPin= A0;
int smoke_level = -1;                      
int pin1 = 12;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin,INPUT);
  pinMode(pin1, OUTPUT);
}

void loop() {
  smoke_level=analogRead(sensorPin); // MQ-2 est un capteur simple de résistance analogique
  Serial.println(smoke_level);
  if (smoke_level >200) {           //condition pour que le capteur soit en alerte
    Serial.println("Fumée détectée");
    tone (12, 600); // allume le buzzer actif arduino
    delay(300);
    tone(12, 900); // allume le buzzer actif arduino
    delay(200);
    tone(12, 600); // allume le buzzer actif arduino
    delay(500);
    noTone(12);  // désactiver le buzzer actif arduino
    delay(500);    
    
  }
  
  else {
    
    noTone(12);
  }

  delay(100);

}
