#include "StepperDriver.h"

static StepperDriver screwPlacer;
static uint32_t ts = micros();

void setup(){
  screwPlacer.begin(P4_1, P4_3);
  delay(2000);
  screwPlacer.stepp(800,1,500);
}

void loop(){
  uint32_t dt = micros() - ts;
  if(dt / 1000000 == 0){
    screwPlacer.stepp(800,1,500);
  } else if(dt / 1000000 == 1){
    screwPlacer.stepp(800,-1,500);
  } else {
    ts = micros();
  }
  screwPlacer.update();
  
}
