#include <dht.h>




#define dht_apin A0

#define BLYNK_PRINT SerialUSB


#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>


// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

#define DHTPIN 2          // What digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

//DHT dht(DHTPIN, DHTTYPE);
dht DHT;
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = DHT.humidity;
  float t = DHT.temperature; // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    SerialUSB.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  DHT.read11(dht_apin);
  
  Blynk.virtualWrite(V5, String(h, 0));
  Blynk.virtualWrite(V6, String(t, 1));
}

void tweetOnButtonPress()
{
  float hm = DHT.humidity;
  float tm = DHT.temperature;
  // Invert state, since button is "Active LOW"
  int isButtonPressed = !digitalRead(2);
  if (isButtonPressed) {
    Serial.println("Button is pressed.");

    Blynk.tweet(String("Arduino lab 13 test. Temperature: ") + tm + String("°C. Humidity: ")+ hm +String("%."));
  }
}
BLYNK_WRITE(V7)
{
  float hm = DHT.humidity;
  float tm = DHT.temperature;
  if(param.asInt() == 0)
  {
    Blynk.tweet(String("Arduino lab 13 test. Temperatue: ") + tm + String("°C. Humidity: ")+ hm +String("%."));
  }
}

void setup()
{
  // Debug console
  SerialUSB.begin(9600);

  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  //dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
  // Setup twitter button on pin 2
  pinMode(2, INPUT_PULLUP);
  // Attach pin 2 interrupt to our handler
  attachInterrupt(digitalPinToInterrupt(2), tweetOnButtonPress, CHANGE);
}

void loop()
{
  
  Blynk.run();
  timer.run();
}
