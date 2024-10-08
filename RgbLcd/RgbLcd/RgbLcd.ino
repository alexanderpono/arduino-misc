#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

const int LED = 6;
const int BUTTON = 2;
const int POT = 0;
#define CALLBACKS_SIZE 6

#define RGB_RED   11
#define RGB_GREEN 10
#define RGB_BLUE   9

int _rgbR = 50;
int _rgbG = 50;
int _rgbB = 50;
bool isButPressed = false;


void doNothing() {}

void do1_buttonLogic() {
  if (digitalRead(BUTTON) == LOW) {
    isButPressed = false;
    turnOff(LED_BUILTIN);
  } else {
    isButPressed = true;
    turnOn(LED_BUILTIN);
  }
  lcdButState();
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
    lcdTelemetry();
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
int _potVal = 0;
void do5_pot() {
  int val = analogRead(POT);
  int per = map(val, 0, 1023, 0, 100);
  _potVal = per;

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

void lcdButState() {
  lcd.setCursor(0, 1);
  String butInfo = "bt " + String(isButPressed);
  lcd.print(butInfo); 
}

void lcdTelemetry() {
  lcdButState();

  lcd.setCursor(0, 0);
  String lightInfo = "lt " + String(_doBlink_light) + "  ";
  lcd.print(lightInfo); 
  
  lcd.setCursor(8, 0);
  String potInfo = "pot " + String(_potVal) + "  ";
  lcd.print(potInfo); 

  lcd.setCursor(8, 1);
  int r = _rgbR / 10;
  int g = _rgbG / 10;
  int b = _rgbB / 10;
  String rgbInfo = "RGB " + String(r) + String(g) + String(b);
  lcd.print(rgbInfo); 

}

void (*empty[CALLBACKS_SIZE])() = {doNothing, doNothing, doNothing, doNothing, doNothing, doNothing};
void (*do1[CALLBACKS_SIZE])() = {do1_buttonLogic, doNothing, doNothing, doNothing, doNothing, doNothing};
void (*do12[CALLBACKS_SIZE])() = {do1_buttonLogic, do2_echo, doNothing, doNothing, doNothing, doNothing};
void (*do123[CALLBACKS_SIZE])() = {do1_buttonLogic, do2_echo, do3_printLight, doNothing, doNothing, doNothing};
void (*do1234[CALLBACKS_SIZE])() = {do1_buttonLogic, do2_echo, do3_printLight, do4_blink, doNothing, doNothing};
void (*do12345[CALLBACKS_SIZE])() = {do1_buttonLogic, do2_echo, do3_printLight, do4_blink, do5_pot, doNothing};
void (*do16345[CALLBACKS_SIZE])() = {do1_buttonLogic, do6_rgbFromSerial, do3_printLight, do4_blink, do5_pot, doNothing};
void (*do163457[CALLBACKS_SIZE])() = {do1_buttonLogic, do6_rgbFromSerial, do3_printLight, do4_blink, do5_pot, lcdTelemetry};



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
  
  lcd.init();                     
  lcd.backlight();
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
