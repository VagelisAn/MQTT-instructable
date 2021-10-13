#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <ArduinoJson.h>

int LED_PIN1 = D8;
int LED_PIN2 = D2;

// WiFi
const char *ssid = ""; // Enter your WiFi name
const char *password = "";  // Enter WiFi password



// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(111, 111, 111, 111); //Enter arduino server IP Address
IPAddress server(111, 111, 111, 111); //Enter mqtt server IP Address

const char *mqtt_username = "bedroom"; 
const char *mqtt_password = "bedroom";

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient client(server, 1883, callback,espClient);

// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
  char str[length + 1];
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  int i = 0;
  for (i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    str[i] = (char)payload[i];

  }
  str[i] = 0; // Null termination
  Serial.println();

  StaticJsonDocument <256> doc;

  deserializeJson(doc, str);
  const char* sensor = doc["sensor"];
  int value = doc["value"];

  Serial.print("Sensor = ");
  Serial.println(sensor);
  Serial.print("Value = ");
  Serial.println(value);
  if (strcmp(sensor, "led1") == 0) {
    //turn the LED ON if the payload is '1' and publish to the MQTT server a confirmation message
    if (value == 1) {
      digitalWrite(LED_PIN1, HIGH);
      client.publish("bedroom", "Light On");
    }

    //turn the LED OFF if the payload is '0' and publish to the MQTT server a confirmation message
    if (value == 0) {
      digitalWrite(LED_PIN1, LOW);
      client.publish("bedroom", "Light Off");
    }
  }
  if (strcmp(sensor, "led2") == 0) {
    //turn the LED ON if the payload is '1' and publish to the MQTT server a confirmation message
    if (value == 1) {
      digitalWrite(LED_PIN2, HIGH);
      client.publish("bedroom", "Light On");
    }

    //turn the LED OFF if the payload is '0' and publish to the MQTT server a confirmation message
    if (value == 0) {
      digitalWrite(LED_PIN2, LOW);
      client.publish("bedroom", "Light Off");
    }
  }
} // void callback


void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN1, OUTPUT);
//  digitalWrite(LED_PIN1, LOW);
  pinMode(LED_PIN2, OUTPUT);
//  digitalWrite(LED_PIN2, LOW);

  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
    }
  if (client.connect("arduinoClient", mqtt_username, mqtt_password)) {   //
    client.publish("bedroom", "Arduino bedrom");
        client.subscribe("bedroom");
  }
}

void loop()
{
  client.loop();
}
