#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#define TRIG_PIN 5
#define ECHO_PIN 18
#define PUMP_PIN 19

const char* ssid = "ESP32-AP";
const char* password = "12345678";

AsyncWebServer server(80);

long duration;
float distance;
bool pumpState = false;

float readWaterLevel() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2; // cm
  float levelPercent = max(0.0, min(100.0, 100.0 - (distance / 50.0 * 100.0)));
  return levelPercent;
}

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);

  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", webpage);
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    float level = readWaterLevel();

    StaticJsonDocument<200> doc;
    doc["level"] = level;
    doc["pump"] = pumpState;

    String json;
    serializeJson(doc, json);
    request->send(200, "application/json", json);
  });

  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request){
    pumpState = !pumpState;
    digitalWrite(PUMP_PIN, pumpState ? HIGH : LOW);
    request->send(200, "text/plain", pumpState ? "Pump ON" : "Pump OFF");
  });

  server.begin();
}

void loop() {
  // All handled by async server
}

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Smart Water Tank</title>
<style>
body { font-family: Arial; text-align: center; background-color: #111; color: #0ff; }
h1 { margin-bottom: 20px; }
div.data { font-size: 24px; margin: 15px; }
button { font-size: 20px; padding: 10px 20px; margin-top: 10px; }
</style>
</head>
<body>
<h1>Smart Water Tank Dashboard</h1>
<div class="data">Water Level: <span id="level">0</span> %</div>
<div class="data">Pump State: <span id="pump">OFF</span></div>
<button onclick="togglePump()">Toggle Pump</button>

<script>
function fetchData() {
  fetch('/data')
  .then(response => response.json())
  .then(data => {
    document.getElementById('level').innerText = data.level.toFixed(1);
    document.getElementById('pump').innerText = data.pump ? "ON" : "OFF";
  });
}
function togglePump() {
  fetch('/toggle')
  .then(fetchData);
}
setInterval(fetchData, 1000);
fetchData();
</script>
</body>
</html>
)rawliteral";
