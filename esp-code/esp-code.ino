#define BLYNK_TEMPLATE_ID "TMPLZvb-ToMb"
#define BLYNK_DEVICE_NAME "Aquascape lalisa"
#define BLYNK_AUTH_TOKEN "Bu_UJF4gQD_YOrRHO7e59NzNmB9uJybi"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "daylily";
char pass[] = "midnight";

int count = 0;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  Blynk.run();
}

BLYNK_WRITE(V0) {
  int value = param.asInt();
  Blynk.virtualWrite(V6, count++);
  Serial.print(count);
  digitalWrite(LED_BUILTIN, 1 - value);
}
