#include <AccelStepper.h>
#include "world.h"

#define HALFSTEP_MODE 8

// Motor pin definitions
#define motor1Pin1  8     // IN1 on the ULN2003 driver 1
#define motor2Pin1  4     // IN1 on the ULN2003 driver 1

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper with 28BYJ-48
AccelStepper stepper1(HALFSTEP_MODE, motor1Pin1, motor1Pin1 + 2, motor1Pin1 + 1, motor1Pin1 + 3);
AccelStepper stepper2(HALFSTEP_MODE, motor2Pin1, motor2Pin1 + 2, motor2Pin1 + 1, motor2Pin1 + 3);

#define STEPPER_MAX_SPEED 2000.0

#define STEP_COUNT 4076.0

//int turnSteps = 3230; // number of steps for a 90 degree turn
/*
 * 3235: tourne un peu trop
 * 3240: tourne un peu trop
 */
//int lineSteps = 6600 * 1.5; //number of steps to drive straight
int stepperSpeed = 1000; //speed of the stepper (steps per second)

void step_setup() {
  stepper1.setMaxSpeed(STEPPER_MAX_SPEED);
  stepper1.move(1);  // I found this necessary
  stepper1.setSpeed(stepperSpeed);

  stepper2.setMaxSpeed(STEPPER_MAX_SPEED);
  stepper2.move(-1);  // I found this necessary
  stepper2.setSpeed(stepperSpeed);
}

int step_do() {
  stepper1.runSpeedToPosition();
  stepper2.runSpeedToPosition();
}

int step_is_done() {
  int debug = 0;
  if (debug) {
    Serial.print("distance ");
    Serial.print(stepper1.distanceToGo());
    Serial.print(" | ");
    Serial.print(stepper2.distanceToGo());
    Serial.print("\n");
  }

  return (stepper1.distanceToGo() == 0 &&
          stepper2.distanceToGo() == 0);
}

static float counter_l = 0.0;
static float counter_r = 0.0;

void step_move(float dist_l, float dist_r) {
  int target_l = CM_TO_STEP(dist_l);
  int target_r = CM_TO_STEP(dist_r);

  counter_l += dist_l;
  counter_r += dist_r;

  sprintf(buffer, "L+%d = %d | R+%d = %d\n", (int) dist_l, (int) counter_l, (int) dist_r, (int) counter_r);
  log_info(buffer);

  step_move_steps(target_l, target_r);
}

void step_move_steps(int target_l, int target_r) {
  stepper1.move(target_l);
  stepper2.move(-target_r);

  stepper1.setSpeed(stepperSpeed);
  stepper2.setSpeed(stepperSpeed);

  stepper1.runSpeedToPosition();
  stepper2.runSpeedToPosition();
}

void step_reset_counters(void) {
  counter_l = 0.0;
  counter_r = 0.0;
  log_info("Counters reseted\n");
}

void step_forward() {
  step_move(SQUARE_SIZE, SQUARE_SIZE);
  log_info("Forward\n");
}

void step_backward() {
  step_move(-SQUARE_SIZE, -SQUARE_SIZE);
  log_info("Backard\n");
}

void step_left() {
  step_move(-TWO_WHEELS_90_TURN, TWO_WHEELS_90_TURN);
  log_info("Two wheels left\n");
}

void step_right() {
  step_move(TWO_WHEELS_90_TURN, -TWO_WHEELS_90_TURN);
  log_info("Two wheels right\n");
}

void step_one_wheel_left() {
  step_move(0, ONE_WHEEL_90_TURN);
  log_info("One wheel left\n");
}

void step_one_wheel_right() {
  step_move(ONE_WHEEL_90_TURN, 0);
  log_info("One wheel right\n");
}
