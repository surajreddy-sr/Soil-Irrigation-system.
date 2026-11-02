#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* -------------------- Wi-Fi Settings -------------------- */

const char* ssid = "STM32-Sensors";
const char* password = "sensor123";

ESP8266WebServer server(80);

/* -------------------- Sensor Values -------------------- */

String soil = "Waiting for STM32";
String temperature = "Waiting for STM32";
String humidity = "Waiting for STM32";

unsigned long lastReceived = 0;

/* -------------------- Webpage -------------------- */

void handleRoot()
{
    String page = "<!DOCTYPE html><html><head>";

    page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    page += "<meta http-equiv='refresh' content='3'>";

    page += "<title>STM32 Sensor Dashboard</title>";

    page += "<style>";

    page += "body{font-family:Arial;text-align:center;";
    page += "background:#eef2f3;margin:0;padding:20px;}";

    page += "h1{color:#123456;}";

    page += ".card{background:white;margin:15px auto;";
    page += "padding:20px;border-radius:12px;";
    page += "max-width:350px;box-shadow:0 2px 8px #bbb;}";

    page += ".value{font-size:26px;font-weight:bold;color:#008060;}";

    page += "</style></head><body>";

    page += "<h1>STM32 Sensor Dashboard</h1>";

    page += "<div class='card'>";
    page += "<h2>Soil Sensor ADC</h2>";
    page += "<p class='value'>" + soil + "</p>";
    page += "</div>";

    page += "<div class='card'>";
    page += "<h2>Temperature</h2>";
    page += "<p class='value'>" + temperature + "</p>";
    page += "</div>";

    page += "<div class='card'>";
    page += "<h2>Humidity</h2>";
    page += "<p class='value'>" + humidity + "</p>";
    page += "</div>";

    if (lastReceived != 0 && millis() - lastReceived < 6000)
    {
        page += "<p style='color:green;font-weight:bold'>";
        page += "STM32 Connected</p>";
    }
    else
    {
        page += "<p style='color:red;font-weight:bold'>";
        page += "Waiting for STM32 data</p>";
    }

    page += "<p>Updates automatically every 3 seconds.</p>";

    page += "</body></html>";

    server.send(200, "text/html", page);
}

/* -------------------- Setup -------------------- */

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(100);

    WiFi.mode(WIFI_AP);

    WiFi.softAP(ssid, password);

    server.on("/", handleRoot);

    server.begin();
}

/* -------------------- Main Loop -------------------- */

void loop()
{
    while (Serial.available())
    {
        String data = Serial.readStringUntil('\n');

        data.trim();

        int soilIndex = data.indexOf("SOIL:");
        int tempIndex = data.indexOf(",TEMP:");
        int humIndex = data.indexOf(",HUM:");

        if (soilIndex >= 0 && tempIndex >= 0 && humIndex >= 0)
        {
            soil = data.substring(
                soilIndex + 5,
                tempIndex
            );

            temperature = data.substring(
                tempIndex + 6,
                humIndex
            );

            humidity = data.substring(
                humIndex + 5
            );

            lastReceived = millis();
        }
    }

    server.handleClient();
}
