/*******************************************************************
    A sample project for making a HTTP/HTTPS GET request on an ESP32

    It will connect to the given request and print the body to
    serial monitor

    Parts:
    ESP32 Dev Board
       Aliexpress: * - https://s.click.aliexpress.com/e/_dSi824B
       Amazon: * - https://amzn.to/3gArkAY

 *  * = Affilate

    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/


    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/


// ----------------------------
// Standard Libraries
// ----------------------------

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
//------- Replace the following! ------
char ssid[] = "Palm2704";       // your network SSID (name)
char password[] = "";  // your network key

// For Non-HTTPS requests
// WiFiClient client;

// For HTTPS requests
WiFiClientSecure client;

float heightsWater[50];
int q = 0;
// Just the base of the URL you want to connect to
#define TEST_HOST "www.worldtides.info"


// Root cert of server we are connecting to
// Baltimore CyberTrust Root - Expires 12 May 2025
// (FYI, from a security point of view you should not trust certs from other people, including me!)
const char *server_cert = "-----BEGIN CERTIFICATE-----\n"
                                  "MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n"
                                  "RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n"
                                  "VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n"
                                  "DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n"
                                  "ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n"
                                  "VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n"
                                  "mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n"
                                  "IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n"
                                  "mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n"
                                  "XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n"
                                  "dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n"
                                  "jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n"
                                  "BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n"
                                  "DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n"
                                  "9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n"
                                  "jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n"
                                  "Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n"
                                  "ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n"
                                  "R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n"
                                  "-----END CERTIFICATE-----\n";


void setup() {

  Serial.begin(115200);

  // Connect to the WiFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //--------

  // Checking the cert is the best way on an ESP32
  // This will verify the server is trusted.
  client.setCACert(server_cert);

  // If you don't want to verify the server
  // Unlike the fingerprint method of the ESP8266 which expires frequently
  // the cert lasts years, so I don't see much reason to ever
  // use this on the ESP32
   client.setInsecure();

  makeHTTPRequest();
}

void makeHTTPRequest() {

  // Opening connection to server (Use 80 as port if HTTP)
  if (!client.connect(TEST_HOST, 443))
  {
    Serial.println(F("Connection failed"));
    return;
  }

  // give the esp a breather
  yield();

  // Send HTTP request
  client.print(F("GET "));
  // This is the second half of a request (everything that comes after the base URL)
  client.print("/api/v3?heights&today&lat=21.347724&lon=-105.245877&localtime&datum=CD&key=8688cbf2-4b2a-4e50-9bb5-9a146976dd69"); // %2C == ,
  client.println(F(" HTTP/1.1"));

  //Headers
  client.print(F("Host: "));
  client.println(TEST_HOST);

  client.println(F("Cache-Control: no-cache"));

  if (client.println() == 0)
  {
    Serial.println(F("Failed to send request"));
    return;
  }
  //delay(100);
  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0)
  {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders))
  {
    Serial.println(F("Invalid response"));
    return;
  }

  // This is probably not needed for most, but I had issues
  // with the Tindie api where sometimes there were random
  // characters coming back before the body of the response.
  // This will cause no hard to leave it in
  // peek() will look at the character, but not take it off the queue
  while (client.available() && client.peek() != '{')
  {
    char c = 0;
    client.readBytes(&c, 1);
    Serial.print(c);
    Serial.println("BAD");
  }

  // While the client is still availble read each
  // byte and print to the serial monitor
//  while (client.available()) {
//    char c = 0;
//    client.readBytes(&c, 1);
 //   Serial.print(c);
//  }
DynamicJsonDocument doc(12288);

DeserializationError error = deserializeJson(doc, client);

if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return;
}

int Status = doc["status"]; // 200
int callCount = doc["callCount"]; // 1
const char* copyright = doc["copyright"]; // "Tidal data retrieved from www.worldtides.info. Copyright ...
double requestLat = doc["requestLat"]; // 21.347724
double requestLon = doc["requestLon"]; // -105.245877
float responseLat = doc["responseLat"]; // 21.3333
float responseLon = doc["responseLon"]; // -105.333
const char* atlas = doc["atlas"]; // "TPXO"
const char* timezone = doc["timezone"]; // "Etc/GMT-7"
const char* requestDatum = doc["requestDatum"]; // "CD"
const char* responseDatum = doc["responseDatum"]; // "CD"

for (JsonObject height : doc["heights"].as<JsonArray>()) {

  long height_dt = height["dt"]; // 1668668400, 1668670200, 1668672000, 1668673800, 1668675600, ...
  const char* height_date = height["date"]; // "2022-11-17T00:00:00-07:00", "2022-11-17T00:30:00-07:00", ...
    float height_height = height["height"]; // 0.704, 0.749, 0.802, 0.861, 0.923, 0.986, 1.045, 1.098, ...
 //heightsWater[q]= height_height;
 //q++;
Serial.print(height_height); 
//Serial.print(height_date);
}

}

void loop() {
  // Stream& input;



}
