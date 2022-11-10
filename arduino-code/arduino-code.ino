#include <SoftwareSerial.h>
  SoftwareSerial esp(2,3);

void setup() {
  Serial.begin(9600);
  Serial.println("START");
  
  esp.begin(115200);
}

void loop() {
  if (esp.available()){
    int data = esp.parseInt();
    data += 1;
    Serial.println(data);
  }
}
