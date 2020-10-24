#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ArduinoWebsockets.h>
const int R = 12;
const int G = 13;
const int B = 14;

const char* ssid = "violpi"; //Enter SSID
const char* password = "0357095092"; //Enter Password
const char* websockets_server = "192.168.4.1";
const uint16_t port=3001; //server adress and port
using namespace websockets;


//LED STUFF
//HIGH is off
//LOW is on
int rgb(int v){
  return 255-v;
}
void setRGB(int r, int g, int b){
  analogWrite(R,rgb(r));
  analogWrite(G,rgb(g));
  analogWrite(B,rgb(b));
}
//digital RGB 
void dRGB(int r, int g, int b) {
   digitalWrite(R,r?LOW:HIGH);
   digitalWrite(G,g?LOW:HIGH);
   digitalWrite(B,b?LOW:HIGH);  
}


//WEBSOCKET 
WebsocketsClient client; 
//callback fn for ws event

void onEventsCallback(WebsocketsEvent event, String data) {
    if(event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connnection Opened");
        dRGB(1,0,1); // purple for connected
    } else if(event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connnection Closed");
        dRGB(1,0,0); // red for closed
    } else if(event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
        dRGB(0,1,0); //green for ping
    } else if(event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
        dRGB(0,1,1); //cyan for pong
    }
}

void onMessageCallback(WebsocketsMessage message) {
    StaticJsonDocument<200> doc;
    String data=message.data();
    Serial.print("Got Message: ");
    Serial.println(data);
    DeserializationError error = deserializeJson(doc, data);
    int r=doc["red"];
    int b=doc["blue"];
    int g=doc["green"];
    Serial.println(r);
    setRGB(r,g,b);
}

void setup() {
  // put your setup code here, to run once:
  analogWriteRange(254);    
  WiFi.mode(WIFI_STA); //station not access point
  Serial.begin(115200); //setup monitor
  WiFi.begin(ssid, password);

  //set output for 3 led
  pinMode(R,OUTPUT);
  pinMode(G,OUTPUT);
  pinMode(B,OUTPUT);
  //turn off 3 led 
  dRGB(0,0,0);
  int t=0;
  //attempting connect wifi
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED){
    //blinking green till connect
     if (t%2==0) {dRGB(0,1,0);} else
                 {dRGB(0,0,0);};
     t+=1;
     delay(1000);
  }
  dRGB(0,0,1); // blue for connected

  // websocket setup
   client.onEvent(onEventsCallback);
   client.onMessage(onMessageCallback);
   bool connected=client.connect(websockets_server,port,"/ws");
   Serial.println(connected);
   client.send("Hi Server!");
   client.ping();
}

void loop() {
  client.poll();
}
