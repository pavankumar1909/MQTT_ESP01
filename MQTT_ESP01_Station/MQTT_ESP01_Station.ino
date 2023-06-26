// by this code we can make ESP8266 to work as an subscriber in a MQTT protocol
 
#include <ESP8266WiFi.h>
//#include <WiFi.h>
#include <PubSubClient.h>

// Replace with your station credentials
const char* ssid = "JioFiber-AUKun";   
const char* password ="yahzad2Ioxagiela";

// Replace with your MQTT broker IP address
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttTopic= "example_topic";    //Topic name should be common for both Publisher and subscriber
const char* mqttClientId = "esp8266_Station";    //client name should be unnique for each device

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(LED_BUILTIN,OUTPUT);

  // checking esp8266(nodemcu) whether it was connected to wifi or not
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  // making connection between subscriber(esp8266) and broker(test.mosquitto.org)  
  client.setServer(mqttServer, mqttPort);
  
  client.setCallback(callback);   

  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT broker...");
      client.subscribe(mqttTopic);
    if (client.connect(mqttClientId))
    {
      Serial.println("Connected to MQTT broker");
      client.subscribe(mqttTopic);
    }
    else
    {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
  
}


void callback(char* topic, byte* payload, unsigned int length) {
  String Msg="";
  Serial.print("Message received on topic: ");
  Serial.println(topic);
 

  Serial.print("Message payload: ");
  for (int i = 0; i < length; i++) 
  {
    Msg+=(char)payload[i];
    Serial.print((char)payload[i]);
    delay(500);
  }
  Serial.println(" ");
   for (int i = 0; Msg[i]!='\0'; i++) 
  {
    Serial.print(Msg[i]);
  }
   if(Msg=="ledon")    
  {
    digitalWrite(LED_BUILTIN,LOW);
    delay(1000);
    Serial.println("-led is turned On on subscriber side");
  }
  else if(Msg=="ledoff")
  {
    digitalWrite(LED_BUILTIN,HIGH);
    Serial.println("-led is turned Off on subscriber side");
    delay(1000);
  }
  Serial.println("---------");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");

    if (client.connect(mqttClientId)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(mqttTopic);
    }
    else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      delay(5000);
    }
  }
}
