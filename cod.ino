#include <Adafruit_Sensor.h>
#include <Adafruit_HTU21DF.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

const int motorPin = A0; 
const int mq135Pin = A2; 

Adafruit_HTU21DF htu = Adafruit_HTU21DF(); 

float voltage;
float windSpeed;

#define TFT_CS 10
#define TFT_RST 8
#define TFT_DC 9
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

enum DisplayMode {
  TEMPERATURE,
  HUMIDITY,
  WIND_SPEED,
  AIR_QUALITY
};

DisplayMode currentMode = TEMPERATURE;

void setup() {
  Serial.begin(9600);
  
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    while (1);
  }
  
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
}

void loop() {
  float time = 3000;

  switch (currentMode) {
    case TEMPERATURE:
      displayTemperature();
      delay(time);
      break;
    case HUMIDITY:
      displayHumidity();
      delay(time);
      break;
    case WIND_SPEED:
      displayWindSpeed();
      delay(time);
      break;
    case AIR_QUALITY:
      displayAirQuality();
      delay(time);
      break;
  }

  int sensorValue = analogRead(motorPin);
  voltage = sensorValue * (5.0 / 1023.0);
  windSpeed = 90.9090909 * voltage;
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(", Speed: ");
  Serial.println(windSpeed);
  delay(500);

  currentMode = (DisplayMode)((currentMode + 1) % 4);
}

void displayTemperature() {
  float temp = htu.readTemperature();
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);

  int16_t x, y;
  uint16_t w, h;
  tft.getTextBounds("TEMP", 0, 0, &x, &y, &w, &h);
  int16_t labelWidth = w;
  tft.setCursor((tft.width() - labelWidth) / 2, tft.height() / 2 - h - 10);
  tft.println("TEMP");

  String tempStr = String(temp) + " *C";
  tft.getTextBounds(tempStr, 0, 0, &x, &y, &w, &h);
  int16_t valueWidth = w;
  tft.setCursor((tft.width() - valueWidth) / 2, tft.height() / 2 + 10);
  tft.print(tempStr);
}

void displayHumidity() {
  float humidity = htu.readHumidity();
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);

  int16_t x, y;
  uint16_t w, h;
  tft.getTextBounds("HUM", 0, 0, &x, &y, &w, &h);
  int16_t labelWidth = w;
  tft.setCursor((tft.width() - labelWidth) / 2, tft.height() / 2 - h - 10);
  tft.println("HUM");

  String humidityStr = String(humidity) + " %";
  tft.getTextBounds(humidityStr, 0, 0, &x, &y, &w, &h);
  int16_t valueWidth = w;
  tft.setCursor((tft.width() - valueWidth) / 2, tft.height() / 2 + 10);
  tft.print(humidityStr);
}

void displayWindSpeed() {
  int sensorValue = analogRead(motorPin);
  voltage = sensorValue * (5.0 / 1023.0);
  windSpeed = 90.9090909 * voltage;
  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);

  int16_t x, y;
  uint16_t w, h;
  tft.getTextBounds("WIND", 0, 0, &x, &y, &w, &h);
  int16_t labelWidth = w;
  tft.setCursor((tft.width() - labelWidth) / 2, tft.height() / 2 - h - 10);
  tft.println("WIND");

  String windSpeedStr = String(windSpeed, 2) + " m/s";
  tft.getTextBounds(windSpeedStr, 0, 0, &x, &y, &w, &h);
  int16_t valueWidth = w;
  tft.setCursor((tft.width() - valueWidth) / 2, tft.height() / 2 + 10);
  tft.print(windSpeedStr);
}

void displayAirQuality() {
  int sensorValue = analogRead(mq135Pin);
  float voltage = sensorValue * (5.0 / 1023.0);
  float airQuality = voltage * 200.0;

  //Serial.println(airQuality);

  tft.fillScreen(ST7735_BLACK);
  tft.setTextSize(2);

  int16_t x, y;
  uint16_t w, h;
  tft.getTextBounds("AIR", 0, 0, &x, &y, &w, &h);
  int16_t labelWidth = w;
  tft.setCursor((tft.width() - labelWidth) / 2, tft.height() / 2 - h - 10);
  tft.println("AIR");

  String airQualityStr = String(airQuality, 2) + " ppm";
  tft.getTextBounds(airQualityStr, 0, 0, &x, &y, &w, &h);
  int16_t valueWidth = w;
  tft.setCursor((tft.width() - valueWidth) / 2, tft.height() / 2 + 10);
  tft.print(airQualityStr);
  tft.setCursor((tft.width() - valueWidth) / 2, tft.height() / 2 + 35);
  tft.print(airQuality / 1023 * 100);
  tft.print("%");
}