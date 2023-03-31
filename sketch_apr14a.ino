#include <DHT.h>
#include <Adafruit_Sensor.h>
#include "ThingSpeak.h"
#include "DHT.h"
#include <ESP8266WiFi.h>

String apiKey = "X3OG9MOVY6BDBAWR";       // Enter your Write API key from ThingSpeak
const char* ssid = "WELCOME TO VNU-IS";           // Give your wifi network name
const char* password = "";   // Give your wifi network password
const char* server = "api.thingspeak.com";
#define DHTPIN 0      //pin D0 where the dht11 is connected
DHT dht(DHTPIN, DHT11);
WiFiClient client;
int BUZZER=D6;

void setup() 
{
  pinMode(16, OUTPUT);  //pin D0 where the dht11 is connected
  pinMode(5, OUTPUT);   //pin D1 where the dht11 is connected
  pinMode(4, OUTPUT);   //pin D2 where the dht11 is connected
  pinMode(BUZZER,OUTPUT);
  
       Serial.begin(115200);
       delay(10);
       dht.begin();
       Serial.println("Connecting to ");
       Serial.println(ssid);
       WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi is connected");
      
  
}

void loop() 
{
      float h = dht.readHumidity();
      float t = dht.readTemperature();
             /* if (isnan(h) || isnan(t)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                      delay(5000);
                 }
*/                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {    
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr += "\r\n\r\n";

                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);

                             Serial.print("Temperature: ");
                             Serial.print(t);
                             Serial.print("°C, Humidity: ");
                             Serial.print(h);
                             Serial.println("%. Send to Thingspeak.");
  
  if(t <= 0){
 digitalWrite(16,HIGH);
 digitalWrite(5,LOW);
 digitalWrite(4,LOW);
  noTone(BUZZER);     // Stop sound...
  //delay(1000);        // ...for 1sec
 }
  if( 0 < t && t < 26 ){ 
 digitalWrite(16,LOW);
 digitalWrite(5,HIGH);
 digitalWrite(4,LOW);
  tone(BUZZER, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(BUZZER);     // Stop sound...
  delay(1000);        // ...for 1sec
 }
 if(t >= 26){ 
 digitalWrite(16,LOW);
 digitalWrite(5,LOW);
 digitalWrite(4,HIGH);
 
  //tone(BUZZER, 6000);
  digitalWrite(BUZZER,HIGH);
 }
 }
   
                 client.stop();

          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(5000);
}
