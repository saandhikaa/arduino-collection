//update 25-01-2022

const int TotalButton = 4;        // Total Button
const int NButton[] = {7,6,5,4};  // Push Button Pin
const int PushState = 0;          // Push Button Active Low = 0, Active High = 1
const int PushLong = 3000;        // Long press timer = 3 sec.

void setup() {
  for (int s = 0; s < TotalButton; s++){
    pinMode(NButton[s],INPUT);
  }

  Serial.begin(9600);
  Serial.println("\nSTART================");
}

void loop() {
  int value = MultipleButton();
  if (value > 0){
    Serial.println("> Button " + String(value) + ": pressed!");
  }else if (value < 0){
    value = abs(value);
    Serial.println("Button " + String(value) + ": long pressed!");
  }
}


int MultipleButton(){
  int result = 0;
  for (int s = 0; s < TotalButton; s++){
    if (digitalRead(NButton[s]) == PushState){
      result = s+1;
      long waits = millis() + PushLong;
      while (abs(1 - PushState - digitalRead(NButton[s]))){
        if (millis() > waits){
          result = -abs(s+1);
          Serial.print("> ");
          break;
        }
      }
      while (abs(1 - PushState - digitalRead(NButton[s])));
      delay(100);
    }
  }
  return result;
}
