#include <ESP8266WiFi.h>
//#include <WiFi.h>
#include <PubSubClient.h>

// Replace with your station credentials
const char* ssid = "JioFiber-AUKun";   //"ESP8266";
const char* password ="yahzad2Ioxagiela";// "123456789";

// Replace with your MQTT broker IP address
const char* mqttServer = /*"192.168.4.1";*/"test.mosquitto.org";
const int mqttPort = 1883;
const char* mqttTopic= "example_topic";
const char* mqttClientId = "esp8266_Station";

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  pinMode(LED_BUILTIN,OUTPUT);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT broker...");
    // Serial.println(client.connect(mqttClientId));
      client.subscribe(mqttTopic);
     // Serial.println(client.subscribe(mqttTopic));
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
  String Msg[7];int j=0;
  Serial.print("Message received on topic: ");
  Serial.println(topic);
 

  Serial.print("Message payload: ");
  for (int i = 0; i < length; i++,j++) 
  {
    Msg[j]=(char)payload[i];
    Serial.print((char)payload[i]);
    delay(500);
   // Serial.println(Msg[j]);
  }
  Msg[j]='\0';
  Serial.println(&Msg);
   if(Msg=="ledon")
  {
    digitalWrite(LED_BUILTIN,LOW);
    delay(1000);
    Serial.println("led is turned On on subscriber side");
    //delay(1000);
  }
  else if(Msg=="ledoff")
  {
    digitalWrite(LED_BUILTIN,HIGH);
    Serial.println("led is turned Off on subscriber side");
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
