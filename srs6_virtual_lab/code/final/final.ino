#include "logic.h"

const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int BTN_PIN = 2;
const int RED_PIN = 13;
const int YELLOW_PIN = 12;
const int GREEN_PIN = 11;

const unsigned long ECHO_TIMEOUT_US = 30000;
const unsigned long DEBOUNCE_MS = 50;
const unsigned long SAMPLE_PERIOD_MS = 60;
const int WINDOW = 5;

int btnReading = HIGH;
int btnStable = HIGH;
unsigned long btnChangedMs = 0;

Sample window[WINDOW];
int filled = 0;
int head = 0;
unsigned long lastSampleMs = 0;

void showZone(Zone z) {
  digitalWrite(RED_PIN, z == ZONE_RED ? HIGH : LOW);
  digitalWrite(YELLOW_PIN, z == ZONE_YELLOW ? HIGH : LOW);
  digitalWrite(GREEN_PIN, z == ZONE_GREEN ? HIGH : LOW);
}

void resetWindow() {
  filled = 0;
  head = 0;
}

bool buttonPressed() {
  int raw = digitalRead(BTN_PIN);
  if (raw != btnReading) {
    btnReading = raw;
    btnChangedMs = millis();
  }
  if (millis() - btnChangedMs >= DEBOUNCE_MS && btnStable != btnReading) {
    btnStable = btnReading;
    Serial.println(btnStable == LOW ? "BTN: D2=LOW pressed" : "BTN: D2=HIGH released");
  }
  return btnStable == LOW;
}

unsigned long readEchoUs() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  return pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  showZone(ZONE_NONE);
  Serial.println("final start: timeout 30000us, validMeasurement, millis debounce, median of 5");
}

void loop() {
  if (!buttonPressed()) {
    showZone(ZONE_NONE);
    resetWindow();
    return;
  }

  if (millis() - lastSampleMs < SAMPLE_PERIOD_MS) return;
  lastSampleMs = millis();

  unsigned long us = readEchoUs();
  window[head] = makeSample(us);
  head = (head + 1) % WINDOW;
  if (filled < WINDOW) filled++;

  float median = 0;
  bool validMeasurement = filled == WINDOW && medianOfValid(window, WINDOW, &median);
  Zone z = classify(validMeasurement, median);
  showZone(z);

  Serial.print("raw_us=");
  Serial.print(us);
  Serial.print(" validMeasurement=");
  Serial.print(validMeasurement ? 1 : 0);
  if (validMeasurement) {
    Serial.print(" median_cm=");
    Serial.print(median);
  } else if (filled == WINDOW) {
    Serial.print(" ERROR: no valid echo, SAFE STOP");
  }
  Serial.print(" zone=");
  Serial.println(zoneName(z));
}
