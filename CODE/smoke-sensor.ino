#include <MQUnifiedsensor.h>

const int sensorPin=A0;
int smoke_level=-1; //Initialise le niveau de fumée à une valeur impossible au débogage. Si on voit la valeur -1 sur la sortie,
                      //on sait que ce n'est pas une valeur lue par analalogRead()


void setup() {
  Serial.begin(9600);
  pinMode(sensorPin,INPUT);
}

void loop() {
  smoke_level=analogRead(sensorPin); // MQ-2 est un capteur simple de résistance analogique
  Serial.println(smoke_level);
  if (smoke_level >1023) {           //condition pour que le capteur soit en alerte
    Serial.println("Fumée détectée");
  }
  delay(100);

}
