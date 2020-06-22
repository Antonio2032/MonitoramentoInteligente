
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
// Update these with values suitable for your network.

char* posicao = "";

const char* ssid = "Douglas";
const char* password = "Morrison12345";
const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* posicaoPlaca = "(x:30, y:49)";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
int dataResp;
char msg[50];
int value = 0;
String inString = "";
int data;
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  char* inString="";
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    inString+=(char)payload[i];
  }
  Serial.println();

  posicao = inString;

  Serial.println(posicao);


}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("mapa/calor/pir/sensor", "ESP Conectado");
      // ... and resubscribe
      client.subscribe("mapa/calor/pir/sensor");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(16, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  
  client.loop();

  long now = millis();
    
 if (now - lastMsg > 2000) {
  lastMsg = now;
  ++value;
  Serial.print("PIR: ");
  Serial.println(posicaoPlaca);
  client.publish("mapa/calor/pir/sensor", posicaoPlaca);
  digitalWrite(16, 1);  
 }

}
