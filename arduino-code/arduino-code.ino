#include <Wire.h>

#include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd = LiquidCrystal_I2C (0x27,16,2);

#include <DS3231.h>
  DS3231 rtc(SDA, SCL);
  
#include <Servo.h>
  Servo myservo;
  
#define trig_pin 12
#define echo_pin 11

#define buzz_on digitalWrite(A2,HIGH);
#define buzz_off digitalWrite(A2,LOW);

#define push analogRead(A3)

float wlevel, wtemperature;

long waits = 0;

int relay_pin[5] = {8,7,6,5,4};
boolean rstatus[5] = {0,0,0,0,0};
int lstatus = 0;
boolean manual = false;

////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  Serial.println("\n================= START =================\n");

  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("HELLO");
  
  rtc.begin();
  Serial.println("Jam: " + String(rtc.getTimeStr()) + "\n");

  pinMode(echo_pin, INPUT);
  pinMode(trig_pin, OUTPUT);
  digitalWrite(trig_pin, LOW);

  pinMode(A3, INPUT);
  pinMode(A2,OUTPUT);
  buzz_off;
  
  for (int r = 0; r < 5; r++){
    pinMode(relay_pin[r],OUTPUT);
    relay(r,rstatus[r]);
  }

  pinMode(9,OUTPUT);
  myservo.attach(9);
  myservo.write(30);
  
  delay(1000);
}

void loop() {
  
  if (millis() > waits){
    lcd_show(lstatus);
    waits = millis() + 300;
  }
  
  button();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int button(){
  int result[2] = {0,0};
  int range[2][2] = {{950,1050}, {800,900}};
  int value = push;

  if (value > 500){
    long wait = millis() + 200;
    buzz_on;
    while (value > 500){
      result[0] = result[1];
      for (int s = 2; s > 0; s--)
        if (value > range[s-1][0] && value < range[s-1][1]) 
          result[1] = 3 - s;
      value = push;
      delay(10);
      if (millis() > wait) buzz_off;
    }
    buzz_off;
    delay(100);
  }
  if (result[0]) Serial.println(result[0]);

  if (!manual){
    if (result[0] == 1) {
      lstatus++;
      if (lstatus >= 9) lstatus = 0;
    }else
    if (result[0] == 2 && lstatus >= 3){
      manual = true;
      control_manual(lstatus);
    }
  }
  else return result[0];
}

void lcd_show(int value){
  if (value == 2) water_level();
   
  String line[9][2] = {
    { rtc.getDateStr(), rtc.getTimeStr() },           // 0
    { "Temperature", String(wtemperature,2) + " C" }, // 1
    { "Water Level", String(wlevel,1) + " cm" },      // 2
    { "Fan Cooler", rstatus[0] ? "ON" : "OFF" },      // 3
    { "Heater", rstatus[1] ? "ON" : "OFF" },          // 4
    { "LED", rstatus[2] ? "ON" : "OFF" },             // 5
    { "Water In", rstatus[3] ? "ON" : "OFF" },        // 6
    { "Water Out", rstatus[4] ? "ON" : "OFF" },       // 7
    { "Feeder", " "}                                  // 8
  };
    
  lcd.clear();
  for (int l = 0; l < 2; l++){
    lcd.setCursor(0,l);
    lcd.print(line[value][l]);
  }
}

void control_manual(int value){
  lcd.setCursor(15,1);
  lcd.print("M");
  int num = value - 3;
  while (manual){
    int a = button();
    if (a == 1){
      if (value == 8) feeder();
      else relay(num, 1-rstatus[num]);
      lcd_show(value);
      lcd.setCursor(15,1);
      lcd.print("M");
    }else if (a == 2) manual = false;
  }
}

void relay(int num, int value){
  digitalWrite(relay_pin[num], 1-value);
  rstatus[num] = value;
  Serial.println("Relay " + String(num) + " : " + String(rstatus[num] ? "ON" : "OFF") );
}

void feeder(){
  const int range[2] = {30,150};
  for (int r = range[0]; r <= range[1]; r++, delay(10)) myservo.write(r);
  delay(2000);
  for (int r = range[1]; r >= range[0]; r--, delay(10)) myservo.write(r);
  Serial.println("Feed!");
}

void water_level(){
  digitalWrite(trig_pin, LOW);
  delay(100);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  
  long duration = pulseIn(echo_pin, HIGH);
  wlevel = 22.5 - (duration * 0.034 / 2);
  if (wlevel < 0) wlevel = 0;
}
