#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define LED_PIN1 2
#define LED_PIN2 3

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(111, 111, 111, 111); //Enter arduino server IP Address
IPAddress server(111, 111, 111, 111); //Enter mqtt server IP Address

const char *mqtt_username = "bedroom";
const char *mqtt_password = "bedroom";

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);

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
      client.publish("kitchenroom", "Light On");
    }

    //turn the LED OFF if the payload is '0' and publish to the MQTT server a confirmation message
    if (value == 0) {
      digitalWrite(LED_PIN1, LOW);
      client.publish("kitchenroom", "Light Off");
    }
  }
  if (strcmp(sensor, "led2") == 0) {
    //turn the LED ON if the payload is '1' and publish to the MQTT server a confirmation message
    if (value == 1) {
      digitalWrite(LED_PIN2, HIGH);
      client.publish("kitchenroom", "Light On");
    }

    //turn the LED OFF if the payload is '0' and publish to the MQTT server a confirmation message
    if (value == 0) {
      digitalWrite(LED_PIN2, LOW);
      client.publish("kitchenroom", "Light Off");
    }
  }
} // void callback


void setup()
{
  Serial.begin(9600);
  pinMode(LED_PIN1, OUTPUT);
  digitalWrite(LED_PIN1, LOW);
  pinMode(LED_PIN2, OUTPUT);
  digitalWrite(LED_PIN2, LOW);

  Ethernet.begin(mac, ip);
  if (client.connect("arduinoClient", mqtt_username, mqtt_password)) {
    client.publish("kitchenroom", "Arduino kitchen");
        client.subscribe("kitchenroom");
  }
}

void loop()
{
  client.loop();
}
