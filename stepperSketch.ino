#include <ESP_FlexyStepper.h>

// IO pin assignments
const int MOTOR_STEP_PIN = 33;
const int MOTOR_DIRECTION_PIN = 25;
const int EMERGENCY_STOP_PINB = 26; //define the IO PIN the emergency stop switch is connected to
const int EMERGENCY_STOP_PINT= 32;
// Speed settings
const int DISTANCE_TO_TRAVEL_IN_STEPS = 500;
const int SPEED_IN_STEPS_PER_SECOND = 1000;
const int ACCELERATION_IN_STEPS_PER_SECOND = 800;
const int DECELERATION_IN_STEPS_PER_SECOND = 800;

// create the stepper motor object
ESP_FlexyStepper stepper;




void setup() {
  //Serial.begin(57600);
 Serial.begin(115200);
  pinMode(EMERGENCY_STOP_PINB, INPUT_PULLUP);
  pinMode(EMERGENCY_STOP_PINT, INPUT_PULLUP);
  
  // put your setup code here, to run once:
 stepper.connectToPins(MOTOR_STEP_PIN, MOTOR_DIRECTION_PIN);
  stepper.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  stepper.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  stepper.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);
  stepper.startAsService();
   //digitalWrite(enablePin, LOW);
  stepper.setSpeedInStepsPerSecond(2000);
  stepper.setAccelerationInStepsPerSecondPerSecond(2000);
 
  /*
  while(digitalRead(EMERGENCY_STOP_PINB)){
    stepper.moveRelativeInSteps(1000);
  }
   stepper.emergencyStop();
   stepper.moveToPositionInSteps(-400);
   stepper.setCurrentPositionAsHomeAndStop();
   stepper.moveToPositionInSteps(-400);
   stepper.setCurrentPositionAsHomeAndStop();
   stepper.setCurrentPositionInMillimeters(0);
   stepper.setStepsPerMillimeter(5.5);
   //digitalWrite(enablePin, HIGH);
   */
  if( stepper.moveToHomeInMillimeters(1,1000.0, 6000,26)){
    stepper.moveToPositionInMillimeters(-1000);
  }
  float blinky = stepper.getCurrentPositionInSteps();
  Serial.println(blinky);
   if( stepper.moveToHomeInMillimeters(-1,1000.0, 6000,32)){
    stepper.moveToPositionInMillimeters(1000);
  }
  float plinky = stepper.getCurrentPositionInSteps();
  Serial.println(plinky);
}

void loop() {
  /*
  // put your main code here, to run repeatedly:
 while(digitalRead(EMERGENCY_STOP_PINB)){
    stepper.moveRelativeInSteps(1000);
  }
   stepper.emergencyStop();
   stepper.moveToPositionInSteps(-400);
    while(digitalRead(EMERGENCY_STOP_PINT)){
    stepper.moveRelativeInSteps(-1000);
  }
   stepper.emergencyStop();
   stepper.moveToPositionInSteps(400);
   */
}
