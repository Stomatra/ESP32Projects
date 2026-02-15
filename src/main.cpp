#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// -------------------- Pin config (adjust if needed) --------------------
static const int PIN_I2C_SDA = 21;
static const int PIN_I2C_SCL = 22;

static const int PIN_DHT = 4;
static const int PIN_PIR = 27;
static const int PIN_LDR_ADC = 34;     // ADC input-only pin

static const int PIN_LED_R = 25;
static const int PIN_LED_G = 26;
static const int PIN_LED_B = 33;

// -------------------- Devices --------------------
static const int SCREEN_WIDTH = 128;
static const int SCREEN_HEIGHT = 64;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

DHT dht(PIN_DHT, DHT11);

// -------------------- PWM config for RGB (common cathode => HIGH = brighter) --------------------
static const int PWM_FREQ = 5000;  // Hz
static const int PWM_RES_BITS = 8; // 0-255

static const int CH_R = 0;
static const int CH_G = 1;
static const int CH_B = 2;

// -------------------- Behavior tuning --------------------
static const uint32_t UI_REFRESH_ACTIVE_MS = 1000;
static const uint32_t UI_REFRESH_IDLE_MS   = 5000;

static const uint32_t ACTIVE_HOLD_MS = 30UL * 1000UL; // keep "active" for 30s after last motion

// LDR threshold: depends heavily on your module and wiring.
// We'll calibrate by printing raw ADC (0..4095). Start with ~1800-2600 range.
static int LDR_DARK_THRESHOLD = 2200;

// Night light brightness (0..255)
static uint8_t NIGHT_BRIGHTNESS = 80;

// Warm-ish color (tweak)
static uint8_t NIGHT_R = 255;
static uint8_t NIGHT_G = 120;
static uint8_t NIGHT_B = 20;

// -------------------- State --------------------
uint32_t lastMotionMs = 0;
bool nightLightOn = false;

static void setRgb(uint8_t r, uint8_t g, uint8_t b) {
  ledcWrite(CH_R, r);
  ledcWrite(CH_G, g);
  ledcWrite(CH_B, b);
}

static void nightLight(bool on) {
  nightLightOn = on;
  if (on) {
    // scale color by NIGHT_BRIGHTNESS
    auto scale = [](uint8_t c, uint8_t br) -> uint8_t {
      return (uint16_t)c * (uint16_t)br / 255;
    };
    setRgb(scale(NIGHT_R, NIGHT_BRIGHTNESS),
           scale(NIGHT_G, NIGHT_BRIGHTNESS),
           scale(NIGHT_B, NIGHT_BRIGHTNESS));
  } else {
    setRgb(0, 0, 0);
  }
}

static void drawUi(float tC, float hPct, int ldrRaw, bool pir, bool isActive) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("Multi-Sensor Station");

  display.setCursor(0, 12);
  display.print("Temp: ");
  if (isnan(tC)) display.print("--");
  else { display.print(tC, 1); display.print(" C"); }

  display.setCursor(0, 22);
  display.print("Humi: ");
  if (isnan(hPct)) display.print("--");
  else { display.print(hPct, 0); display.print(" %"); }

  display.setCursor(0, 32);
  display.print("Light(ADC): ");
  display.print(ldrRaw);

  display.setCursor(0, 42);
  display.print("PIR: ");
  display.print(pir ? "MOTION" : "none");

  display.setCursor(0, 52);
  display.print("Night: ");
  display.print(nightLightOn ? "ON " : "OFF");
  display.print("  Mode: ");
  display.print(isActive ? "ACTIVE" : "IDLE");

  display.display();
}

void setup() {
  Serial.begin(9600);
  delay(200);

  pinMode(PIN_PIR, INPUT);

  // I2C init
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 init failed (addr 0x3C). Check wiring/address.");
    while (true) delay(1000);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Booting...");
  display.display();

  dht.begin();

  // ADC config (ESP32 default ok, but set attenuation for wider range)
  analogReadResolution(12); // 0..4095
  analogSetAttenuation(ADC_11db);

  // PWM init
  ledcSetup(CH_R, PWM_FREQ, PWM_RES_BITS);
  ledcSetup(CH_G, PWM_FREQ, PWM_RES_BITS);
  ledcSetup(CH_B, PWM_FREQ, PWM_RES_BITS);
  ledcAttachPin(PIN_LED_R, CH_R);
  ledcAttachPin(PIN_LED_G, CH_G);
  ledcAttachPin(PIN_LED_B, CH_B);
  nightLight(false);

  Serial.println("Ready.");
  Serial.println("Tip: watch LDR raw values to adjust LDR_DARK_THRESHOLD.");
}

void loop() {
  const uint32_t now = millis();

  // Read PIR
  const bool pir = (digitalRead(PIN_PIR) == HIGH);
  if (pir) lastMotionMs = now;

  const bool isActive = (now - lastMotionMs) < ACTIVE_HOLD_MS;

  // Read sensors
  const int ldrRaw = analogRead(PIN_LDR_ADC);

  // DHT11 is slow; reading too often may return NaN. We'll read each loop but UI refresh throttles.
  const float h = dht.readHumidity();
  const float t = dht.readTemperature();

  // Auto night light rule:
  // Only turn on if ACTIVE (recent motion) AND it's dark.
  const bool isDark = (ldrRaw < LDR_DARK_THRESHOLD);
  if (isActive && isDark) nightLight(true);
  else nightLight(false);

  // UI refresh rate depends on activity
  static uint32_t lastUiMs = 0;
  const uint32_t uiEvery = isActive ? UI_REFRESH_ACTIVE_MS : UI_REFRESH_IDLE_MS;
  if (now - lastUiMs >= uiEvery) {
    lastUiMs = now;

    drawUi(t, h, ldrRaw, pir, isActive);

    Serial.printf(
      "T=%.1fC H=%.0f%% LDR=%d PIR=%d Active=%d Dark=%d Night=%d Threshold=%d\n",
      t, h, ldrRaw, pir, isActive, isDark, nightLightOn, LDR_DARK_THRESHOLD
    );
  }

  delay(20);
}