#include "MotorDriver.h"
#include <Servo.h>
#include "Encoder.h"
#include "StepperDriver.h"
#include "PID.h"

MotorDriver vibe;
const uint8_t solenoid = P3_2;

Servo screwHolder;
Servo lift;

Encoder encoder;
void encoder_isr(){
  encoder.isr_();
}

StepperDriver auger;

bool once;

void grabScrews();
uint8_t grabScrews_index = 0;
uint32_t grabScrews_time = micros();

// Enable for a fun multithreading flex!
bool pulse = true;
uint32_t t0 = millis();
uint32_t tt = millis();

PID vibe_pid(25.0, 0.00000001  , 0.0, -255, 255);
static int32_t vibe_position_array[10] = {0,0,0,0,0,0,0,0,0,0};
uint16_t vibe_speed = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  vibe.begin(P3_0,P2_5);
  encoder.begin(P1_7 , encoder_isr);
  pinMode(solenoid, OUTPUT);

  screwHolder.attach(P3_6);
  lift.attach(P5_7);
  auger.begin(P5_0, P5_2);

  if(pulse){
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
  }

  once = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(once){
     lift.write(40);
     screwHolder.write(64);
//     delayMicroseconds(1000000);
     delay(1000);
     auger.stepp(3200,1,500);
     
     grabScrews();
     once = false;
  }
  vibe.setPower(vibe_pid.step(20 - vibe_speed) + 150);
  auger.stepp(100, 1, 2000);
  grabScrews_frame();

// --------

  auger.update();
  
  uint32_t t1 = millis();
  if((t1 - tt) >= 100){
    for(int i = 8; i >= 0; i--){
      vibe_position_array[i] = vibe_position_array[i + 1];
    }
    vibe_position_array[9] = encoder.get_position();
    
    vibe_speed = (vibe_position_array[9] - vibe_position_array[0]) * 1;   // ticks/s
////    Serial.println(vibe_speed);

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
      tt = t1;
    }
  }
  
}

void grabScrews(){
  grabScrews_time = micros();
  grabScrews_index = 0;
}


void grabScrews_frame(){
  uint32_t t = micros() - grabScrews_time;
  const int START_DELAY = 3000000;
  const int HOLDER_ONE = 70;
  if(t < START_DELAY + 1000000){
    lift.write(40);
    screwHolder.write(HOLDER_ONE);
    return;
  }
  // i = 0
  if(t >= START_DELAY + 1000000 && t < START_DELAY + 2000000){
    lift.write(140);
    digitalWrite(solenoid, HIGH);
    return;
  }
  if(t >= START_DELAY + 2000000 && t < START_DELAY + 2200000){
    screwHolder.write(HOLDER_ONE + 10 + 36 * 0);
    return;
  }
  if(t >= START_DELAY + 2200000 && t < START_DELAY + 3200000){
    digitalWrite(solenoid, LOW);
    lift.write(40);
    return;
  }
  if(t >= START_DELAY + 3200000 && t < START_DELAY + 4200000){
    screwHolder.write(HOLDER_ONE + 30 + 36 * 0);
    return;
  }
  // i = 1
  if(t >= START_DELAY + 4200000 && t < START_DELAY + 5200000){
    lift.write(140);
    digitalWrite(solenoid, HIGH);
    return;
  }
  if(t >= START_DELAY + 5200000 && t < START_DELAY + 5400000){
    screwHolder.write(HOLDER_ONE + 10 + 36 * 1);
    return;
  }
  if(t >= START_DELAY + 5400000 && t < START_DELAY + 6400000){
    digitalWrite(solenoid, LOW);
    lift.write(40);
    return;
  }
  if(t >= START_DELAY + 6400000 && t < START_DELAY + 7400000){
    screwHolder.write(HOLDER_ONE + 30 + 36 * 1);
    return;
  }
  // i = 2
  if(t >= START_DELAY + 7400000 && t < START_DELAY + 8400000){
    lift.write(140);
    digitalWrite(solenoid, HIGH);
    return;
  }
  if(t >= START_DELAY + 8400000 && t < START_DELAY + 8600000){
    screwHolder.write(HOLDER_ONE + 10 + 34 * 2);
    return;
  }
  if(t >= START_DELAY + 8600000 && t < START_DELAY + 9600000){
    digitalWrite(solenoid, LOW);
    lift.write(40);
    return;
  }
  if(t >= 9600000 && t < 10600000){
    screwHolder.write(HOLDER_ONE + 30 + 34 * 2);
    return;
  }
  screwHolder.write(180);
}
