/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  WARNING :
  For this example you'll need Adafruit DHT sensor libraries:
    https://github.com/adafruit/Adafruit_Sensor
    https://github.com/adafruit/DHT-sensor-library

  App project setup:
    Value Display widget attached to V10
    Value Display widget attached to V11
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "ZEaZNYynXRCnHClUHkT5lg9vdUFXGgNK"; 
//char ssid[] = "TP-LINK_C9E28E";
//char pass[] = "QWer852654";

char auth[] = "gDML_Fz0dxcoWKobDY8msGy_FqFC9EYs"; 
char ssid[] = "TP-LINKC9E28E";
char pass[] = "QWer852654";

#define DHTPIN 2          // What digital pin we're connected to
#define BLYNK_GREEN     "#23C48E"
#define BLYNK_BLUE      "#04C0F8"
#define BLYNK_DARK_BLUE "#5F7CD8"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_RED       "#D3435C"


// Uncomment whatever type you're using!
//#define DHTTYPE DHT11     // DHT 11
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

bool ledStatus = false;


DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
BlynkTimer ErrBlink;
WidgetLED ErrLed(V9);

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.

void blinkLedWidget()
{
  if (ledStatus) {
    ErrLed.setValue(175);
    ledStatus = false;
  } else {
    ErrLed.setValue(255);
    ledStatus = true;
  }
}


void sendSensor()
{
  
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
  long rssi = WiFi.RSSI();
  if (isnan(h) || isnan(t) || h > 100) {
  //if (isnan(h) || isnan(t) ) {
    //Serial.println("Failed to read from DHT sensor!");
    ErrLed.setColor(BLYNK_RED);
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.

  ErrLed.setColor(BLYNK_GREEN);
  Blynk.virtualWrite(V10, t-2);
  Blynk.virtualWrite(V11, h);
  Blynk.virtualWrite(V8, rssi);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "connect.blynk-cloud.ml", 9443);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,88,252), 8080);

  dht.begin();  
  // Setup a function to be called every second
  timer.setInterval(3000L, sendSensor);
  ErrBlink.setInterval(1000L, blinkLedWidget);
  ErrLed.on();
}

void loop()
{
  Blynk.run();
  timer.run();
  ErrBlink.run();
}
