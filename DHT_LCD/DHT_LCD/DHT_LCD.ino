#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2 // Тот самый номер пина, о котором упоминалось выше
// Одна из следующих строк закоментирована. Снимите комментарий, если подключаете датчик DHT11 к arduino
//DHT dht(DHTPIN, DHT22); //Инициация датчика

LiquidCrystal_I2C lcd(0x27,16,2);  // Устанавливаем дисплей
DHT dht(DHTPIN, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();

  lcd.init();                     
  lcd.backlight();// Включаем подсветку дисплея
}

void loop() {
  delay(2000); // 2 секунды задержки
  float h = dht.readHumidity(); //Измеряем влажность
  float t = dht.readTemperature(); //Измеряем температуру
  if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
    Serial.println("Ошибка считывания");
    return;
  }
  Serial.print("Влажность: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Температура: ");
  Serial.print(t);
  Serial.println(" *C "); //Вывод показателей на экран

  // Устанавливаем курсор на вторую строку и нулевой символ.
  lcd.setCursor(0, 0);
  // Выводим на экран количество секунд с момента запуска ардуины
  lcd.print("Hum: " + String(h) + " % ");

  lcd.setCursor(0, 1);
  lcd.print("Tem: " + String(t) + " *C ");

}
