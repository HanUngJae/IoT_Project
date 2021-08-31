 #include <Wire.h>
 //#include <PubSubClient.h>
 #include <SoftwareSerial.h>
 #include <DHT.h>
 
 #define DHTTYPE DHT22
 #define DHTPIN 7
 #define RELAY 6
 #define TX 2
 #define RX 3

 int heaterPower = 1;
 int state = 0;
 int setTemp = 28;

 unsigned long lastMsg = 0;
 
 DHT dht(DHTPIN, DHTTYPE);
 
 void setup() {
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  pinMode(RELAY, OUTPUT);
  pinMode(13, OUTPUT);
  
  dht.begin(); //LCD 초기화
}

void loop() {
  delay(2000);

  float humi = dht.readHumidity();
  float temp = dht.readTemperature();
  String strTemp = String(temp) + String(humi);


  if(isnan(humi) || isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  //float hic = dht.computeHeatIndex(temp, humi, false);

  Serial.write((char*)strTemp.c_str());

  if(temp < setTemp) //히터 on
  {
    digitalWrite(RELAY,HIGH);
  }
  else //히터 off
  {
    digitalWrite(RELAY,LOW);
  }
}
