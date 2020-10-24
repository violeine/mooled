#include <ESP8266WiFi.h>
const int R = 12;
const int G = 13;
const int B=14;
//HIGH is off
//LOW is on
int rgb(int v){
  return 254-v;
}
void setRGB(int r, int g, int b){
  analogWrite(R,rgb(r));
  analogWrite(G,rgb(g));
  analogWrite(B,rgb(b));
}
void setup() {
  // put your setup code here, to run once:
  pinMode(R,OUTPUT);
  Serial.begin(115200);
  Serial.println("ON AP");
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);
  digitalWrite(R,HIGH);
  digitalWrite(B,HIGH);
  digitalWrite(G,HIGH);
  analogWriteRange(254);
}

void loop() {
 int red=255;
 int green=0;
 int blue=0;
 for (int i = 0; i <= 255; i++) {
    blue = i;
 setRGB(red,green,blue);
    delay(20);
  } 
  for (int i = 0; i <= 255; i++) {
    blue = 255 - i;
  setRGB(red,green,blue);
    delay(20);
  }  
}
