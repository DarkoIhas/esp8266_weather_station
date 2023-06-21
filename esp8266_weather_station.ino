#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "TMPL1oK5tYaK"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "Z47wBFmHbT3i9Q496Uay2JYgKl0Ku7LJ"
#define BLYNK_PRINT Serial
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define DHTPIN D4
#define DHTTYPE DHT11


char auth[] = BLYNK_AUTH_TOKEN;
const char* ssid = "JohnGrammaticus";
const char* password = "3dxnurbs";

const char* ntpServerName = "pool.ntp.org";
const int timeZone = 2;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServerName, timeZone * 3600, 60000);

BlynkTimer timer;

DHT dht(DHTPIN, DHTTYPE);
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define NUMFLAKES     10 // Number of snowflakes in the animation example
#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();
  Blynk.begin(auth, ssid, password);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
}

void loop() {
    float temperature = dht.readTemperature();  // Read temperature value from DHT11
    int humidity = dht.readHumidity(); // Read humidity value from DHT11

    Blynk.virtualWrite(V0, temperature); // Define virtual pins to use with Blynk app
    Blynk.virtualWrite(V1, humidity);


    char floatStr[5]; 
    dtostrf(temperature, 4, 1, floatStr); // Display temperature readings with only one number after the comma

      if (isnan(temperature) || isnan(humidity)) {      // Message to display if reading from the sensorsa fails
    Serial.println("failed to read from DHT sensor!");
    return;
  }

    timeClient.update();                // Read the current time
    int hour = timeClient.getHours();     // Hours value
    int minute = timeClient.getMinutes();  // Minutes value

    Blynk.run();  


      display.clearDisplay();   // Clear the buffer

      // Display digital clock
      display.setTextSize(2);            
      display.setCursor(35,0);   
      display.setTextColor(SSD1306_WHITE);
      if (hour < 10) {
        display.print('0');
      }
      display.print(hour);
      display.print(":");
      if (minute < 10) {
        display.print('0');                     
      }
      display.print(minute);

      // Display temparature value
      display.setTextSize(2);             
      display.setTextColor(SSD1306_WHITE);       
      display.setCursor(0,25);            
      display.print("Temp:");
      display.print(floatStr);
      display.print("c");

      // Display humidity value
      display.setTextSize(2);             
      display.setTextColor(SSD1306_WHITE);        
      display.setCursor(0,50);             
      display.print("Hum:");
      display.print(humidity);
      display.print("%");

      display.display();
      delay(30000);  // Read every 30sec
}






