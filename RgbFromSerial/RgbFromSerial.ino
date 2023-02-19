const int LED = 6;
const int BUTTON = 2;
const int POT = 0;
#define CALLBACKS_SIZE 5

#define RGB_RED   11
#define RGB_GREEN 10
#define RGB_BLUE   9

int _rgbR = 50;
int _rgbG = 50;
int _rgbB = 50;


void doNothing() {}

void do1_buttonLogic() {
  if (digitalRead(BUTTON) == LOW) {
    turnOff(LED_BUILTIN);
  } else {
    turnOn(LED_BUILTIN);
  }
}

void do2_echo() {
  char data;
  if (Serial.available() > 0) {
    data = Serial.read();
    Serial.print(data);
  }
}

// SHARED STATE OF PROCESSES
int _doBlink_light = 0;


int _printLight_lastPrintedLight = -1;
void do3_printLight() {
  if (((_doBlink_light % 75) == 0) && (_printLight_lastPrintedLight != _doBlink_light)) {
    Serial.print("LIGHT: ");
    Serial.println(_doBlink_light);
    _printLight_lastPrintedLight = _doBlink_light;
  }
}

bool _doBlink_lighter = true;
void do4_blink() {
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

int _printPot_lastPrintedValue = -1;
void do5_pot() {
  int val = analogRead(POT);
  int per = map(val, 0, 1023, 0, 100);

  if (((_doBlink_light % 75) == 0) && (abs(_printPot_lastPrintedValue - per) > 1)) {
    Serial.print("POT: ");
    Serial.print(val);
    Serial.print(" / ");
    Serial.print(per);
    Serial.println("%");
    _printPot_lastPrintedValue = per;
  }  
}

void do6_rgbFromSerial() {
  
  while (Serial.available() > 0)
  {
    _rgbR = Serial.parseInt();
    _rgbG = Serial.parseInt();
    _rgbB = Serial.parseInt();

    if (Serial.read() == '\n')
    {
      analogWrite(RGB_RED, _rgbR);
      analogWrite(RGB_GREEN, _rgbG);
      analogWrite(RGB_BLUE, _rgbB);
    }
  }

}

void (*empty[CALLBACKS_SIZE])() = {doNothing, doNothing, doNothing, doNothing, doNothing};
void (*do1[CALLBACKS_SIZE])() = {do1_buttonLogic, doNothing, doNothing, doNothing, doNothing};
void (*do12[CALLBACKS_SIZE])() = {do1_buttonLogic, do2_echo, doNothing, doNothing, doNothing};
void (*do123[CALLBACKS_SIZE])() = {do1_buttonLogic, do2_echo, do3_printLight, doNothing, doNothing};
void (*do1234[CALLBACKS_SIZE])() = {do1_buttonLogic, do2_echo, do3_printLight, do4_blink, doNothing};
void (*do12345[CALLBACKS_SIZE])() = {do1_buttonLogic, do2_echo, do3_printLight, do4_blink, do5_pot};
void (*do16345[CALLBACKS_SIZE])() = {do1_buttonLogic, do6_rgbFromSerial, do3_printLight, do4_blink, do5_pot};



void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);

  analogWrite(RGB_RED, _rgbR);
  analogWrite(RGB_GREEN, _rgbG);
  analogWrite(RGB_BLUE, _rgbB);
  
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
  runCallbacks(do16345);
  delay(10);
}
