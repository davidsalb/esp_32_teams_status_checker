#include <WiFi.h>
#include <HTTPClient.h>
#include "EEPROM.h"
#include <Arduino_JSON.h>

#include <Adafruit_NeoPixel.h>
#define LED_PIN 23
#define LED_COUNT 3
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#include "credentials.h"

int TOKEN_EEPROM_ADDR = 0;
#define TOKEN_EEPROM_SIZE 1536

String access_token = "";
String refresh_token = "";

typedef struct {
  String availability;
  String activity;
} presenceStatus;

int refresh_counter = 0;
int refresh_interval = 10000; // [ms]

const char* tokenEndpoint = "https://login.microsoftonline.com/common/oauth2/v2.0/token";
const char* deviceCodeEndpoint = "https://login.microsoftonline.com/common/oauth2/v2.0/devicecode";
const char* graphPresenceEndpoint = "https://graph.microsoft.com/beta/me/presence";

void setup() {
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  Serial.begin(115200);
  Serial.println("starting now...");

  if (!EEPROM.begin(TOKEN_EEPROM_SIZE)) {
    Serial.println("failed to init EEPROM");
  }
  delay(1000);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    colorWipe(strip.Color(0,   0,   255));
    delay(500);
    colorWipe(strip.Color(0,   0,   0));
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected to the WiFi network ");
  Serial.println(ssid);
}

void loop() {
  if (access_token != "") {
    presenceStatus presence = getPresence();
    showStatus(presence);

    refresh_counter ++;
    Serial.print(refresh_counter);
    Serial.print(" PresenceStatus (Availability, Activity): ");
    Serial.print(presence.availability);
    Serial.print(", ");
    Serial.println(presence.activity);
  }
  else if (refresh_token != "") {
    refreshAccessToken();
  }
  else if (recoverRefreshToken()) {
    Serial.print("\nRecovered RefreshToken from EEPROM: ");
    Serial.println(refresh_token);
    Serial.println("Enter \"reset\" to delete RefreshToken and re-authenticate.");
  }
  else {
    Serial.println("Could not recover refresh token. Authenticate.");
    authenticate();
  }
  
  if (getSerialInput(refresh_interval).indexOf("reset") > -1) {
    Serial.println("Resetting.");
    refresh_token = "";
    access_token = "";
    clearEEPROM();
  };
}

enum Activities {
  Default,       // 0
  Available,     // 1
  BeRightBack,   // 2
  Away,          // 3
  OffWork,       // 4
  InAMeeting,    // 5
  InACall,       // 6
  Presenting,    // 7
  Busy,          // 8
  DoNotDisturb,  // 9
};

int resolveActivities(String presence) {
  Activities output = Default;

  if (presence == "Available") {
    output = Available;
  }
  else if (presence == "BeRightBack") {
    output = BeRightBack;
  }
  else if (presence == "Away") {
    output = Away;
  }
  else if (presence == "OffWork") {
    output = OffWork;
  }
  else if (presence == "InAMeeting") {
    output = InAMeeting;
  }
  else if (presence == "InACall") {
    output = InACall;
  }
  else if (presence == "Presenting") {
    output = Presenting;
  }
  else if (presence == "Busy") {
    output = Busy;
  }
  else if (presence == "DoNotDisturb") {
    output = DoNotDisturb;
  }

  return output;
}

void showStatus(presenceStatus presence) {

  if (!showActivity(presence.activity)) {
    Serial.println("could not resolve activity!");
    showAvailability(presence.availability);
  }
}

bool showActivity(String presence) {
  switch (resolveActivities(presence)) {

    case Available:
      colorWipe(strip.Color(0,   255,   0));
      break;

//    case Away:
//    case BeRightBack:
//      colorWipe(strip.Color(255,   150,   0));
//      break;

    case InAMeeting:
    case InACall:
    case Presenting:
      colorWipe(strip.Color(100,   0,   255));
      break;

    case Busy:
    case DoNotDisturb:
      colorWipe(strip.Color(255,   0,  10));
      break;

    default:
      colorWipe(strip.Color(0,   0,   0));
      return false;
      break;

      return true;
  }
}

bool showAvailability(String presence) {
  Serial.println("showAvailability called");
  // not implemented yet
  return false;
}

void authenticate() {
  String deviceCode = getDeviceCode();
  Serial.println("Proceed by entering any key. ");
  getSerialInput(0);
  getAccessToken(deviceCode);
  persistRefreshToken();
}

presenceStatus getPresence() {
  String activity = "";
  String aviability = "";
  presenceStatus presence = {aviability, activity};
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(graphPresenceEndpoint, graph_ca);
    http.addHeader("Authorization", ("Bearer " + access_token));
    int httpCode = http.GET();
    Serial.print("GET getPresence HTTP-Code: ");
    Serial.println(httpCode);

    if (httpCode < 0) {
      Serial.println("Error on HTTP request");
      http.end();
      return presence;
    }

    String payload = http.getString();

    //    Serial.println("getPresence payload:");
    //    Serial.println(payload);
    JSONVar jsonObject = JSON.parse(payload);

    if (httpCode == 200) {
      if (jsonObject.hasOwnProperty("activity")) {
        activity = jsonObject["activity"];
      }
      if (jsonObject.hasOwnProperty("availability")) {
        aviability = jsonObject["availability"];
      }
    }
    else if (httpCode == 401) {
      access_token = "";
      refresh_counter ++;
      loop();
    }
    http.end();
  }
  presence = {aviability, activity};
  return presence;
}

void getAccessToken(String device_code) {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;

    http.begin(tokenEndpoint, login_ca);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String requestData = appendRequestData("", "client_id", client_id);
    requestData = appendRequestData(requestData, "device_code", device_code);
    requestData = appendRequestData(requestData, "grant_type", "urn:ietf:params:oauth:grant-type:device_code");

    int httpCode = http.POST(requestData);
    Serial.print("POST getAccessToken HTTP-Code: ");
    Serial.println(httpCode);

    String payload = http.getString();
    JSONVar jsonObject = JSON.parse(payload);
    if (httpCode == 200) { //Check for the returning code
      if (jsonObject.hasOwnProperty("access_token")) {
        access_token = jsonObject["access_token"];
      }
      if (jsonObject.hasOwnProperty("refresh_token")) {
        refresh_token = jsonObject["refresh_token"];
      }
      //      if (jsonObject.hasOwnProperty("expires_in")) {
      //        int expires_in = (int)jsonObject["expires_in"];
      //        access_token_expires_at = millis() + ((expires_in) * 1000);
      //      }
    }

    else {
      access_token = "";
      refresh_token = "";
      Serial.println("Failed to retrieve Token.");
      if (jsonObject.hasOwnProperty("error")) {
        Serial.print("Error: ");
        Serial.println(jsonObject["error"]);
        if (jsonObject.hasOwnProperty("error_description")) {
          Serial.print("Error description: ");
          Serial.println(jsonObject["error_description"]);
        }
        http.end();
      }
    }
  }
}

void refreshAccessToken() {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;

    access_token = "";

    http.begin(tokenEndpoint, login_ca);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String requestData = appendRequestData("", "client_id", client_id);
    requestData = appendRequestData(requestData, "scope", "openid Presence.Read profile User.Read email");
    requestData = appendRequestData(requestData, "refresh_token", refresh_token);
    requestData = appendRequestData(requestData, "grant_type", "refresh_token");

    int httpCode = http.POST(requestData);
    Serial.print("POST refreshAccessToken HTTP-Code: ");
    Serial.println(httpCode);

    String payload = http.getString();
    if (httpCode == 200) {
      JSONVar jsonObject = JSON.parse(payload);

      if (jsonObject.hasOwnProperty("access_token")) {
        access_token = jsonObject["access_token"];
      }
      if (jsonObject.hasOwnProperty("refresh_token")) {
        if (!refresh_token.equals(jsonObject["refresh_token"]))
        {
          Serial.print("New RefreshToken: ");
          refresh_token = jsonObject["refresh_token"];
          Serial.println(refresh_token);
          persistRefreshToken();
        }
      }
      //      if (jsonObject.hasOwnProperty("expires_in")) {
      //        int expires_in = (int)jsonObject["expires_in"];
      //      }
    }

    else {
      Serial.println("Failed to retrieve Token.");
      access_token = "";
      refresh_token = "";
    }
    http.end();
    loop();
  }
}

String getDeviceCode() {
  String device_code = "";
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;

    http.begin(deviceCodeEndpoint, login_ca);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String requestData = appendRequestData("", "client_id", client_id);
    requestData = appendRequestData(requestData, "scope", "presence.read offline_access");

    int httpCode = http.POST(requestData);
    Serial.print("POST getDeviceCode HTTP-Code: ");
    Serial.println(httpCode);
    String payload = http.getString();
    if (httpCode == 200) {
      JSONVar jsonObject = JSON.parse(payload);

      if (jsonObject.hasOwnProperty("device_code")) {
        device_code = jsonObject["device_code"];
      }
      if (jsonObject.hasOwnProperty("message")) {
        Serial.println(jsonObject["message"]);
      }
      if (jsonObject.hasOwnProperty("expires_in")) {
        int expires_in = (int)jsonObject["expires_in"];
        Serial.print("Expries in ");
        Serial.print(expires_in / 60);
        Serial.println(" min");
      }
    }

    else {
      Serial.println("Failed to retrieve device code.");
    }
    http.end();
  }
  return device_code;
}

String appendRequestData(String in, String key, String value) {
  if (in == "") {
    return (key + "=" + value);
  }
  else {
    return (in + "&" + key + "=" + value);
  }
}

String getSerialInput(int timeout) {
  String incomimgString = "";
  long start = millis();
  while (true) {
    if ( timeout != 0 && (millis() - start > timeout)) {
      return incomimgString;
    }
    if (Serial.available()) {
      incomimgString = Serial.readString();
      return incomimgString;
    }
  }
}

void clearEEPROM(){
  WriteEeprom(TOKEN_EEPROM_ADDR, TOKEN_EEPROM_SIZE, "");
}

void persistRefreshToken() {
  if (refresh_token != "") {
    WriteEeprom(TOKEN_EEPROM_ADDR, TOKEN_EEPROM_SIZE, StringToCharArray(refresh_token));
    Serial.println("Persist RefreshToken to EEPROM");
  }
}

bool recoverRefreshToken() {
  refresh_token = ReadEeprom(TOKEN_EEPROM_ADDR, TOKEN_EEPROM_SIZE);
  if (refresh_token == "") {
    return false;
  }
  else {
    refreshAccessToken();
    if (refresh_token == "") {
      return false;
    }
    return true;
  }
}

char* StringToCharArray(String input) {
  char * output = new char [input.length() + 1];
  strcpy (output, input.c_str());
  return output;
}

void WriteEeprom(int addr, int eepromSize, char* msg) {
  for (int i = 0; i < eepromSize; i++) {
    EEPROM.write(addr, msg[i]);
    addr += 1;
  }
  EEPROM.commit();
}

String ReadEeprom(int addr, int eepromSize) {
  String output = "";
  for (int i = 0; i < eepromSize; i++) {
    byte readValue = EEPROM.read(i + addr);

    if (readValue == 0 || readValue == 0xFF) {
      return output;
    }
    output += char(readValue);
  }
  return output;
}

void colorWipe(uint32_t color) {
  colorWipe(color, 50);
}

void colorWipe(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}
