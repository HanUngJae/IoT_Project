#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

//int trigPin = D4;   // 초음파센서 출력핀
//int echoPin = D5;   // 초음파센서 입력핀
int relay = D3;     // 릴레이모듈 출력핀
int red_led = D4;   // red_led 출력 --> Relay OFF
int green_led = D5; // green_led 출력 --> Relay ON Timer ON
int blue_led = D6;  // blue_led 출력 --> Relay ON Timer OFF

const char* ssid = "HUJ";
const char* password = "159753asd";
const char* mqtt_server = "192.168.0.80";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
float distance = 0;
int multitabstate = 0;
int timestate = 0;
char get_data[3];

int temp = 0;

StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

void setup_wifi() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  temp = temp + 1;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  String topic_a = topic;
  int index = 0;
  for(int i = 0; i < length; i++) {
    if((char)payload[i] != '?') {
      get_data[index] = (char)payload[i];
      index = index + 1;
    }
  }
  if(topic_a.equals("multitab")) {
    // Relay state change OFF
    if(get_data[0] == '0') {
      digitalWrite(relay, LOW); //Relay OFF
      Serial.println("Relay OFF");
      multitabstate = 0;
      if(get_data[1] == '0') {
        digitalWrite(red_led, LOW);
        digitalWrite(green_led, LOW);
        digitalWrite(blue_led, LOW);
        timestate = 0;
      }
      else if(get_data[1] == '1') {
        digitalWrite(blue_led, HIGH);
        digitalWrite(red_led, LOW);
        digitalWrite(green_led, LOW);
        Serial.println("Timer set ON");
        timestate = 1;
      }
    }
    // Relay state change ON
    else if (get_data[0] == '1') {
      digitalWrite(relay, HIGH); //Relay ON
      Serial.println("Relay ON");
      multitabstate = 1;
      Serial.println(get_data[1]);
      if (get_data[1] == '0') {
        digitalWrite(red_led, HIGH);
        digitalWrite(blue_led, LOW);
        digitalWrite(green_led, LOW);
        Serial.println("Timer set OFF");
        timestate = 0;
      }
      else if(get_data[1] == '1') {
        if (get_data[2] == '0') {
          // 마젠타(분홍색 계열)
          analogWrite(red_led, 255);
          analogWrite(green_led, 0);
          analogWrite(blue_led, 255);
          Serial.println("Timer ready");
        }
        else if (get_data[2] == '1') {
          digitalWrite(green_led, HIGH);
          digitalWrite(red_led, LOW);
          digitalWrite(blue_led, LOW);
          Serial.println("Timer ~ing");
        }
        timestate = 1;
      }
    }
  }
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
      Serial.println(client.connect(clientId.c_str()));
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("multitap");
      //Serial.print("temp: ");
      //Serial.println(temp);
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
  pinMode(relay, OUTPUT);
  //pinMode(trigPin, OUTPUT);   // 사용 X
  //pinMode(echoPin, INPUT);    // 사용 X
  pinMode(red_led, OUTPUT);   // red_led 출력 --> Relay OFF
  pinMode(green_led, OUTPUT); // green_led 출력 --> Relay ON Timer ON
  pinMode(blue_led, OUTPUT);  // blue_led 출력 --> Relay ON Timer OFF
  Serial.begin(115200);
  //digitalWrite(relay, HIGH);
  Serial.println("이곳 들어오기는 하니???");
  temp = temp + 1;
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  digitalWrite(relay, LOW);
  digitalWrite(red_led, HIGH);
  delay(1000);
  digitalWrite(red_led, LOW);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    ++value;
    root["multitabState"] = multitabstate;
    root["multitabTime"] = timestate;
    root.printTo(msg);
    //snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("/shc/multi", msg);
    client.subscribe("multitab");
  }
}
