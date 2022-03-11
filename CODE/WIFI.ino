/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <SoftwareSerial.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>  


const char *ssid = ".............";
const char *password = "azertyuiopqsd";
String tableau[10]; 

WebServer server(80);
SoftwareSerial mySerial(12,14);


void handleRoot() {
  char caracteres[5700];
  String particules=tableau[0];
  String taux=tableau[1];
  String temp=tableau[2];
  String lum= tableau[3];
  
  String alertepart=tableau[5];
  String alertetaux=tableau[6];
  String alertetemp=tableau[7];
  snprintf(caracteres, 5700,

    "<html>\
    <head>\
      <meta charset='utf-8'>\
      <meta http-equiv='refresh' content='1'/>\
      <title>Alarme Incendie</title>\
      <style>\
        :root{--clr: #c99fa8;font-family:Arial;}\
        body{background-color:var(--clr);}\
        header{text-align: center; display: block; margin: auto; margin-bottom:20px;}\
        .element{width: 400px; display: inline-block; background:white; margin:auto;cursor: pointer;border-radius:45em;border: solid black 2px;}\
        .element:hover{background:#467088;color:white;}\
        .element2{width:717px;text-align:initial;background:white;margin:auto;cursor:pointer;border-radius:0.5em;border:solid black 2px;}\
        .element2:hover{background:#467088;color:white;}\
        h2{text-decoration:underline;text-decoration-color:red;text-decoration-thickness:2px;}\
      </style>\  
    </head>\
    <body>\
      <header>\
        <div class='element'>\
          <h1>ALARME INCENDIE</h1>\
        </div>\
      </header>\
      <section>\
        <div class='element2'>\
          <div id='intro'>\
            <p>\
              <i>En cas d'incendie, les fumées précèdent les flammes. Elles sont toxiques et inflammables. La solution: de bon réflexes et notre détecteur d'incendie.</i>\
            </p>\
          </div>\
          <h2>Capteur de fumée</h2>\
            <p>Le capteur MQ-2 signale la fumée par le niveau de tension qu'il produit. Plus il y a de fumée plus la tension est élevée.</p>\
            <p>Taux: %s , %s</p>\
          <h2>Capteur de Chaleur</h2>\
            <p>Un capteur Thermistance permet de mesurer la température de l'environnement. La valeur de la résistance augmente en fonction de la chaleur.</p>\
            <p>Temp: %s °C, %s</p>\
          <h2>Capteur de particules</h2>\
            <p>Ce capteur permet de determiner la concentration de particules dans l'air, les particules de l'ordre de quelques micromètres obstruent plus ou moins un faisceau lumineux ce qui permet de déduire la qualité de l'air.</p>\
            <p>Nombre de particules: %s  pcs/0.01cf, %s </p>\
          <h2>Capteur de Luminosité</h2>\
            <p>Ce capteur a vocation à être utilisé dans le cas ou l'on a besoin de surveiller une pièce noire tout le temps comme un entrepot. La seule source de lumière serait alors une anomalie ici un incendie.</p>\
            <p>Luminosité: %s </p>\    
        </div>\
      </section>\
      <div id='GitHUBfeed'>\
        <h2>Notre GitHub</h2>\
          <figure>\
            <img src='Capture.PNG' alt='la mascote Github'/>\
          </figure>\
        <a href='https://github.com/FlorianRouchier/Detecteur-Incendie'></a>\
        <script charset='utf-8'></script>\
      </div>\
      <div>\
        <IFRAME src='https://github.com/FlorianRouchier/Detecteur-Incendie' width='700' height='300'></IFRAME<\
      </div>\
    </body>\
    </html>",

   taux,alertetaux, temp, alertetemp, particules,alertepart, lum);
  
 server.send(200, "text/html", caracteres);
  
}

void handleNotFound() {
 
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  
}

void setup(void) {
  mySerial.begin(9600);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
  recupdonee();
  
  Serial.print(tableau[0]);
  Serial.print(" : ");
  Serial.print(tableau[1]);
  Serial.print(" : ");
  Serial.print(tableau[2]);
  Serial.print(" : ");
  Serial.print(tableau[3]);
  Serial.print(" : ");
  Serial.print(tableau[4]);
  Serial.print(" : ");
  Serial.print(tableau[5]);
  Serial.print(" : ");
  Serial.print(tableau[6]);
  Serial.print(" : ");
  Serial.println(tableau[7]);
  
      
  
  
    
}
void recupdonee(){
  String intermediaire;
   while(mySerial.available()){
    char mychar=mySerial.read();
    
    if(mychar!='$'){
      delay(20);
      intermediaire+=mychar;
    }
    else{
      char indice = mySerial.read();
      int e = indice-'0';
      tableau[e]=intermediaire;
      intermediaire="";
    
  }
}
}
