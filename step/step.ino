#include <AccelStepper.h>
#define HALFSTEP 8

// Motor pin definitions
#define motor1Pin1  8     // IN1 on the ULN2003 driver 1
#define motor2Pin1  4     // IN1 on the ULN2003 driver 1


// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP, motor1Pin1, motor1Pin1 + 2, motor1Pin1 + 1, motor1Pin1 + 3);
AccelStepper stepper2(HALFSTEP, motor2Pin1, motor2Pin1 + 2, motor2Pin1 + 1, motor2Pin1 + 3);


// variables
int turnSteps = 3230; // number of steps for a 90 degree turn
/*
 * 3235: tourne un peu trop
 * 3240: tourne un peu trop
 */
int lineSteps = 6600 * 1.5; //number of steps to drive straight
int stepperSpeed = 1000; //speed of the stepper (steps per second)
int steps1 = 0; // keep track of the step count for motor 1
int steps2 = 0; // keep track of the step count for motor 2

boolean turn1 = false; //keep track if we are turning or going straight next
boolean turn2 = false; //keep track if we are turning or going straight next


void setup() {
  delay(3000); //sime time to put the robot down after swithing it on

  stepper1.setMaxSpeed(2000.0);
  stepper1.move(1);  // I found this necessary
  stepper1.setSpeed(stepperSpeed);

  stepper2.setMaxSpeed(2000.0);
  stepper2.move(-1);  // I found this necessary
  stepper2.setSpeed(stepperSpeed);
}


void loop() {

  if (steps1 == 0) {
    int target = 0;
    if (turn1 == true) {
      target = turnSteps;
    }

    else {
      target = lineSteps;
    }

    stepper1.move(target);
    stepper1.setSpeed(stepperSpeed);
    turn1 = !turn1;
  }

  if (steps2 == 0) {
    int target = 0;
    if (turn2 == true) {
      target = turnSteps;
    }

    else {
      target = -lineSteps;
    }

    stepper2.move(target);
    stepper2.setSpeed(stepperSpeed);
    turn2 = !turn2;
  }

  steps1 = stepper1.distanceToGo();
  steps2 = stepper2.distanceToGo();

  stepper1.runSpeedToPosition();
  stepper2.runSpeedToPosition();
}
