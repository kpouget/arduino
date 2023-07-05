#include "world.h"

void loop_calibrate(char action) {
  float cpt = atof(&program[current_step + 1]);
  switch (action) {
  case 'l':
    sprintf(buffer, "left: %d cm\n", cpt);
    log_info(buffer);
    step_move(cpt, 0);
    break;
  case 'r':
    sprintf(buffer, "right: %.1f cm\n", cpt);
    log_info(buffer);
    step_move(0, cpt);
    break;
  case 'b':
    sprintf(buffer, "both: %.1f cm\n", cpt);
    log_info(buffer);
    step_move(cpt, cpt);
    break;
  case 'o':
    cpt = atof(&program[current_step + 1]);
    sprintf(buffer, "opposite: %.1f steps\n", cpt);
    log_info(buffer);
    step_move(-cpt, cpt);
    break;
  case 'z':
    step_reset_counters();
    break;
  case 'H':
    log_info("l<dist> move the left     tire  of <dist> cm\n");
    log_info("r<dist> move the right    tire  of <dist> cm\n");
    log_info("b<dist> move the both     tires of <dist> cm\n");
    log_info("o<dist> move the opposite tires of <dist> cm\n");
    log_info("z: reset the distance counters\n");
    log_info("x: sleep for 1s\n");
    break;
  case '0': case '1':  case '2': case '3':
  case '4': case '5':  case '6':  case '7':
  case '8': case '9':
    // ignore
    break;
  case 'x':
  default:
    delay(1000);
  }
}

void loop_program(char action) {
  switch (action) {
  case 'L':
    log_info("left one wheel\n");
    step_move(ONE_WHEEL_90_TURN, 0);
    break;

  case 'R':
    log_info("right one wheel\n");
    step_move(0, ONE_WHEEL_90_TURN);
    break;

  case 'l':
    log_info("left two wheels\n");
    step_move(TWO_WHEELS_90_TURN, -TWO_WHEELS_90_TURN);
    break;

  case 'r':
    log_info("right two wheel\n");
    step_move(-TWO_WHEELS_90_TURN, TWO_WHEELS_90_TURN);
    break;

  case 'f':
    log_info("forward\n");
    step_move(SQUARE_SIZE, SQUARE_SIZE);
    break;

  case 'b':
    log_info("backward\n");
    step_move(-SQUARE_SIZE, -SQUARE_SIZE);
    break;

  case 'z':
    step_reset_counters();
    break;

  case 'H':
    log_info("l: 90* left with two wheels\n");
    log_info("L: 90* left with one wheels\n");
    log_info("r: 90* right with one wheel\n");
    log_info("R: 90* right with two wheels\n");
    log_info("z: reset the distance counters\n");
    log_info("x: sleep for 1s\n");
    break;
  case '0': case '1':  case '2': case '3':
  case '4': case '5':  case '6':  case '7':
  case '8': case '9':
    // ignore
    break;
  case 'x':
  default:
    delay(1000);
  }
}

void loop_distance(char action) {
  switch (action) {
  default:
    log_info("Nothing to do at the moment in distance mode\n");
    delay(1000);
  }
}
