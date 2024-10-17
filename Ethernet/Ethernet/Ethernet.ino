#include "DHT.h"
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //Setting MAC Address
 
#define DHTPIN 2
DHT dht(DHTPIN, DHT11);
 
float humidityData;
float temperatureData;
bool ethernetOk = false; 
 
char server[] = "192.168.10.111";
EthernetClient client; 
 
void setup() {
  Serial.begin(9600);
  dht.begin();
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  } else {
    Serial.println("Configure Ethernet using DHCP - OK");
    ethernetOk = true;
  }
  delay(1000);
}
//------------------------------------------------------------------------------
 
 
void loop(){
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Ошибка считывания");
    return;
  }

  if (ethernetOk) {
    sendTelemetryToDB(h, t);
  }
}
 
  void sendTelemetryToDB(float h, float t)
 {
   if (client.connect(server, 80)) {

    Serial.print("GET /api/add.php?id=1&h=");
    client.print("GET /api/add.php?id=1&h=");
    
    Serial.print(h);
    client.print(h);
    
    client.print("&t=");
    Serial.print("&t=");
    
    client.println(t);
    Serial.println(t);
    
//    Serial.print(" ");
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    
//    Serial.print("HTTP/1.1");
    client.print("HTTP/1.1");

//    Serial.println();
    client.println();
    
//    Serial.println("Host: 192.168.10.111");
    client.println("Host: 192.168.10.111");
    
//    Serial.println("Connection: close");
    client.println("Connection: close");
    
//    Serial.println();
    client.println();
  } else {
    Serial.println("connection failed");
  }
 }
