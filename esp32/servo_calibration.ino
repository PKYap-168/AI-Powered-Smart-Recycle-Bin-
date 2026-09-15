#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// ======================================================
// Wi-Fi
// ======================================================

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// ======================================================
// Servo objects
// ======================================================

Servo plasticServo;
Servo glassServo;
Servo metalServo;
Servo generalServo;

// ======================================================
// Pins
// ======================================================

const int PLASTIC_PIN = 18;
const int GLASS_PIN   = 19;
const int METAL_PIN   = 25;
const int GENERAL_PIN = 26;

// ======================================================
// STOP VALUES
// Change individually if needed
// ======================================================

const int PLASTIC_STOP = 93;
const int GLASS_STOP   = 93;
const int METAL_STOP   = 93;
const int GENERAL_STOP = 93;

// ======================================================
// SLOW CALIBRATION SPEEDS
// ======================================================

// First 3 servos use same physical orientation

const int NORMAL_SLOW_OPEN  = 97;
const int NORMAL_SLOW_CLOSE = 90;

// General servo is inverted

const int GENERAL_SLOW_OPEN  = 90;
const int GENERAL_SLOW_CLOSE = 97;

// ======================================================
// STOP ALL SERVOS
// ======================================================

void stopAllServos() {

  plasticServo.write(PLASTIC_STOP);
  glassServo.write(GLASS_STOP);
  metalServo.write(METAL_STOP);
  generalServo.write(GENERAL_STOP);

  Serial.println("ALL SERVOS STOPPED");
}

// ======================================================
// PLASTIC
// ======================================================

void plasticOpen() {

  stopAllServos();

  plasticServo.write(NORMAL_SLOW_OPEN);

  Serial.println("PLASTIC moving toward OPEN");

  server.send(
    200,
    "text/plain",
    "PLASTIC OPENING"
  );
}

void plasticClose() {

  stopAllServos();

  plasticServo.write(NORMAL_SLOW_CLOSE);

  Serial.println("PLASTIC moving toward CLOSE");

  server.send(
    200,
    "text/plain",
    "PLASTIC CLOSING"
  );
}

// ======================================================
// GLASS
// ======================================================

void glassOpen() {

  stopAllServos();

  glassServo.write(NORMAL_SLOW_OPEN);

  Serial.println("GLASS moving toward OPEN");

  server.send(
    200,
    "text/plain",
    "GLASS OPENING"
  );
}

void glassClose() {

  stopAllServos();

  glassServo.write(NORMAL_SLOW_CLOSE);

  Serial.println("GLASS moving toward CLOSE");

  server.send(
    200,
    "text/plain",
    "GLASS CLOSING"
  );
}

// ======================================================
// METAL
// ======================================================

void metalOpen() {

  stopAllServos();

  metalServo.write(NORMAL_SLOW_OPEN);

  Serial.println("METAL moving toward OPEN");

  server.send(
    200,
    "text/plain",
    "METAL OPENING"
  );
}

void metalClose() {

  stopAllServos();

  metalServo.write(NORMAL_SLOW_CLOSE);

  Serial.println("METAL moving toward CLOSE");

  server.send(
    200,
    "text/plain",
    "METAL CLOSING"
  );
}

// ======================================================
// GENERAL
// ======================================================

void generalOpen() {

  stopAllServos();

  generalServo.write(GENERAL_SLOW_OPEN);

  Serial.println("GENERAL moving toward OPEN");

  server.send(
    200,
    "text/plain",
    "GENERAL OPENING"
  );
}

void generalClose() {

  stopAllServos();

  generalServo.write(GENERAL_SLOW_CLOSE);

  Serial.println("GENERAL moving toward CLOSE");

  server.send(
    200,
    "text/plain",
    "GENERAL CLOSING"
  );
}

// ======================================================
// STOP COMMAND
// ======================================================

void handleStop() {

  stopAllServos();

  server.send(
    200,
    "text/plain",
    "STOPPED"
  );
}

// ======================================================
// ROOT
// ======================================================

void handleRoot() {

  server.send(
    200,
    "text/plain",
    "Servo Calibration Mode"
  );
}

// ======================================================
// SETUP
// ======================================================

void setup() {

  Serial.begin(115200);

  delay(1000);

  // --------------------------------------------------
  // Attach servos
  // --------------------------------------------------

  plasticServo.attach(PLASTIC_PIN);
  glassServo.attach(GLASS_PIN);
  metalServo.attach(METAL_PIN);
  generalServo.attach(GENERAL_PIN);

  // Stop immediately

  stopAllServos();

  // --------------------------------------------------
  // Wi-Fi
  // --------------------------------------------------

  Serial.println("Connecting to Wi-Fi...");

  WiFi.begin(
    ssid,
    password
  );

  while (
    WiFi.status() != WL_CONNECTED
  ) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();

  Serial.println("Wi-Fi connected");

  Serial.print("ESP32 IP: ");

  Serial.println(
    WiFi.localIP()
  );

  // --------------------------------------------------
  // Routes
  // --------------------------------------------------

  server.on("/", handleRoot);

  server.on(
    "/plastic/open",
    plasticOpen
  );

  server.on(
    "/plastic/close",
    plasticClose
  );

  server.on(
    "/glass/open",
    glassOpen
  );

  server.on(
    "/glass/close",
    glassClose
  );

  server.on(
    "/metal/open",
    metalOpen
  );

  server.on(
    "/metal/close",
    metalClose
  );

  server.on(
    "/general/open",
    generalOpen
  );

  server.on(
    "/general/close",
    generalClose
  );

  server.on(
    "/stop",
    handleStop
  );

  server.begin();

  Serial.println(
    "Calibration server ready"
  );
}

// ======================================================
// LOOP
// ======================================================

void loop() {

  server.handleClient();
}
