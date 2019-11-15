#include "MotorDriver.h"
#include <Stepper.h>
#include <Servo.h>
//#include <TeensyThreads.h>

#define STEPS 513
#define LOOP_DELAY 10

MotorDriver vibe;
Stepper stepper(STEPS, 3, 4, 5, 6);
#define solenoid 0

Servo solenoidAssist;

#define screwHolderOffset 0
Servo screwHolder;

#define screwSensor A5

bool once;
//
//void stepperThread(){
//  while(1){
//    stepper.step(STEPS * 1.5);
//    threads.delay(1000);
//    threads.yield();
//  }
//}

void grabScrews(){
  const int HOLDER_ONE = 65;
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

  vibe.setup(5,6);
  stepper.setSpeed(5); 
  pinMode(solenoid, OUTPUT);

  solenoidAssist.attach(9);
  screwHolder.attach(10);

//  threads.addThread(stepperThread);

  once = true;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(once){
    solenoidAssist.write(40);
    screwHolder.write(180);
//    grabScrews();
    
    once = false;
  }
  Serial.println("hello");
//  Serial.println(analogRead(screwSensor));
  delay(10);
}
