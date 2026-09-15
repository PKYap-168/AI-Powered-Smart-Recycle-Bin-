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
// Servo pins
// ======================================================

const int PLASTIC_PIN = 18;
const int GLASS_PIN   = 19;
const int METAL_PIN   = 25;
const int GENERAL_PIN = 26;

// ======================================================
// PLASTIC SERVO CALIBRATION
// ======================================================

const int PLASTIC_STOP = 93;

const int PLASTIC_OPEN_SPEED  = 135;
const int PLASTIC_CLOSE_SPEED = 50;

const int PLASTIC_OPEN_TIME  = 295;
const int PLASTIC_CLOSE_TIME = 298;

// ======================================================
// GLASS SERVO CALIBRATION
// ======================================================

const int GLASS_STOP = 93;

const int GLASS_OPEN_SPEED  = 135;
const int GLASS_CLOSE_SPEED = 50;

const int GLASS_OPEN_TIME  = 295;
const int GLASS_CLOSE_TIME = 287;

// ======================================================
// METAL SERVO CALIBRATION
// ======================================================

const int METAL_STOP = 93;

const int METAL_OPEN_SPEED  = 135;
const int METAL_CLOSE_SPEED = 50;

const int METAL_OPEN_TIME  = 295;
const int METAL_CLOSE_TIME = 298;

// ======================================================
// GENERAL WASTE SERVO CALIBRATION
// ======================================================

const int GENERAL_STOP = 93;

const int GENERAL_OPEN_SPEED  = 50;
const int GENERAL_CLOSE_SPEED = 98;

const int GENERAL_OPEN_TIME  = 285;
const int GENERAL_CLOSE_TIME = 225;

// ======================================================
// General timing
// ======================================================

// Bin stays open for 4.5 seconds
const int HOLD_OPEN_TIME = 4500;

// ======================================================
// PLASTIC SERVO
// ======================================================

void activatePlasticServo() {

  Serial.println("Opening PLASTIC bin");

  plasticServo.write(PLASTIC_OPEN_SPEED);
  delay(PLASTIC_OPEN_TIME);

  plasticServo.write(PLASTIC_STOP);

  Serial.println("PLASTIC bin opened");

  delay(HOLD_OPEN_TIME);

  Serial.println("Closing PLASTIC bin");

  plasticServo.write(PLASTIC_CLOSE_SPEED);
  delay(PLASTIC_CLOSE_TIME);

  plasticServo.write(PLASTIC_STOP);

  Serial.println("PLASTIC bin closed");
}

// ======================================================
// GLASS SERVO
// ======================================================

void activateGlassServo() {

  Serial.println("Opening GLASS bin");

  glassServo.write(GLASS_OPEN_SPEED);
  delay(GLASS_OPEN_TIME);

  glassServo.write(GLASS_STOP);

  Serial.println("GLASS bin opened");

  delay(HOLD_OPEN_TIME);

  Serial.println("Closing GLASS bin");

  glassServo.write(GLASS_CLOSE_SPEED);
  delay(GLASS_CLOSE_TIME);

  glassServo.write(GLASS_STOP);

  Serial.println("GLASS bin closed");
}

// ======================================================
// METAL SERVO
// ======================================================

void activateMetalServo() {

  Serial.println("Opening METAL bin");

  metalServo.write(METAL_OPEN_SPEED);
  delay(METAL_OPEN_TIME);

  metalServo.write(METAL_STOP);

  Serial.println("METAL bin opened");

  delay(HOLD_OPEN_TIME);

  Serial.println("Closing METAL bin");

  metalServo.write(METAL_CLOSE_SPEED);
  delay(METAL_CLOSE_TIME);

  metalServo.write(METAL_STOP);

  Serial.println("METAL bin closed");
}

// ======================================================
// GENERAL WASTE SERVO
// ======================================================

void activateGeneralServo() {

  Serial.println("Opening GENERAL WASTE bin");

  generalServo.write(GENERAL_OPEN_SPEED);
  delay(GENERAL_OPEN_TIME);

  generalServo.write(GENERAL_STOP);

  Serial.println("GENERAL WASTE bin opened");

  delay(HOLD_OPEN_TIME);

  Serial.println("Closing GENERAL WASTE bin");

  generalServo.write(GENERAL_CLOSE_SPEED);
  delay(GENERAL_CLOSE_TIME);

  generalServo.write(GENERAL_STOP);

  Serial.println("GENERAL WASTE bin closed");
}

// ======================================================
// HTTP HANDLERS
// ======================================================

void handleRoot() {

  server.send(
    200,
    "text/plain",
    "ESP32 Recycling System Online"
  );
}

void handlePlastic() {

  server.send(
    200,
    "text/plain",
    "PLASTIC received"
  );

  activatePlasticServo();
}

void handleGlass() {

  server.send(
    200,
    "text/plain",
    "GLASS received"
  );

  activateGlassServo();
}

void handleMetal() {

  server.send(
    200,
    "text/plain",
    "METAL received"
  );

  activateMetalServo();
}

void handleGeneral() {

  server.send(
    200,
    "text/plain",
    "GENERAL received"
  );

  activateGeneralServo();
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

  // --------------------------------------------------
  // Stop all servos immediately
  // --------------------------------------------------

  plasticServo.write(PLASTIC_STOP);
  glassServo.write(GLASS_STOP);
  metalServo.write(METAL_STOP);
  generalServo.write(GENERAL_STOP);

  Serial.println("All servos initialized");

  // --------------------------------------------------
  // Wi-Fi
  // --------------------------------------------------

  Serial.println("Connecting to Wi-Fi...");

  WiFi.begin(
    ssid,
    password
  );

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();

  Serial.println("Wi-Fi connected");

  Serial.print("ESP32 IP Address: ");

  Serial.println(
    WiFi.localIP()
  );

  // --------------------------------------------------
  // HTTP routes
  // --------------------------------------------------

  server.on(
    "/",
    handleRoot
  );

  server.on(
    "/plastic",
    handlePlastic
  );

  server.on(
    "/glass",
    handleGlass
  );

  server.on(
    "/metal",
    handleMetal
  );

  server.on(
    "/general",
    handleGeneral
  );

  server.begin();

  Serial.println("Web server started");

  Serial.println();
  Serial.println("Available commands:");
  Serial.println("/plastic");
  Serial.println("/glass");
  Serial.println("/metal");
  Serial.println("/general");
}

// ======================================================
// LOOP
// ======================================================

void loop() {

  server.handleClient();
}
