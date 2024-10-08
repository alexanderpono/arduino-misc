const int LED = 9;
const int BUTTON = 2;
const int POT = 0;
#define CALLBACKS_SIZE 5

// SHARED STATE OF PROCESSES
int _doBlink_light = 0;
int _printLight_lastPrintedLight = -1;
int _printPot = -1;
int _printBut = -1;

void doNothing() {}

int _but_lastPrintedValue = -1;
const int BUT_DEBOUNCE_WAIT = 10;
int _butDebounce = 0;
void doButtonLogic() {
  int butState = digitalRead(BUTTON);
  if (butState == _but_lastPrintedValue) {
    _butDebounce == BUT_DEBOUNCE_WAIT;
  } else {
    if (_butDebounce > 0) {
      _butDebounce--;
      return;
    }
  }


  if (butState != _but_lastPrintedValue && _butDebounce == 0) {
    Serial.print("B=");
    if (butState == LOW) {
      _but_lastPrintedValue = 0;
    } else {
      _but_lastPrintedValue = 1;
    }
    Serial.println(_but_lastPrintedValue);
  }
  
  if (butState == LOW) {
    turnOff(LED_BUILTIN);
    _printBut = 0;
  } else {
    turnOn(LED_BUILTIN);
    _printBut = 1;
  }
}

void doEcho() {
  char data;
  if (Serial.available() > 0) {
    data = Serial.read();
    Serial.print(data);
  }
}

void printAll() {
  if (((_doBlink_light % 75) == 0) && (_printLight_lastPrintedLight != _doBlink_light)) {
    Serial.print("L=");
    Serial.print(_doBlink_light);
    Serial.print(" P=");
    Serial.print(_printPot);
    Serial.print(" B=");
    Serial.println(_printBut);
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

int _pot_lastPrintedValue = -1;
void doPot() {
  int val = analogRead(POT);
  int per = map(val, 0, 1023, 0, 100);

  if (abs(_pot_lastPrintedValue - per) > 1) {
    Serial.print("P=");
    Serial.println(per);
    _pot_lastPrintedValue = per;
  }  
  _printPot = per;
}


void (*empty[CALLBACKS_SIZE])() = {doNothing, doNothing, doNothing, doNothing, doNothing};
void (*button[CALLBACKS_SIZE])() = {doButtonLogic, doNothing, doNothing, doNothing, doNothing};
void (*buttonAndEcho[CALLBACKS_SIZE])() = {doButtonLogic, doEcho, doNothing, doNothing, doNothing};
void (*buttonAndEchoAndPrintLight[CALLBACKS_SIZE])() = {doButtonLogic, doEcho, printAll, doNothing, doNothing};
void (*buttonAndEchoAndPrintLightAndBlink[CALLBACKS_SIZE])() = {doButtonLogic, doEcho, printAll, doBlink, doNothing};
void (*process12345[CALLBACKS_SIZE])() = {doButtonLogic, doEcho, printAll, doBlink, doPot};



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
  runCallbacks(process12345);
  delay(10);
}
