#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define RELAY 7
#define TX 3
#define RX 2

String InputString = "";
boolean StringComplete = 0;
boolean SetValidData = 0; 

LiquidCrystal_I2C lcd(0x27,16,2);
//SoftwareSerial humiSerial(RX,TX);

void setup() {
  
  Serial.begin(115200);
  //pinMode(RELAY, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, LeeHJ");
  lcd.setCursor(2,1);
  lcd.print("HyeongJun Temp");
  delay(2000);
  lcd.clear();
}

void loop() {
  String strTemp = "";
  String strHumi = "";
  
  delay(1000);

  if(Serial.available())
  {
    strTemp = Serial.readString();
  }

  Serial.println((char*)strTemp.c_str());
  
  lcd.setCursor(0,0);
  lcd.print("Temp = ");
  lcd.print(strTemp.substring(0,5));
  lcd.print(" ");
  lcd.print(char(223));
  lcd.print("C ");
  lcd.setCursor(0,1);
  lcd.print("Humi = ");
  lcd.print(strTemp.substring(5,10));
  lcd.print(" % ");
  
  Serial.write((char*)strTemp.c_str());
}
