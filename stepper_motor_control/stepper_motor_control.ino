#include "MotorDriver.h"
#include "Encoder.h"
#include "PID.h"

// Enable for a fun multithreading flex!
bool pulse = true;
uint32_t t0 = millis();
uint32_t tt = millis();

PID vibe_pid(25.0, 0.00000001  , 0.0, -255, 255);
uint16_t vibe_position_array[10] = {0,0,0,0,0,0,0,0,0,0};
uint16_t vibe_speed = 0;

void setup_stepper_motor_control(){
  if(pulse){
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
  }

}

void loop_stepper_motor_control(){

  // 240 RPM --> 32 ticks/s
//  vibe.setPower(vibe_pid.step(20 - vibe_speed) + 150);
  auger.update();
  
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
