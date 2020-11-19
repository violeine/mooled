#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ArduinoWebsockets.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define PIN  14
#define NUMPIXELS 8
// Adafruit NeoPixel library

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const char* ssid = "violpi"; //Enter SSID
const char* password = "0357095092"; //Enter Password
const char* websockets_server = "192.168.4.1";
const uint16_t port=3001; //server adress and port
using namespace websockets;

void showSinglePixel(int r,int g,int b,int bri) {
  pixels.fill(pixels.Color(r,g,b));
  pixels.setBrightness(bri);
  pixels.show();
}

//WEBSOCKET 
WebsocketsClient client; 
//callback fn for ws event

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
        showSinglePixel(255,0,255,255);// purple for connected
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
        showSinglePixel(255,0,0,255);/// red for closed
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
       showSinglePixel(0,255,0,255); //green for ping
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
        showSinglePixel(0,255,255,255); //cyan for pong
    }
}

void onMessageCallback(WebsocketsMessage message) {
    StaticJsonDocument<200> doc;
    String data=message.data();
    Serial.print("Got Message: ");
    Serial.println(data);
    DeserializationError error = deserializeJson(doc, data);
    Serial.println(error.f_str());
    int r=doc["red"];
    int b=doc["blue"];
    int g=doc["green"];
    int bri=doc["brightness"];
   showSinglePixel(r,g,b,bri); 
}
void setup() {
  // put your setup code here, to run once:
  WiFi.mode(WIFI_STA); //station not access point
  Serial.begin(115200); //setup monitor
  WiFi.begin(ssid, password);
    int t=0;
    pixels.begin();
    pixels.clear();


  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED){
    //blinking green till connect
     if (t%2==0) {showSinglePixel(0,255,0,255);} else
                 {showSinglePixel(0,0,0,255);};
     t+=1;
     delay(1000);
  }
 showSinglePixel(0,0,255,255); // blue for connected

  // websocket setup
   client.onEvent(onEventsCallback);
   client.onMessage(onMessageCallback);
   bool connected=client.connect(websockets_server,port,"/");
   Serial.println(connected);
   client.send("Hi Server!");
   client.ping();
}

void loop() {
  client.poll();
}
