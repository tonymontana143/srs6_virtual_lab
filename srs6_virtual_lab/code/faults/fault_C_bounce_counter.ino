const int BTN_PIN = 2;
const unsigned long DEBOUNCE_MS = 50;

int lastRaw = HIGH;
unsigned long rawEdges = 0;

int stableState = HIGH;
int lastReading = HIGH;
unsigned long lastChangeMs = 0;
unsigned long debouncedPresses = 0;

void setup() {
  Serial.begin(9600);
  pinMode(BTN_PIN, INPUT_PULLUP);
  Serial.println("fault_C: raw edges vs millis debounce");
}

void loop() {
  int raw = digitalRead(BTN_PIN);

  if (raw != lastRaw) {
    rawEdges++;
    lastRaw = raw;
  }

  if (raw != lastReading) {
    lastChangeMs = millis();
    lastReading = raw;
  }
  if (millis() - lastChangeMs > DEBOUNCE_MS && raw != stableState) {
    stableState = raw;
    if (stableState == LOW) {
      debouncedPresses++;
      Serial.print("press #");
      Serial.print(debouncedPresses);
      Serial.print(" raw edges total=");
      Serial.println(rawEdges);
    }
  }
}
