#include <SPI.h>
#include <SD.h>
#include "DHT.h"

#define DHTPIN 2     // RH, T read in pin
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);
const int chipSelect = 4;   //TF module CS pin

void setup() {
  Serial.begin(9600);
  Serial.println("DHT22 test!");
  pinMode(10, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  while(digitalRead(3)){}
  
  Serial.println("Initializing SD card");
  if (!SD.begin(chipSelect))
  {
    Serial.println("initialization failed!");
    digitalWrite(6, HIGH);
    while(1);
  }
  Serial.println("initialization done.");
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(9, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(9, LOW);
  delay(100);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(9, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(9, LOW);


  dht.begin();
}

void loop() {
  Serial.println("Open file and write data");
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    digitalWrite(7, HIGH);
    return;
  }
  String dataString = "";
  dataString = "Humidity: " + String(h) + " %, " + "Temperature: " + String(t) + " *C";
  File dataFile = SD.open("DATA.txt", FILE_WRITE);
  if (dataFile)
  {
    dataFile.println(dataString);
    Serial.println("writing...");
    dataFile.close();
    digitalWrite(9, HIGH);
    delay(500);
    digitalWrite(9, LOW);
  }
  else
  {
    Serial.println("error opening DATA.txt");
    digitalWrite(7, HIGH);
    delay(500);
    digitalWrite(7, LOW);
  }
  Serial.println("Wait for next loop");
  delay(2000);
}
