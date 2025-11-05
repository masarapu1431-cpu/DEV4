#include <WiFi.h>
#include "DHT.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -------------------- Sensor Pins --------------------
#define DHTPIN 15
#define DHTTYPE DHT11
#define UV_LDR 34    // UV/Light intensity sensor
#define redLDR 32
#define greenLDR 33
#define blueLDR 25

// -------------------- Sensor & Display Setup --------------------
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// -------------------- Wi-Fi Setup (Optional) --------------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// -------------------- Setup --------------------
void setup() {
  Serial.begin(115200);
  dht.begin();

  // OLED setup
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(10, 25);
  display.println("Smart Skin Monitor");
  display.display();
  delay(2000);

  // Wi-Fi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println("Connecting to WiFi...");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  display.clearDisplay();
  display.setCursor(0, 20);
  display.println("WiFi Connected!");
  display.display();
  delay(1000);
}

// -------------------- Main Loop --------------------
void loop() {
  // --- Read Sensors ---
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();
  int lightVal = analogRead(UV_LDR); // UV/sunlight intensity
  int redVal = analogRead(redLDR);
  int greenVal = analogRead(greenLDR);
  int blueVal = analogRead(blueLDR);

  if (isnan(humidity) || isnan(temp)) {
    Serial.println("Sensor Error");
    return;
  }

  // --- Determine Skin Condition ---
  String condition = "";
  if (humidity < 30) condition = "Dry Skin";
  else if (humidity > 60) condition = "Oily Skin";
  else condition = "Normal Skin";

  if (temp > 37) condition += " + Infection";
  if (lightVal > 2500) condition += " + High UV";

  // --- Determine Skin Tone (Colour Detection) ---
  String tone = "";
  if (redVal > greenVal && redVal > blueVal) tone = "Reddish (Irritated)";
  else if (greenVal > redVal && greenVal > blueVal) tone = "Healthy Tone";
  else if (blueVal > redVal && blueVal > greenVal) tone = "Pale/Dry";
  else tone = "Neutral";

  // --- OLED Display ---
if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
  Serial.println("OLED not found");
  for (;;);
}

display.clearDisplay();
display.setTextColor(SSD1306_WHITE);
display.setTextSize(1);
display.setCursor(0, 0);
display.println("Smart Skin Monitor");
display.display();
delay(2000);
  // --- Serial Monitor Output ---
  Serial.println("==================================");
  Serial.print("Temperature: "); Serial.println(temp);
  Serial.print("Humidity: "); Serial.println(humidity);
  Serial.print("UV/Light: "); Serial.println(lightVal);
  Serial.print("Red: "); Serial.print(redVal);
  Serial.print("  Green: "); Serial.print(greenVal);
  Serial.print("  Blue: "); Serial.println(blueVal);
  Serial.print("Skin Tone: "); Serial.println(tone);
  Serial.print("Condition: "); Serial.println(condition);
  Serial.println("==================================");

  delay(5000);
}