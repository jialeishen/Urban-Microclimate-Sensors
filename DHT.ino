#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#include <stdio.h>
#include <string.h>
#include <DS1302.h>

#define DHTPIN 2     // RH, T read in pin
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

File dataFile;
String fileName = "data.txt";

uint8_t CE_PIN   = 5;
uint8_t IO_PIN   = 8;
uint8_t SCLK_PIN = 9;

char buf[50];
char day[10];
String comdata = "";
int numdata[7] ={0}, j = 0, mark = 0;
DS1302 rtc(CE_PIN, IO_PIN, SCLK_PIN);

DHT dht(DHTPIN, DHTTYPE);
const int chipSelect = 4;   //TF module CS pin

void setup() {
  Serial.begin(9600);
  rtc.write_protect(false);
  rtc.halt(false);
  Serial.println("DHT22 test!");
  pinMode(10, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  while(digitalRead(3)){}
  
  Serial.println("Initializing SD card");
  if (!SD.begin(chipSelect))
  {
    Serial.println("initialization failed!");
    digitalWrite(6, HIGH);
    while(1);
  }
  Serial.println("initialization done.");
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  delay(100);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);

  dataFile = SD.open(fileName, FILE_WRITE);
  dataFile.println("Start recording...");
  dataFile.close();
  dataFile = SD.open(fileName, FILE_WRITE);
  dataFile.println("Week Date Time Humidity(%) Temperature(*C)");
  dataFile.close();
  dht.begin();
}

void loop() {
  float H = dht.readHumidity();
  float T = dht.readTemperature();

  Time t = rtc.time();
  memset(day, 0, sizeof(day));
  switch (t.day)
  {
    case 1: strcpy(day, "Sunday"); break;
    case 2: strcpy(day, "Monday"); break;
    case 3: strcpy(day, "Tuesday"); break;
    case 4: strcpy(day, "Wednesday"); break;
    case 5: strcpy(day, "Thursday"); break;
    case 6: strcpy(day, "Friday"); break;
    case 7: strcpy(day, "Saturday"); break;
  }
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d %02d:%02d:%02d ", day, t.yr, t.mon, t.date, t.hr, t.min, t.sec);

  if (isnan(H) || isnan(T) ) {
    Serial.println("Failed to read from DHT sensor!");
    digitalWrite(6, HIGH);
    delay(100);
    digitalWrite(6, LOW);
    delay(100);
    return;
  }
  String dataString = "";
  String dataStringShow = "";
  dataStringShow = buf + String(H) + "%RH " + String(T) + "*C";
  dataString = buf + String(H) + String(T);
  dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    
    Serial.println(dataStringShow);
    Serial.println("writing...");
    digitalWrite(7, HIGH);
    delay(500);
    digitalWrite(7, LOW);
  }
  else
  {
    Serial.println("error opening data.txt");
    digitalWrite(6, HIGH);
    delay(500);
    digitalWrite(6, LOW);
  }
  delay(2000);
}
