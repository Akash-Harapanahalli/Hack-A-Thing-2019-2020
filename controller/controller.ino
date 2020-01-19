#include "MotorDriver.h"
#include <Stepper.h>
#include <Servo.h>
#include <TeensyThreads.h>

#define STEPS 513
#define LOOP_DELAY 10

MotorDriver vibe;
Stepper stepper_auger(STEPS, 3, 4, 5, 6);
Stepper stepper_screwholder(STEPS, 23, 22, 21, 20);
#define solenoid 25

Servo solenoidAssist;

#define screwHolderOffset 0
Servo screwHolder;

#define screwSensor A4

bool once;

#define HALTED 0
#define GOING  1
int screwHolderState = 0;

void stepper_augerThread(){
  while(1){
    stepper_auger.step(100000);
    stepper_screwholder.step(100000);
    threads.delay(1000);
    threads.yield();
  }
}

void grabScrews(){
  const int HOLDER_ONE = 64;
  solenoidAssist.write(40);
  screwHolder.write(HOLDER_ONE);
  
  for(int i = 0; i < 3; i++){
    delay(1000);
    solenoidAssist.write(120);
    digitalWrite(solenoid, HIGH);
    delay(1000);
//    for(int j = 15; j >= 1; j--){
//      screwHolder.write(HOLDER_ONE + 15 + 32 * i - j);
//      delay(20);
//    }
    
    screwHolder.write(HOLDER_ONE + 10 + 32 * i);
    delay(200);
    digitalWrite(solenoid, LOW);
    solenoidAssist.write(40);
    delay(1000);
    screwHolder.write(HOLDER_ONE + 30 + 32 * i);
  }
  screwHolder.write(180);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  vibe.setup(22,23);
  stepper_auger.setSpeed(10); 
  stepper_screwholder.setSpeed(5);
  pinMode(solenoid, OUTPUT);

  solenoidAssist.attach(9);
  screwHolder.attach(10);

  threads.addThread(stepper_augerThread);

  once = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  vibe.setPower(165);
  if(once){
    solenoidAssist.write(40);
    screwHolder.write(64);
    while(analogRead(screwSensor) > 800) delay(10);
    delay(1000);
    grabScrews();
    
    once = false;
  }
//  vibe.setPower(150);
    Serial.println(analogRead(screwSensor));
  delay(10);
}
