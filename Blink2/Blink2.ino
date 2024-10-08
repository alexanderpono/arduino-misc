const int LED = 9;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void doBlink1(int led) {
  turnOn(led);
  delay(1000);            
  turnOff(led);
  delay(1000);            
}

void turnOn(int led) {
  digitalWrite(led, HIGH);
}

void turnOff(int led) {
  digitalWrite(led, LOW);
}

void doBlink2() {
  const int MAX_LIGHT = 120;

  turnOn(LED_BUILTIN);
  for (int i=0; i<=MAX_LIGHT; i++) {
    analogWrite(LED, i); 
    delay(10);
  }

  turnOff(LED_BUILTIN);
  for (int i=MAX_LIGHT; i>=0; i--) {
    analogWrite(LED, i); 
    delay(10);
  }
  
  delay(600);
}

void loop() {
  doBlink1(LED_BUILTIN);
}
