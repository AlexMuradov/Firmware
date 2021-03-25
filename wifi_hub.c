//#include <FS.h>

//#include <WiFi.h>          
#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>        
#include <Servo.h>
#include <ArduinoJson.h>
//#include <DateTime.h>

//extern "C" {
//#include <user_interface.h>
//}

int servo_pin = D3;  


//******************************************************************************************
//// разрешене аналогого порта
//#define A_MAX 1024
//
//// опорное напряжение на котором работает серва
//#define A_VREF 3
//
//// предельные уровни сигнала с сервы
//#define A_VMIN 0.23
//#define A_VMAX 2.56


int lastAngle = -1;
int AngleMatches = 0;

//******************************************************************************************

//#define uS_TO_S_FACTOR 1000000 /* Conversion factor for micro seconds to seconds */ 
//#define TIME_TO_SLEEP 15 /* Time ESP32 will go to sleep (in seconds) */

 int bootCount = 0;

Servo myservo;
int angle = 170;  
HTTPClient http;

//int realAngle() {
//    return map(
//            analogRead(A0), 
//            A_MAX * A_VMIN / A_VREF, 
//            A_MAX * A_VMAX / A_VREF, 
//            0, 180);
//}

void setup() {

    //int vdd33= ADC.readvdd33();
    // Serial.println("Voltage" + vdd33);
    // put your setup code here, to run once:
    Serial.begin(115200);

     
WiFiManager wifiManager;

wifiManager.autoConnect("AM2");
    
}   

 void loop() { 
  if (WiFi.status() == WL_CONNECTED){
    
   // Serial.println("Starting....");
   delay(3000); 

   // int rotateAngle = realAngle();
   // Serial.println(rotateAngle);
   // delay(1000); 

    listenServer();

   //checkServo();
   //doOperationAngelTest();
  }
}


void servoAtach(){
  if (!myservo.attached()){
    myservo.attach(servo_pin);    
  }
}

int listenServer(){
  
  int operation = -1;
  
  http.begin("http://161.35.88.90/api/nokey/status.php");
  int httpCode = http.GET();

  //if (httpCode > 0){ 
      const size_t bufferSize = JSON_OBJECT_SIZE(2);
      
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());

      int result = root["id"]; // 1
      Serial.println(result);

      if (result != lastAngle) {
        lastAngle = result;
      if (result == 1) {
          servoAtach();
          Serial.println("Door is closing...");
          // move from 10 to 170 degrees with an angle of 15
          for(angle = 10; angle < 170; angle += 5){                          
            myservo.write(angle);
            delay(15);    
          }
          Serial.println(angle);
          myservo.detach();
          
          //delay(3000);
        }
      
      else if (result == 0){
        
          servoAtach();
          Serial.println("Door is opening...");
          
          for(angle = 170; angle >= 10; angle -= 5){                                
            myservo.write(angle);
            delay(15);                       
          }
          myservo.detach();
        //  delay(3000);
        }                       
      } 
  //  }
//    else {
//      Serial.println("Server is not responding.");
//    }

    return operation;
}

//************************************************************************************
void checkServo(){

          angle = 10;

          servoAtach();
          // move from 0 to 180 degrees with an angle of 15
          for(angle = 10; angle < 170; angle += 1){                          
            myservo.write(angle);
            delay(15);    
          }
          Serial.println(angle);
          myservo.detach();
          delay(2000);


          servoAtach();
                   
          for(angle = 170; angle >= 10; angle -= 5){                                
            myservo.write(angle);
            delay(5);                       
          }
          Serial.println(angle);
          myservo.detach();
          delay(2000);
}
//************************************************************************************


void doOperationAngelTest(){

//  http.begin("http://jsonplaceholder.typicode.com/users/1");
//  int httpCode = http.GET();
//
//  if (httpCode > 0){ 
//      const size_t bufferSize = JSON_OBJECT_SIZE(2);
//      
//      DynamicJsonBuffer jsonBuffer(bufferSize);
//      JsonObject& root = jsonBuffer.parseObject(http.getString());
//
//      int operation = root["id"]; // 1
//      Serial.println(operation);
//
//    //  int rotateAngle = realAngle();
//    //  Serial.println(rotateAngle);
//
//
//      if (rotateAngle > 150) {
//        servoAtach();
//        for(angle = 170; angle >= 10; angle -= 5){                                
//            myservo.write(angle);
//            delay(5);                       
//          }
//          delay(3000);
//      }
//
//      if (rotateAngle < 150) {
//        servoAtach();
//         // move from 0 to 180 degrees with an angle of 15
//          for(angle = 10; angle < 170; angle += 1){                          
//            myservo.write(angle);
//            delay(15);    
//          }
//          delay(3000);
//      }
//      myservo.detach();
//  }
//  ESP.deepSleep(15e6);
//  //esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
//  //print_wakeup_reason();
}


//void print_wakeup_reason(){
//  esp_sleep_wakeup_cause_t wakeup_reason;
//
//  wakeup_reason = esp_sleep_get_wakeup_cause();
//
//  switch(wakeup_reason){
//    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
//    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
//    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
//    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
//    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
//  }
//}