#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr int OLED_RESET = -1;
constexpr uint8_t OLED_ADDR = 0x3C;

constexpr int I2C_SDA = 21;
constexpr int I2C_SCL = 22;

const char *AP_SSID = "ESP32_OLED";
const char *AP_PASSWORD = "12345678";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WebServer server(80);

String currentText = "Hello from ESP32";

void drawTextOnOled(const String &text)
{
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0, 0);

	const int maxCharsPerLine = 21;
	const int maxLines = 8;
	int line = 0;
	int index = 0;

	while (index < text.length() && line < maxLines)
	{
		int remain = text.length() - index;
		int take = remain > maxCharsPerLine ? maxCharsPerLine : remain;
		display.println(text.substring(index, index + take));
		index += take;
		line++;
	}

	display.display();
}

String buildHtmlPage()
{
	String html;
	html.reserve(1200);
	html += "<!doctype html><html><head><meta charset='utf-8'>";
	html += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
	html += "<title>ESP32 OLED</title></head><body style='font-family:Arial;padding:16px;'>";
	html += "<h2>ESP32 OLED Text</h2>";
	html += "<p>Connected AP: <b>";
	html += AP_SSID;
	html += "</b></p>";
	html += "<form action='/set' method='get'>";
	html += "<input type='text' name='text' maxlength='200' style='width:90%;padding:8px;' placeholder='Type English text'>";
	html += "<button type='submit' style='padding:8px 12px;margin-left:8px;'>Send</button>";
	html += "</form>";
	html += "<p>Current text on OLED:</p><pre style='background:#f3f3f3;padding:10px;'>";
	html += currentText;
	html += "</pre></body></html>";
	return html;
}

void handleRoot()
{
	server.send(200, "text/html", buildHtmlPage());
}

void handleSetText()
{
	if (server.hasArg("text"))
	{
		currentText = server.arg("text");
		if (currentText.length() == 0)
		{
			currentText = "(empty)";
		}
		drawTextOnOled(currentText);
	}

	server.sendHeader("Location", "/");
	server.send(302, "text/plain", "Updated");
}

void setup()
{
	Serial.begin(115200);

	Wire.begin(I2C_SDA, I2C_SCL);
	if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
	{
		Serial.println("SSD1306 init failed");
		while (true)
		{
			delay(1000);
		}
	}

	drawTextOnOled("Starting...");

	WiFi.mode(WIFI_AP);
	bool apOk = WiFi.softAP(AP_SSID, AP_PASSWORD);
	if (!apOk)
	{
		drawTextOnOled("AP start failed");
		return;
	}

	IPAddress apIp = WiFi.softAPIP();
	String ipMsg = "AP IP: " + apIp.toString();
	drawTextOnOled(ipMsg);
	delay(1200);

	drawTextOnOled(currentText);

	server.on("/", handleRoot);
	server.on("/set", handleSetText);
	server.begin();

	Serial.print("AP IP: ");
	Serial.println(apIp);
}

void loop()
{
	server.handleClient();
}