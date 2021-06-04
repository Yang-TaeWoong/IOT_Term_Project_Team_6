// IOT_Project Fire Detecting Sensor 
#include <WiFi.h>
#include <AWS_IOT.h>
#include <Arduino_JSON.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <Servo.h>

#define SEA_LEVEL_PRESSURE_HPA (1024.5)

/*** Networking Variable ***/
AWS_IOT hornbill;
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PW";
char HOST_ADDRESS[] = "HOST_ADDRESS";
char CLIENT_ID[] = "FD_Sensor";
char pTOPIC_NAME[] = "esp32/alarm";

int msgCount = 0, msgReceived = 0;
char payload[512];
char rcvdPayload[512];

const int SENSOR_ID = 1; // 1, 2, 3

/*** BME680 Variable ***/
#define BME_CS 5
Adafruit_BME680 bme(BME_CS); // hardware SPI
int delayTime;

/*** Piezo Variable ***/
const int ledChannel = 0;
const int resolution = 8;
const int buzPin = 16;
const int duty = 128;

int nFrq[] = {262, 294, 330, 349, 392, 440, 494, 523, 0};

/*** Clock Variable ***/
unsigned long preMil = 0;
const long intMil = 5000; // 30s

/*** Servo motor Variable ***/
static const int servoPin = 17;
Servo servo;
int pos = 0;

/*** Condition ctrl ***/
bool currentCond = false;
bool prevCond = false;

void playNote(int note) {
  if(note == -1) {
    ledcSetup(ledChannel, 0, resolution);
    ledcWrite(ledChannel, 0);  
  } else {
    ledcSetup(ledChannel, nFrq[note], resolution);
    ledcWrite(ledChannel, duty);  
  }
  delay(1000);
}

void mySubCallBackHandler(char* topicName, int payloadLen, char* payLoad)
{
  strncpy(rcvdPayload,payLoad,payloadLen);
  rcvdPayload[payloadLen] = 0; 
  msgReceived = 1; 
}

void openDoor()
{
  servo.attach(servoPin);
  servo.write(0);
  delay(2000);
  servo.detach();
}

void closeDoor()
{
  servo.attach(servoPin);
  servo.write(180);
  delay(2000);
  servo.detach();
}

bool isChange(bool prev, bool cur)
{
  return prev ^ cur;
}

void setup() {
  bool status;
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("BME680 test"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  servo.attach(servoPin);
  servo.write(pos);
  delay(2000);

  servo.detach();

  ledcAttachPin(buzPin, ledChannel);
  playNote(-1);
  
  Serial.println("--Default Test--");
  delayTime = 1000;
  Serial.println();

  Serial.print("WIFI status = ");
  Serial.println(WiFi.getMode());
  WiFi.disconnect(true); 
  delay(1000); 
  WiFi.mode(WIFI_STA); 
  delay(1000); 
  Serial.print("WIFI status = "); 
  Serial.println(WiFi.getMode()); //++choi 
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi"); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000); 
    Serial.print(".");
  }
  Serial.println("\nConnected to wifi");
  
  if(hornbill.connect(HOST_ADDRESS,CLIENT_ID)== 0) { 
    Serial.println("Connected to AWS"); 
    delay(1000);
  } 
  else { 
    Serial.println("AWS connection failed, Check the HOST Address"); 
    while(1); 
  } 

  delay(2000);
}

void loop() {
  float temp = bme.temperature;
  float humid = bme.humidity;
  float press = bme.pressure / 100.0;
  float altit = bme.readAltitude(SEA_LEVEL_PRESSURE_HPA);
  float gas = bme.gas_resistance / 1000.0;

  //currentCond = (temp >= 80 && humid <= 20 && gas <= 100); // 실제 화재 상황
  currentCond = (temp >= 40 && humid >= 10 && gas <= 300);   // 시연 화재 상황
  
  if(isChange(currentCond,prevCond)) {
    if(currentCond){
      playNote(6);
      closeDoor();
    } else {
      playNote(-1);
      openDoor();
    }
  }

  if((millis()-preMil) >= intMil) {
    preMil = millis();
    printValues(temp, humid, press, gas, altit);
    sprintf(payload,"{\"number\":%d,\"temp\":%.2f,\"humid\":%.2f,\"press\":%.2f,\"gas\":%.2f,\"altit\":%.2f}",SENSOR_ID,temp,humid,press,gas,altit);
    if(hornbill.publish(pTOPIC_NAME,payload) == 0) {        
        Serial.print("Publish Message:"); 
        Serial.println(payload);
    } else {
      Serial.println("Publish failed");
    }
  }

  prevCond = currentCond;
}

void printValues(float temp, float humid, float press, float gas, float altit) {
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *C");

  Serial.print("Humidity = ");
  Serial.print(humid);
  Serial.println(" %");

  Serial.print("Pressure = ");
  Serial.print(press);
  Serial.println(" hPa");

  Serial.print("Gas = ");
  Serial.print(gas);
  Serial.println(" KOhms");

  Serial.print("Approx. Altitude = ");
  Serial.print(altit);
  Serial.println(" m");
}
