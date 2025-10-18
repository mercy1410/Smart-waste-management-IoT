#define BLYNK_TEMPLATE_ID "TMPL3iSm6IAIy"
#define BLYNK_TEMPLATE_NAME "smart waste management system"
#define BLYNK_AUTH_TOKEN "IByveECJpoRwXZZ9Ku3_ixWO5e0yjiER"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>


char ssid[] = "ABCD";
char pass[] = "99999999";

#define TRIG_PIN 33
#define ECHO_PIN 32
#define DISTANCE_THRESHOLD 20  // cm

#define LED1 25
#define LED2 26
#define LED3 27

BlynkTimer timer;
bool alertSent = false;

void sendDistance() {
  long duration;
  float distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.0343) / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");

  Blynk.virtualWrite(V0, distance);

  // LED Logic
  if (distance >= 12) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    delay(200);
    digitalWrite(LED1, LOW);
    delay(200);
  } 
  else if (distance > 7) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);

  } else {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    
  }

  if (distance > 0 && distance < DISTANCE_THRESHOLD) {
    if (!alertSent) {
      Blynk.logEvent("distance_alert", "Object is too close!");
      alertSent = true;
    }
  } else {
    alertSent = false;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  Blynk.config(BLYNK_AUTH_TOKEN);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Blynk.connect();

  timer.setInterval(1000L, sendDistance);
}

void loop() {
  Blynk.run();
  timer.run();
}
