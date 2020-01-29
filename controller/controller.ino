#include "MotorDriver.h"
#include <Servo.h>
#include "Encoder.h"
#include "StepperDriver.h"
#include "PID.h"

MotorDriver vibe;

Servo screwHolder;
Servo lift;

Encoder encoder;
void encoder_isr(){
  Serial.print("hh");
  encoder.isr_();
}
const uint8_t screwSensor = P1_6;

StepperDriver auger;
StepperDriver screwPlacer;

bool once;

void grabScrews();
void grabScrews_frame();
uint32_t grabScrews_time = micros();

void placeScrew();
void placeScrew_frame();
uint32_t placeScrew_time = micros();

// Enable for a fun multithreading flex!
bool pulse = false;
uint32_t t0 = millis();
uint32_t tt = millis();

PID vibe_pid(25.0, 0.0000000  , 0.0, -255, 255);
int32_t vibe_position_array[10] = {0,0,0,0,0,0,0,0,0,0};
uint16_t vibe_speed = 0;


/**
 * --------------------------------------------------------------UR-5 IO--------------------------------------------------------------
 * 
 * The UR-5 cannot communicate with the MSP-432 over digital io due to voltage thresholds. 
 * As a result, te UR-5 will actually be using ports to communicate with the MSP.
 * We should see virtually no difference besides requiring the read voltage to be higher than a certain number (probably associated with around 1.5V).
 * 
 * {
 * When the screw is ready to be picked up, the MSP will write HIGH.
 * When UR-5 reads HIGH, it will pick up the screw. 
 * When UR-5 reaches a safe location, it will change states to HIGH and back to LOW.
 * When the MSP reads FALLING, it will trigger the stepper action to put the screw safely onto the bit.
 * The UR-5 will continue to place the screw into the jig.
 * When the MSP reads LOW off of UR-5 and LOW off of photodiode, it will pick up a new screw.
 * The UR-5 will wait at the safe location.
 * }
 * 
 */
bool prev_inputUR5 = 0;
const uint8_t inputUR5  = P3_3;
const uint8_t outputUR5 = P2_7;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  vibe.begin(P3_0,P2_5);
  encoder.begin(P1_7 , encoder_isr);
  pinMode(screwSensor, INPUT);
  
  pinMode(inputUR5,  INPUT_PULLUP);
  pinMode(outputUR5, OUTPUT);

  screwHolder.attach(P3_2);
  lift.attach(P5_7);
  
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
  // put your main code here, to run repeatedly:
  if(once){
     lift.write(40);
     screwHolder.write(106);
     auger.stepp(3200,1,500);
     
     once = false;
  } 

  if(digitalRead(screwSensor) && !digitalRead(inputUR5)){
    grabScrews();
  }
  
  vibe.setPower(vibe_pid.step(20 - vibe_speed) + 175);
  auger.stepp(100, 1, 2000);
  
  grabScrews_frame();
  placeScrew_frame();

  auger.update();
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
      tt = t1;
    }
  }
  
}

void grabScrews(){
  if((micros() - grabScrews_time) > 3200000){
    grabScrews_time = micros();
  }
}

void placeScrew(){
  if((micros() - grabScrews_time) > 3200000){
    grabScrews_time = micros();
  }
}


void grabScrews_frame(){
  uint32_t t = micros() - grabScrews_time;
  const int START_DELAY = 0;
  const int HOLDER_ONE = 135;
  if(t < START_DELAY + 1000000){
    lift.write(40);
    screwHolder.write(HOLDER_ONE);
    return;
  }
  if(t >= START_DELAY + 1000000 && t < START_DELAY + 2000000){
    lift.write(140);
    return;
  }
  if(t >= START_DELAY + 2000000 && t < START_DELAY + 2200000){
    screwHolder.write(180);
    return;
  }
  if(t >= START_DELAY + 2200000 && t < START_DELAY + 3200000){
    lift.write(40);
    return;
  }
  if(t >= START_DELAY + 3200000 && t < START_DELAY + 3450000){
    screwHolder.write(180);
    digitalWrite(outputUR5, HIGH);
    return;
  }
  return;
}


void placeScrew_frame(){

}
