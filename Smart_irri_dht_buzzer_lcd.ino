#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// === Definitions ===
#define DHTPIN 2
#define DHTTYPE DHT22

#define soilPin A0
#define buzzerPin 3
#define relayPin 4

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pump protection timer
unsigned long lastPumpRun = 0;
const unsigned long pumpInterval = 15000; // 15 seconds minimum gap

void setup() {

  Serial.begin(9600);

  dht.begin();

  lcd.init();
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin, LOW);

  // === Boot Screen ===
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Smart Irrigation");
  lcd.setCursor(0,1);
  lcd.print("System Booting");

  delay(2000);

  // === Loading Bar with Buzzer Sync ===
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Loading...");

  lcd.setCursor(0,1);
  lcd.print("[              ]");

  for(int i = 1; i <= 14; i++)
  {
    lcd.setCursor(i,1);
    lcd.print("#");

    digitalWrite(buzzerPin, HIGH);
    delay(60);
    digitalWrite(buzzerPin, LOW);

    delay(140);
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("System Ready");
  lcd.setCursor(0,1);
  lcd.print("Starting...");

  delay(1000);
  lcd.clear();
}

void loop() {

  int soilValue = analogRead(soilPin);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("DHT Error!");
    return;
  }

  Serial.print("Soil: ");
  Serial.println(soilValue);

  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temp,1);
  lcd.print((char)223);
  lcd.print("C ");

  lcd.print("H:");
  lcd.print(hum,0);
  lcd.print("% ");

  lcd.setCursor(0,1);
  lcd.print("Soil:");
  lcd.print(soilValue);
  lcd.print(" ");

  // === Soil Condition ===
  if (soilValue > 700) {

    lcd.setCursor(12,1);
    lcd.print("Dry");

    // Check pump protection timer
    if(millis() - lastPumpRun > pumpInterval) {

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Watering Plants");
      lcd.setCursor(0,1);
      lcd.print("Pump: ON");

      digitalWrite(relayPin, HIGH);

      // === Pump Alert Buzzer while ON ===
      unsigned long startTime = millis();
      while(millis() - startTime < 9000) { // Pump runs for 9 seconds
        digitalWrite(buzzerPin, HIGH);
        delay(300);
        digitalWrite(buzzerPin, LOW);
        delay(700);
      }

      digitalWrite(relayPin, LOW);

      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Watering Done");
      lcd.setCursor(0,1);
      lcd.print("Pump: OFF");

      lastPumpRun = millis();

      delay(100);
      lcd.clear();
    }
  }

  else if (soilValue < 400) {

    lcd.setCursor(12,1);
    lcd.print("Wet ");

    digitalWrite(relayPin, LOW);
  }

  else {

    lcd.setCursor(12,1);
    lcd.print("Good");

    digitalWrite(relayPin, LOW);
  }

  delay(2000);
}