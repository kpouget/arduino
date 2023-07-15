#include "world.h"

void loop_calibrate(char action) {
  float cpt = atof(&program[current_step + 1]);
  switch (action) {
  case 'l':
    sprintf(buffer, "left: %.1f cm\n", cpt);
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
    log_info(F("l<dist> move the left     tire  of <dist> cm\n"));
    log_info(F("r<dist> move the right    tire  of <dist> cm\n"));
    log_info(F("b<dist> move the both     tires of <dist> cm\n"));
    log_info(F("o<dist> move the opposite tires of <dist> cm\n"));
    log_info(F("z: reset the distance counters\n"));
    log_info(F("x: sleep for 1s\n"));
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
    log_info(F("left one wheel\n"));
    step_move(ONE_WHEEL_90_TURN, 0);
    break;

  case 'R':
    log_info(F("right one wheel\n"));
    step_move(0, ONE_WHEEL_90_TURN);
    break;

  case 'l':
    log_info(F("left two wheels\n"));
    step_move(TWO_WHEELS_90_TURN, -TWO_WHEELS_90_TURN);
    break;

  case 'r':
    log_info(F("right two wheel\n"));
    step_move(-TWO_WHEELS_90_TURN, TWO_WHEELS_90_TURN);
    break;

  case 'f':
    log_info(F("forward\n"));
    step_move(SQUARE_SIZE, SQUARE_SIZE);
    break;

  case 'b':
    log_info(F("backward\n"));
    step_move(-SQUARE_SIZE, -SQUARE_SIZE);
    break;

  case 'z':
    step_reset_counters();
    break;

  case 'H':
    log_info(F("l: 90* left with two wheels\n"));
    log_info(F("L: 90* left with one wheels\n"));
    log_info(F("r: 90* right with one wheel\n"));
    log_info(F("R: 90* right with two wheels\n"));
    log_info(F("\n"));
    log_info(F("f: 1 square forwards\n"));
    log_info(F("b: 1 square backwards\n"));
    log_info(F("\n"));
    log_info(F("z: reset the distance counters\n"));
    log_info(F("x: sleep for 1s\n"));
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
