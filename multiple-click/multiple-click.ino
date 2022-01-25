//update 25-01-2022

const int PushPin = 2;      // Push Button Pin
const int PushState = 0;    // Push Button Active Low = 0, Active High = 1
const int PushLong = 3000;  // Long press timer = 3 sec.

void setup() {
  pinMode(PushPin,INPUT);
  Serial.begin(9600);
  Serial.println("\nSTART ================");
}

void loop() {
  int value = ButtonMultiClick(PushPin);

  if (value > 0){
    if (value == PushLong){
      Serial.println("Long Pressed");
    }
    else{
      Serial.println("> Pressed " + String(value) + " time(s)"); 
    }
  }
  
}


int ButtonMultiClick(int Button){
  int result = 0;
  boolean multiple = true;

  if (digitalRead(Button) == PushState){
    result = 1;
    long waits = millis() + PushLong;
    while (abs(1-PushState-digitalRead(Button))){
      if (millis() > waits){
        Serial.print("> ");
        result = PushLong;
        multiple = false;
        break;
      }
    }
    while (abs(1-PushState-digitalRead(Button)));
    delay(100);

    for (int s = 1;multiple;s++){
      waits = millis() + 1000;
      while (millis() <= waits){
        if (digitalRead(Button) == PushState){
          while (abs(1-PushState-digitalRead(Button)));
          delay(100);
          result++;
          break;
        }
      }
      if (result == s){
        multiple = false;
      }
    }
  }
  
  return result;
}
