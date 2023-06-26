#include <ESP8266WiFi.h>
//#include<WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>

// Replace with your access point credentials


  String message;
// Replace with your MQTT broker IP address
const char* mqttServer = "test.mosquitto.org"; /* "192.168.4.2"*/;
const int mqttPort = 1883;
const char* mqttTopic= "example_topic";
//const char* mqttTopicLedOff = "ledoff";
const char* mqttClientId = "esp8266_Accesspoint";


WiFiServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);


void setup() 
{
   
     Serial.begin(115200);
     WiFi.mode(WIFI_STA);
     WiFi.begin("JioFiber-AUKun", "yahzad2Ioxagiela");
     pinMode(LED_BUILTIN,OUTPUT);

     //WiFi.softAP("ESP8266","123456789");
   //  Serial.println("..");
      while(WiFi.status()!= WL_CONNECTED )
      {
        delay(500);
        Serial.print("..");
      }
       Serial.println("WiFi Module Started");
       Serial.print("ESP01 IP adress is: ");
       Serial.println(WiFi.localIP());
       server.begin();
     //Serial.println(WiFi.softAPIP());
 

      client.setServer(mqttServer, mqttPort);
     Serial.println(mqttServer);
    // Serial.println( client.setServer(mqttServer, mqttPort));
     Serial.println(client.connected());
      Serial.println(client.connect(mqttClientId));
   while (!client.connected()) 
   {
     Serial.println("1");
    Serial.println("Connecting to MQTT broker...");

    if (client.connect(mqttClientId))
    {
      Serial.println("Connected to MQTT broker");
      Serial.println("2");
    }
    else
    {
      Serial.println("3");
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
 
}

void loop()
{
  // = "Hello, MQTT!";
   int i=0;
   digitalWrite(LED_BUILTIN,HIGH);
   delay(1000);
   while(i<=10)
    {
       LedOn();
       delay(3000);
      //mqttTopic="ledoff";
        LedOff();
        delay(2000);
       i++;
    }
     digitalWrite(LED_BUILTIN,LOW);
     delay(5000);
}
void LedOn()
{
   message="ledon"; 
   client.publish(mqttTopic, message.c_str());
   Serial.println("Message sent: " + message);
}
void LedOff()
{ 
   message="ledoff"; 
   client.publish(mqttTopic, message.c_str());
   Serial.println("Message sent: " + message);
}
