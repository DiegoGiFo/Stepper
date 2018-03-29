#include "Motor.hpp"
#include <AccelStepper.h>

const float LAMBDA = 0.999f;

int get_direction(float x)
{
    if(x > 0) return 1;
    else return -1;
}

Motor::Motor(int STEP, int DIR){
    this->target_w = 0.0;
    this->current_w = 0.0;
    this->step_mt(AccelStepper::DRIVER, STEP, DIR);
    this->step_mt.setMaxSpeed(10000);
}

float Motor::get_speed() {
  return this->current_w;
}

void Motor::set_speed(float speed) {
  this->target_w = speed;
}

void Motor::run() {
  this->current_w = LAMBDA*this->current_w + (1-LAMBDA)*this->target_w;
  const float w_abs = fabs(this->current_w)*(100.0/M_PI);
  const float direction = get_direction(this->current_w);
  this->step_mt.setSpeed(direction*w_abs);
}
