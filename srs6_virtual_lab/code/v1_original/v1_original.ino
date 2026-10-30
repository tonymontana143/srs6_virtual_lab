const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int BTN_PIN = 2;
const int RED_PIN = 13;
const int YELLOW_PIN = 12;
const int GREEN_PIN = 11;

void allOff() {
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  allOff();
  Serial.println("v1_original start");
}

void loop() {
  if (digitalRead(BTN_PIN) == HIGH) {
    allOff();
    Serial.println("D2=HIGH idle, LED off");
    delay(200);
    return;
  }

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH);

  if (duration == 0) {
    allOff();
    Serial.println("D2=LOW ERROR: no echo (pulseIn=0), LED off");
    delay(200);
    return;
  }

  float distance = duration * 0.0343 / 2;

  allOff();
  if (distance < 20) {
    digitalWrite(RED_PIN, HIGH);
  } else if (distance <= 50) {
    digitalWrite(YELLOW_PIN, HIGH);
  } else {
    digitalWrite(GREEN_PIN, HIGH);
  }

  Serial.print("D2=LOW duration=");
  Serial.print(duration);
  Serial.print("us distance=");
  Serial.print(distance);
  Serial.println("cm");
  delay(200);
}
