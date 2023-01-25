#include <DHT_U.h>
#include <DHT.h>

// ----- Situation management ------
#define ALWAYS_ON_MOSE 21
#define ALWAYS_OFF_MOSE 22
#define WORKING_MODE_BY_TEMPERATURE 23
int CurrentStatus;

// ----- buttons-----
#define btnG D1
#define btnR D2
bool IsRedBtnPressed = false;
bool IsGreenBtnPressed = false;
bool IsTwoBtnsPressed = false;
unsigned long LastRedBtnPressed;
unsigned long LastGreenBtnPressed;

void ControllerButtons() // A function returns a number representing which button is pressed.
{
  if(digitalRead(btnG) == HIGH){
    IsGreenBtnPressed = false;
    IsTwoBtnsPressed = false;
  }
  if(digitalRead(btnR) == HIGH){
    IsRedBtnPressed = false;
    IsTwoBtnsPressed = false;
  }

  if((!IsGreenBtnPressed) && (digitalRead(btnG) == LOW) && ((millis() - LastGreenBtnPressed) > 100)){
      IsGreenBtnPressed = true;
      LastGreenBtnPressed = millis();
      CurrentStatus = ALWAYS_ON_MOSE;
  }

  if((!IsRedBtnPressed) && (digitalRead(btnR) == LOW) && ((millis() - LastRedBtnPressed) > 100)){
      IsRedBtnPressed = true;
      LastRedBtnPressed = millis();
      CurrentStatus = ALWAYS_OFF_MOSE;
  }

  if((!IsTwoBtnsPressed) && (IsGreenBtnPressed) && (IsRedBtnPressed)){
      IsTwoBtnsPressed = true;
      CurrentStatus = WORKING_MODE_BY_TEMPERATURE;
  }

}


// ---- temperature Sensor ----
#define DHTPIN D4 
#define DHTTYPE DHT11
float inputTeperature; 

DHT dht(DHTPIN, DHTTYPE);


// ---- fan ----
#define A1B D7
#define A1A D6
unsigned long timeForFanOn;
unsigned long timeForFanOff;
bool IfTheFanIsOn = true;

void FanOn()
{
  digitalWrite(A1A, HIGH);
  digitalWrite(A1B, LOW);
  IfTheFanIsOn =true;  
}

void FanOff()
{
  digitalWrite(A1A, LOW);
  digitalWrite(A1B, LOW);
  IfTheFanIsOn = false;
}

void FanInActiveMode()
{
  if((millis() - timeForFanOn) > 2000)
  {
    FanOff();
  } 
  
  if((millis() - timeForFanOff) > 4000)
  {
    FanOn();
  }

  if(!IfTheFanIsOn){
    timeForFanOn = millis();
  }else {
    timeForFanOff =millis();
  }   
}

void setup() {
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  pinMode(btnG, INPUT_PULLUP);
  pinMode(btnR,  INPUT_PULLUP);
  dht.begin(); 
  Serial.begin(9600);
  wifi_Setup();  
 
  LastGreenBtnPressed = millis();
}

void loop() {
  wifi_loop();
  ControllerButtons();
  switch(CurrentStatus)
  {
    case ALWAYS_ON_MOSE:
      if(IsGreenBtnPressed){
        timeForFanOff = millis();
      }
      FanInActiveMode();
      break;
    case ALWAYS_OFF_MOSE:
      FanOff();
      break;
    case WORKING_MODE_BY_TEMPERATURE:
      float nowTemperature = dht.readHumidity();
      if(nowTemperature > inputTeperature){
        FanInActiveMode();
      }else{
        FanOff();
      }
      break;
  }
  
}
