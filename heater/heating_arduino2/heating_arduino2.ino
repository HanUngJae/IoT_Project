#define RELAY 7
int state = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RELAY, OUTPUT);
}

void loop() {
  if(Serial.available()) 
  {
    state = Serial.read();
  }
  
  if(state == 1)
  {
    digitalWrite(RELAY, HIGH);
  }
  else
  {
    digitalWrite(RELAY, LOW);
  }
}
