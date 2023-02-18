const int LED = 9;
const int BUTTON = 2;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void turnOn(int led) {
  digitalWrite(led, HIGH);
}

void turnOff(int led) {
  digitalWrite(led, LOW);
}

void doBlink(void (*callback)()) {
  const int MAX_LIGHT = 150;

  for (int i=0; i<=MAX_LIGHT; i++) {
    analogWrite(LED, i);
    callback();
    delay(10);
  }

  for (int i=MAX_LIGHT; i>=0; i--) {
    analogWrite(LED, i); 
    callback();
    delay(10);
  }
}

void doButtonLogic() {
  if (digitalRead(BUTTON) == LOW) {
    turnOff(LED_BUILTIN);
  } else {
    turnOn(LED_BUILTIN);
  }
}

void doNothing() {}

void loop() {
  doBlink(doButtonLogic);
//  doBlink(doNothing);
}
