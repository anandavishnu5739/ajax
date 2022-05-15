#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> 
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#define trigger 4
#define echo 5
#define lid = 6 
ESP8266WebServer server;
String lat = "00.00"
String lng = "00.00"

char* ssid = "SSID";
char* password = "PASSWORD";

long duration=0;
float waterlevel= 0;

#include"index.h"
int value = 0;
Adafruit_ADS1115 ads(0x48);
void setup()
{
Serial.begin(9600); 
ads.begin();
pinMode(trigger, OUTPUT); 
pinMode(echo, INPUT);
pinMode(lid, INPUT);
 WiFi.begin(ssid,password);
   while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");

    delay(500);
  }
Serial.println("");
Serial.println(WiFi.localIP());

server.on("/",[](){server.send_P(200,"text/html", webpage);});

server.on("/gasNow",GasValue_handle();
server.on("/LocNow",Loc_handle();
server.on("/LevelNow",W_levelValue_handle();
server.on("/TempNow",TempValue_handle();
server.on("/LidNow",lid_handle());
server.begin();
 randomSeed(1);
}

void loop()
{

  server.handleClient();
  GasValue_handle();
  Loc_handle();
  W_levelValue_handle();
  TempValue_handle();
  lid_handle());

}

void GasValue_handle(){
  int16_t adc0;
  adc0 = ads.readADC_SingleEnded(0);
  String string_methane =  String(adc0);
  server.send(200,"text/plain", string_methane);
}

void TempValue_handle(){
  int16_t adc1;
  adc1 = ads.readADC_SingleEnded(1);
  String string_temp =  String(adc1);
  server.send(200,"text/plain", string_temp);
}


  void W_levelValue_handle(){
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);
    duration = pulseIn(echo, HIGH);
    float buffer_value= duration*0.034/2;
    float percent_buffer=1-buffer_value/170;
    waterlevel= percent_buffer*100;
    String string_waterlevel =  String(waterlevel);
    server.send(200,"text/plain", string_waterlevel);
  }


void lid_handle(){
  if (digitalRead(lid) == HIGH;){
    server.send(200,"text/plain", 'CLOSED');
  }
  else if (digitalRead(lid) == LOW;)
  {
    server.send(200,"text/plain", 'OPEN');
  }
  
}

  void Loc_handle(){
     String location =  lat + ',' + lng;
    server.send(200,"text/plain", location);
  }

