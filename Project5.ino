#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#define pinMerah 2
#define pinHijau 5
#define pinKuning 21

const char* ssid = "wolverine";
const char* pass = "tanpapassword";

String response = "";

DynamicJsonDocument doc(2048);

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 1*20*1000;


void setup() {
  Serial.begin(115200);

  pinMode(pinMerah, OUTPUT);
  pinMode(pinKuning, OUTPUT);
  pinMode(pinHijau, OUTPUT);

  //Initiate Wifi Connect

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.println("");

  //Wait for connect
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print("Wifi Connected with IP : ");
  Serial.println(WiFi.localIP());

}
  

void loop(void) {
  HTTPClient http;
  currentTime = millis();

  if(currentTime - previousTime > timeoutTime){
    Serial.println("Send Request");
    previousTime = currentTime;

    //The API URL
    String request = "https://api.tomorrow.io/v4/timelines?location=-73.98529171943665,40.75872069597532&fields=temperature&timesteps=current&units=metric&apikey=iWAVSMoeUvXkOCGhT2ITZ1J2r28NYc0Q";

    //Start Req
    http.begin(request);

    //Use HTTP Get req
    http.GET();

    //Respone server
    response = http.getString();

    //Parse JSON, read error if any
    DeserializationError error = deserializeJson(doc, response);
    if (error){  
      Serial.print(F("deserializeJson() failed: "));
      Serial.println (error.c_str());
      return;
    }
    JsonObject data_timelines_0 = doc["data"]["timelines"][0];
    const char* data_timelines_0_intervals_0_startTime = data_timelines_0["intervals"][0]["startTime"];
    float data_timelines_0_intervals_0_values_temperature = data_timelines_0["intervals"][0]["values"]["temperature"];
    Serial.print("Waktu :");
    Serial.println(data_timelines_0_intervals_0_startTime);
    Serial.print("Temperatur :");
    Serial.println(data_timelines_0_intervals_0_values_temperature);
//    float suhu = doc ["data"]["timelines"]["intervals"]["values"]["temperature"];

    if (data_timelines_0_intervals_0_values_temperature < 10){
      digitalWrite(pinHijau, HIGH);
    }else if (data_timelines_0_intervals_0_values_temperature < 20){
      digitalWrite(pinKuning, HIGH);
    }else {
      digitalWrite(pinMerah, HIGH);
    }
//    Serial.println(suhu);
    //close connection
    http.end();
  }
}
