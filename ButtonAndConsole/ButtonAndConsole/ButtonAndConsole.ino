const int LED = 9;
const int BUTTON = 2;
#define CALLBACKS_SIZE 4

void doNothing() {}

void doButtonLogic() {
  if (digitalRead(BUTTON) == LOW) {
    turnOff(LED_BUILTIN);
  } else {
    turnOn(LED_BUILTIN);
  }
}

void doEcho() {
  char data;
  if (Serial.available() > 0) {
    data = Serial.read();
    Serial.print(data);
  }
}

// SHARED STATE OF PROCESSES
int _doBlink_light = 0;


int _printLight_lastPrintedLight = -1;
void printLight() {
  if (((_doBlink_light % 75) == 0) && (_printLight_lastPrintedLight != _doBlink_light)) {
    Serial.print("printLight() light=");
    Serial.println(_doBlink_light);
    _printLight_lastPrintedLight = _doBlink_light;
  }
}

bool _doBlink_lighter = true;
void doBlink() {
  const int MAX_LIGHT = 150;

  setLight(_doBlink_light);
  if (_doBlink_lighter) {
    _doBlink_light++;
    if (_doBlink_light > MAX_LIGHT) {
      _doBlink_light = MAX_LIGHT;
      _doBlink_lighter = false;
    }
    return;
  }

  if (!_doBlink_lighter) {
    _doBlink_light--;
    if (_doBlink_light < 0) {
      _doBlink_light = 0;
      _doBlink_lighter = true;
    }
  }
}


void (*empty[CALLBACKS_SIZE])() = {doNothing, doNothing, doNothing};
void (*button[CALLBACKS_SIZE])() = {doButtonLogic, doNothing, doNothing, doNothing};
void (*buttonAndEcho[CALLBACKS_SIZE])() = {doButtonLogic, doEcho, doNothing, doNothing};
void (*buttonAndEchoAndPrintLight[CALLBACKS_SIZE])() = {doButtonLogic, doEcho, printLight, doNothing};
void (*buttonAndEchoAndPrintLightAndBlink[CALLBACKS_SIZE])() = {doButtonLogic, doEcho, printLight, doBlink};



void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
}

void setLight(int light) {
    analogWrite(LED, light);
}

void turnOn(int led) {
  digitalWrite(led, HIGH);
}

void turnOff(int led) {
  digitalWrite(led, LOW);
}


void runCallbacks(void (*callbacks[CALLBACKS_SIZE])()) {
  for (int i=0; i<CALLBACKS_SIZE; i++) {
    void (*callback)() = callbacks[i];
    callback();
  }
}


void loop() {
  runCallbacks(buttonAndEchoAndPrintLightAndBlink);
  delay(10);
}
