#include "MotorDriver.h"
#include "Encoder.h"
#include "StepperDriver.h"
#include "PID.h"

MotorDriver vibe;

Encoder encoder;
void encoder_isr(){
  encoder.isr_();
}
const uint8_t screwSensor = P1_6;

StepperDriver auger;
StepperDriver screwPlacer;

bool once;

bool pulse = false;
uint32_t t0 = millis();
uint32_t tt = millis();

PID vibe_pid(25.0, 0.0, 0.0, -255, 255);
int32_t vibe_position_array[10] = {0,0,0,0,0,0,0,0,0,0};
uint16_t vibe_speed = 0;

uint32_t start_time;

const uint8_t outputUR5 = P2_7;


void setup() {
  Serial.begin(9600);

  vibe.begin(P3_0,P2_5);
  encoder.begin(P1_7 , encoder_isr);
  pinMode(screwSensor, INPUT);
  
  pinMode(outputUR5, OUTPUT);

  auger.begin(P5_0, P5_2);
  screwPlacer.begin(P5_6, P6_6);

  if(pulse){
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
  }

  once = true;
}

void loop() {
  if(once){
    start_time = micros();
    once = false;
  }
  
  vibe.setPower(vibe_pid.step(13 - vibe_speed) + 140);
  auger.stepp(100, 1, 2000);

  uint32_t dt = micros() - start_time;
  
  if((dt > 5000000 && dt < 25000000) || (dt > 40000000 && dt < 60000000)){
    auger.halt();
  } else {
    auger.update();
  }
  screwPlacer.update();
  
  uint32_t t1 = millis();
  if((t1 - tt) >= 100){
    for(int i = 8; i >= 0; i--){
      vibe_position_array[i] = vibe_position_array[i + 1];
    }
    vibe_position_array[9] = encoder.get_position();
    
    vibe_speed = (vibe_position_array[9] - vibe_position_array[0]) * 1;   // ticks/s
    Serial.println(vibe_speed);

    if(pulse){
      float f = 0.00025;
      uint16_t T = 1/f;
      uint16_t t = (t1 - t0) % T;
      float cr = cos(2* PI * f * t);
      float cg = cos(2* PI * f * t + PI/3);
      float cb = cos(2* PI * f * t + PI/6);
      (cr > 0) ? analogWrite(RED_LED, 255 * cr) : analogWrite(RED_LED, 0);
      (cg > 0) ? analogWrite(GREEN_LED, 255 * cg) : analogWrite(GREEN_LED, 0);
      (cb > 0) ? analogWrite(BLUE_LED, 255 * cb) : analogWrite(BLUE_LED, 0);
    }
    tt = t1;
  }
  
}
