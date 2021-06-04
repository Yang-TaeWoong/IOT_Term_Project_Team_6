#include <AWS_IOT.h> 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <WiFi.h>
#include <Arduino_JSON.h>

#define OLED_MOSI 13 // 9
#define OLED_CLK 14 //10
#define OLED_DC 26 //11
#define OLED_CS 12 //12
#define OLED_RESET 27 //13
Adafruit_SH1106 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH 16

#if (SH1106_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SH1106.h!");
#endif

AWS_IOT hornbill;
const char* ssid = ""; 
const char* password = ""; 
char HOST_ADDRESS[]= ""; 
char CLIENT_ID[]= "ChoiESP32"; 

char sTOPIC_NAME[]= "esp32/hub"; 
char pTOPIC_NAME[]= "esp32/button";   // publish topic name

int status = WL_IDLE_STATUS; 

int msgCount=0,msgReceived = 0; 

char payload[512]; 
char rcvdPayload[512]; 


const int ledPin = 16;
const int ledPin2 = 17;
const int ledPin3 = 5;
const int freq = 5000;
const int ledChannel1 = 1;
const int ledChannel2 = 2;
const int ledChannel3 = 3;

const int resolution = 8;

const int buzzChannel = 4;
const int buzPin = 23;
const int duty = 128;

const int buttonPin = 15;  // pushbutton pin 

unsigned long preMil = 0; 
const long intMil = 5000; 

int sVal;

int dur[4] = {0 ,250,125,60};

int module_number=0;
int module_time[5] = {0,0,0,0,0};

const int interval = 10000;

/*========================================================================*/
// USE_SUBSCRIBE_HANDLER FOR RCVPAYLOAD
void mySubCallBackHandler(char* topicName, int payLoadLen, char* payLoad){
  strncpy(rcvdPayload,payLoad,payLoadLen);
  rcvdPayload[payLoadLen]=0;
  msgReceived = 1;
}
/*========================================================================*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); //++choi This is here to force the ESP32 to reset the WiFi and initialise correctly. 
  Serial.print("WIFI status = ");
  Serial.println(WiFi.getMode());
  WiFi.disconnect(true); 
  delay(1000); 
  WiFi.mode(WIFI_STA); 
  delay(1000); 
  Serial.print("WIFI status = "); 
  Serial.println(WiFi.getMode()); //++choi 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) { 
    delay(1000); 
    Serial.println("Connecting to WiFi.."); 
   } 
  Serial.println("Connected to wifi");
  if(hornbill.connect(HOST_ADDRESS,CLIENT_ID)== 0) { 
    Serial.println("Connected to AWS"); 
    delay(1000); 
    if(0== hornbill.subscribe(sTOPIC_NAME,mySubCallBackHandler)){
      Serial.println("Subscribe Successfull");
    }
    else{
      Serial.println("Subscribe Fail");
      while(1);
    }
  } 
  else { 
    Serial.println("AWS connection failed, Check the HOST Address"); 
    while(1); 
  }
  
  display.begin(SH1106_SWITCHCAPVCC);
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(200);
  display.clearDisplay();

  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  
  pinMode(buttonPin, INPUT); 
  
  ledcSetup(ledChannel1, freq, resolution);
  ledcSetup(ledChannel2, freq, resolution);
  ledcSetup(ledChannel3, freq, resolution);
  ledcAttachPin(ledPin, ledChannel1);
  ledcAttachPin(ledPin2, ledChannel2);
  ledcAttachPin(ledPin3, ledChannel3);
  
  ledcAttachPin(buzPin, buzzChannel);
}
/*========================================================================*/
/* LED_CONTROL*/
void ledOn(int module_number){
  
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle+=4) {
    // changing the LED brightness with PWM
    ledcWrite(module_number, dutyCycle);
    delay(15);
  }
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle-=4) {
    ledcWrite(module_number, dutyCycle);
    delay(15);
  }
}
void ledOff(int number){
  digitalWrite(number,LOW);
}
/*========================================================================*/
/* BUZZ_PLAY_CONTROL*/
void playbuzz(int buzz_counting){
  ledcSetup(buzzChannel,294,resolution);
  ledcWrite(buzzChannel,duty);
  delay(dur[buzz_counting]);
}
/*========================================================================*/

int buzz_index[4] = {0,0,0,0};
int buzz_counting = 0;

void draw_alarm(int number){
  display.setTextSize(3);
  display.setCursor(0,20);
  
  if(number == 1){
    display.setTextColor(WHITE);
    display.println("1 FIRE!");
    display.display();
  }
  else if(number==2){
    display.setTextColor(WHITE);
    display.println("2 FIRE!");
    display.display();
  }
  else if(number==3){
    display.setTextColor(WHITE);
    display.println("3 FIRE!");
    display.display();
  }
  else{
    display.setTextColor(GREEN);
    display.println("DONE");
    display.display();
  }
  
  delay(2000);
  display.clearDisplay();
  
}
void loop() {
  if(msgReceived == 1) { 
    msgReceived = 0; 
    Serial.print("Received Message:"); 
    Serial.println(rcvdPayload); 
    JSONVar object = JSON.parse(rcvdPayload);
    JSONVar state = object["state"];
    module_number = (int)state["module"];
    Serial.println(module_number);
    buzz_index[module_number] = 1;
    module_time[module_number] = millis();   
    draw_alarm(module_number);
    module_number=0;
  } 
  if(msgReceived==0){
    display.setTextSize(3);
    display.setCursor(0,20);
    display.setTextColor(GREEN);
    display.println("DONE");
    display.display();
  }
  for(int i=1; i <= sizeof(module_number)/sizeof(int)-1;++i){
    if(buzz_index[i] == 1 && buzz_counting != 4){
      buzz_counting += 1;
    }
  }
  playbuzz(buzz_counting);

  for(int i=1; i < sizeof(module_time)/sizeof(int)-1; ++i){
    if(millis() - module_time[i] >= interval){
      ledOff(i); 
      buzz_index[i] = 0;
    }
    else{
      ledOn(i);
    }
  }
  
  if((millis()-preMil) > intMil) { 
    if (digitalRead(buttonPin)) { 
      preMil = millis(); 
      sprintf(payload,"현재 %d번 지역에서 산불이 발생했습니다!!",module_number); 
      if(hornbill.publish(pTOPIC_NAME,payload) == 0) {        
        Serial.print("Publish Message:"); 
        Serial.println(payload); 
      } 
      else 
      Serial.println("Publish failed"); 
    } 
  }
}
