// by this code we can make ESP8266 to work as an publisher in a MQTT protocol
#include <ESP8266WiFi.h>
//#include<WiFi.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>

// Replace with your publisher client credentials
String message;
const char* mqttServer = "test.mosquitto.org"; // this was a public server which acts as broker
const int mqttPort = 1883;
const char* mqttTopic= "example_topic";  //Topic name should be common for both Publisher and subscriber
const char* mqttClientId = "esp8266_Accesspoint";  //client name should be unnique for each device

// Replace with your station credentials
const char* ssid = "JioFiber-AUKun";   
const char* password ="yahzad2Ioxagiela"; 

WiFiServer server(80);
WiFiClient espClient;
PubSubClient client(espClient);


void setup() 
{
   
     Serial.begin(115200);
     WiFi.mode(WIFI_STA); // making esp8266 as a station
     WiFi.begin("JioFiber-AUKun", "yahzad2Ioxagiela");
     pinMode(LED_BUILTIN,OUTPUT);

      // checking esp8266(nodemcu) whether it was connected to wifi or not
      while(WiFi.status()!= WL_CONNECTED ) 
      {
        delay(500);
        Serial.print("..");
      }
         Serial.println("WiFi Module Started");
         Serial.print("ESP01 IP adress is: ");
         Serial.println(WiFi.localIP());
         server.begin();
         // making connection between publisher(esp8266) and broker(test.mosquitto.org)    
         client.setServer(mqttServer, mqttPort);
         Serial.println(mqttServer);
         Serial.println(client.connected());
         Serial.println(client.connect(mqttClientId));
        while (!client.connected()) 
        { 
         Serial.println("Connecting to MQTT broker...");

        if (client.connect(mqttClientId))
        {
          Serial.println("Connected to MQTT broker");
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
   int i=0;
   
      delay(2000);

       LedOn();
       delay(1000);
       LedOff();
       delay(2000);
}
void LedOn()
{
   digitalWrite(LED_BUILTIN,LOW);    //making inbuilt has low because it was active low(high when i/p=0)
   message="ledon"; 
   client.publish(mqttTopic, message.c_str());   //adding ledon payload to topic to publish to broker
   Serial.println("Message sent: " + message);
   delay(1000);
}
void LedOff()
{ 
   digitalWrite(LED_BUILTIN,HIGH);    //making inbuilt has high because it was active low
   message="ledoff"; 
   client.publish(mqttTopic, message.c_str());   //adding ledoff payload to topic to publish to broker
   Serial.println("Message sent: " + message);
   delay(1000);
}
