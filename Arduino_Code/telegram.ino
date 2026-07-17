#define BLYNK_TEMPLATE_ID "#define BLYNK_TEMPLATE_ID "TMPL3hPV4MIYu"
#define BLYNK_TEMPLATE_NAME "Railway Animal Detection""
#define BLYNK_AUTH_TOKEN ""

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/******** WIFI ********/
char ssid[] = "Redmi 9";
char pass[] = "17062006";

/******** TELEGRAM ********/
#define BOT_TOKEN "8799541678:AAFc_cpM6siCRq1Mt9ffK-BiTjPvxDzHlm0"
#define CHAT_ID "6754550178"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

/******** LCD ********/
LiquidCrystal_I2C lcd(0x27, 16, 2);

/******** SENSORS ********/
const int piezoPins[] = {32, 33, 34, 35};
const int numSensors = 4;

/******** OUTPUT ********/
const int redLED = 26;
const int greenLED = 27;
const int buzzerPin = 25;

/******** SETTINGS ********/
int threshold = 900;

/******** VARIABLES ********/
bool detected = false;
int activeSensor = 0;
unsigned long detectTime = 0;
const unsigned long alertDuration = 3000;

bool telegramSent = false; // prevent spam

void setup() {

  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  client.setInsecure(); // needed for ESP32

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);

  lcd.begin();
  lcd.backlight();

  lcd.print("System Loading");
  delay(2000);

  lcd.clear();
  lcd.print("Railway Monitor");
  lcd.setCursor(0, 1);
  lcd.print("Status: Active");
}

void loop() {

  Blynk.run();

  if (!detected) {

    telegramSent = false;

    for (int i = 0; i < numSensors; i++) {

      int val = analogRead(piezoPins[i]);

      if (val > threshold) {

        detected = true;
        activeSensor = i + 1;
        detectTime = millis();

        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);

        lcd.clear();
        lcd.print("ANIMAL DETECTED");
        lcd.setCursor(0, 1);
        lcd.print("Sensor: ");
        lcd.print(activeSensor);

        // 📲 Blynk update
        Blynk.virtualWrite(V0, "DETECTED");
        Blynk.virtualWrite(V1, activeSensor);

        break;
      }
    }
  }

  if (detected) {

    // 🔊 buzzer
    tone(buzzerPin, 4000);
    delay(200);
    noTone(buzzerPin);
    delay(200);

    // 📲 Telegram Alert (ONLY ONCE)
    if (!telegramSent) {
      String msg = "🚨 ANIMAL DETECTED!\n";
      msg += "Sensor: " + String(activeSensor);

      bot.sendMessage(CHAT_ID, msg, "");

      telegramSent = true;
    }

    if (millis() - detectTime >= alertDuration) {

      detected = false;

      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, HIGH);
      noTone(buzzerPin);

      lcd.clear();
      lcd.print("Railway Monitor");
      lcd.setCursor(0, 1);
      lcd.print("Status: Active");

      // 📲 reset Blynk
      Blynk.virtualWrite(V0, "SAFE");
      Blynk.virtualWrite(V1, 0);
    }
  }
}