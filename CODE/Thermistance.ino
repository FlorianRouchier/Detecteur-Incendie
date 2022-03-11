const int A=6;
float res;
float tot;
void setup() {
  Serial.begin(9600);
  pinMode(A,INPUT);
  

}

void loop() {
    res=digitalRead(A);
    tot=res*5/1023;
    Serial.println(tot);
}
