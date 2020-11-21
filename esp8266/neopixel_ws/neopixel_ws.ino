#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ArduinoWebsockets.h>
// // Adafruit NeoPixel library
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
#define PIN  14
#define NUMPIXELS 8

int r=0;
int g=0;
int b=0;
int bri=64;
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// connect wifi
const char* ssid = "violpi"; //Enter SSID
const char* password = "0357095092"; //Enter Password
// websocket lan
const char* websockets_server = "gw.wlan"; // raspberry pi network
const uint16_t port=3001; //server adress and port
using namespace websockets;

void showPixel() {
  pixels.fill(pixels.Color(r,g,b));
  pixels.setBrightness(bri);
  pixels.show();
}

void statusLight(int re, int gr, int bl){
  pixels.setPixelColor(0,re,gr,bl);
  pixels.show();
  }
//WEBSOCKET 
WebsocketsClient client; 
//callback fn for ws event

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
        statusLight(255,0,255);// purple for connected
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
        statusLight(255,0,0);/// red for closed
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
       statusLight(0,255,0); //green for ping
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
        statusLight(0,255,255); //cyan for pong
    }
}

void onMessageCallback(WebsocketsMessage message) {
    StaticJsonDocument<200> doc;
    String data=message.data();
    Serial.print("Got Message: ");
    Serial.println(data);
    DeserializationError error = deserializeJson(doc, data);
    Serial.println(error.f_str());
    bri=doc["brightness"]|bri;
    r=doc["red"]|r;
    b=doc["blue"]|b;
    g=doc["green"]|g;
   
    Serial.println(r);
    Serial.println(b);
    Serial.println(g);
    Serial.println(bri);
    showPixel(); 
}
void setup() {
  // put your setup code here, to run once:
  WiFi.mode(WIFI_STA); //station not access point
  Serial.begin(115200); //setup monitor
  WiFi.begin(ssid, password);
    int t=0;
    pixels.begin();
    pixels.setBrightness(bri);
    pixels.clear();


  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED){
    //blinking green till connect
     if (t%2==0) {statusLight(0,255,0);} else
                 {statusLight(0,0,0);};
     t+=1;
     delay(500);
  }
 statusLight(0,0,255); // blue for connected

  // websocket setup
   client.onEvent(onEventsCallback);
   client.onMessage(onMessageCallback);
   bool connected=client.connect(websockets_server,port,"/");
   Serial.println(connected);
   client.send("Hi Server!");
   client.ping();
}

void loop() {
  if (client.available())
    client.poll();
  else client.connect(websockets_server,port,"/");
 
}
