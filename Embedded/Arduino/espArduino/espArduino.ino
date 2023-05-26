#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <WiFiManager.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

WiFiManager wifiManager;

// Insert your network credentials


// Insert Firebase project API Key
#define API_KEY "AIzaSyAQg815KBuHMIlC4zFU8MP9RrF9QUUDORo"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://embedded-4affd-default-rtdb.asia-southeast1.firebasedatabase.app/"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int intValue;
float floatValue;
int count = 0;
bool signupOK = false;


SoftwareSerial STM(D1, D2); // RX | TX

long last = 0;
long lastFetch = 0;
int SENDING_INTERVAL_MS=3000,FETCHING_INTERVAL_MS=500;

void setup() {
  // put your setup code here, to run once:
  
  pinMode(D2, INPUT);
  pinMode(D1, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  Serial.begin(9600);
  STM.begin(9600);

  Serial.println("waiting for connecting wifi...");
  wifiManager.autoConnect();
  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  Serial.println("connecting to firebase...");
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println("start...");

}

unsigned int rightRotate(unsigned int n, unsigned int d) {
  return (n >> d) | (n << (32 - d));
}

int readInt() {
  unsigned int value = 0;
  for (int i = 0; i < 4; i++) {
    while (!STM.available())
      ;  // poll input until we get a byte
    // value <<= 8;
    value |= STM.read();            // OR in one byte.
    value = rightRotate(value, 8);  // shift 8 bits
  }
  return (int)value;
}


void loop() {
  if (Firebase.ready() && signupOK) {
  // put your main code here, to run repeatedly:
  long now = millis();
  if (now - last >= SENDING_INTERVAL_MS) {
    STM.print("x");
    Serial.println("Sending message...");
    last = now;
  }

  
  if (STM.available() > 0) {
    int temperature = readInt();
    int moisture = readInt();
    int light = readInt();
    
    // // say what you got:
    Serial.printf("From STM: %d, %d, %d (%d) + sending\n\r", temperature, moisture, light, now);

  if (Firebase.RTDB.setInt(&fbdo, "temperature", temperature)) {
      // Serial.println("PASSED Temp");
      // Serial.println("PATH: " + fbdo.dataPath());
      // Serial.println("TYPE: " + fbdo.dataType());
    } else {
      Serial.println("FAILED Temp");
      Serial.println("REASON: " + fbdo.errorReason());
    }

  if (Firebase.RTDB.setInt(&fbdo, "moisture", moisture)) {
      // Serial.println("PASSED Moisture");
      // Serial.println("PATH: " + fbdo.dataPath());
      // Serial.println("TYPE: " + fbdo.dataType());
    } else {
      Serial.println("FAILED Moisture");
      Serial.println("REASON: " + fbdo.errorReason());
    }

  if (Firebase.RTDB.setInt(&fbdo, "light", light)) {
      // Serial.println("PASSED Light");
      // Serial.println("PATH: " + fbdo.dataPath());
      // Serial.println("TYPE: " + fbdo.dataType());
    } else {
      Serial.println("FAILED Light");
      Serial.println("REASON: " + fbdo.errorReason());
    }

  if (Firebase.RTDB.getBool(&fbdo, "/isAuto")) {
      bool AUTO = fbdo.boolData();
      Serial.print("AUTO : ");
      Serial.println(AUTO?"on":"off");
      
      if(moisture<20){
      if (Firebase.RTDB.setInt(&fbdo, "isWatering", true)){
        // Serial.println("PASSED isWatering");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
      }else{
        Serial.println("FAILED isWatering");
        Serial.println("REASON: " + fbdo.errorReason());
      }

      if (Firebase.RTDB.setInt(&fbdo, "isAuto", true)){
        // Serial.println("PASSED isAuto");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
      }else{
        Serial.println("FAILED isAuto");
        Serial.println("REASON: " + fbdo.errorReason());
      }

    }else if(moisture>=20&&AUTO){

      if (Firebase.RTDB.setInt(&fbdo, "isWatering", false)){
        // Serial.println("PASSED isWatering");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
      }else{
        Serial.println("FAILED isWatering");
        Serial.println("REASON: " + fbdo.errorReason());
      }

      if (Firebase.RTDB.setInt(&fbdo, "isAuto", false)){
        // Serial.println("PASSED isAuto");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
      }else{
        Serial.println("FAILED isAuto");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }
  }
     

  }
  

  
  

  if (now - lastFetch >= FETCHING_INTERVAL_MS && now-last >= 1000){
  
    // int t = Firebase.RTDB.getInt(&fbdo, "temperature" );
  if (Firebase.RTDB.getBool(&fbdo, "/isWatering")) {
      bool water_on = fbdo.boolData();
      Serial.print("water : ");
      Serial.println(water_on?"on":"off");
      digitalWrite(D6, water_on);
  }

  if (Firebase.RTDB.getBool(&fbdo, "/isLighting")) {
    bool light_on = fbdo.boolData();
    Serial.print("light : ");
    Serial.println(light_on?"on":"off");
    digitalWrite(D5, light_on);
  }

  


    lastFetch = now;
  }
  }
}
