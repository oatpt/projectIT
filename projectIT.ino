#include <WiFi.h>
#include "PubSubClient.h"
 
#include <String>
const char* ssid = "KMITL-WIFI"; 
const char* password = "";

const char* mqtt_broker = "10.3.0.21";           //IP mqtt server
const char* mqtt_username = "admin";              //mqtt username ตามที่แจกให้
const char* mqtt_password = "projectit";     //mqtt password  ตามที่แจกให้
const int mqtt_port = 1883;

WiFiClient   espClient;
PubSubClient client(espClient);             //สร้างออปเจ็ค สำหรับเชื่อมต่อ mqtt

String text;
void callback(char *topic, byte *payload, unsigned int length) {
  text="";
  for (int i = 0; i < length; i++) 
     text+=(char) payload[i];
  if(strcmp(topic,"projectit/F2/202")==0)
  {

    if(text.compareTo("true")==0)
      
      digitalWrite(4,HIGH);
    else
      digitalWrite(4,LOW);
      
  }
  

}

void reconnect() { 

      client.setServer(mqtt_broker, mqtt_port);            //set mqtt server ตามIPและportที่ตั้งไว้

      client.setCallback(callback);                              //เลือกฟังก์ชั่นsubscribeจากหน้า12

      while (!client.connected())                                //รอจนกว่าจะเชื่อมต่อmqttสำเร็จ

      {

            String client_id = "esp32-nut-";

            client_id += String(WiFi.macAddress());

            Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());

            

            if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))

                  Serial.println("Public emqx mqtt broker connected");

            else {

                  Serial.print("failed with state ");

                  Serial.print(client.state());

                  delay(2000);

}     }     }


void setup_wifi() { 

      WiFi.mode(WIFI_STA); 

      WiFi.begin(ssid, password); 

      while (WiFi.status() != WL_CONNECTED) {

            delay(500);

            Serial.println("Connecting to WiFi...");

      }

      Serial.println("WiFi connected");

}

void setup() {
  // put your setup code here, to run once:
  pinMode(4,OUTPUT);
  Serial.begin(115200);
  Serial.print("nut");
  setup_wifi();

reconnect();                               //เชื่อมต่อmqtt

client.subscribe("projectit/F2/202");       //กำหนด topic ที่จะ subscribe

}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();
}
