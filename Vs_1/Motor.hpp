#ifndef __MOTOR__HPP__
#define __MOTOR__HPP__

#include <AccelStepper.h>

class Motor {
  public:
    Motor(int STEP, int DIR);

    float get_speed();
    void set_speed(float speed);
    void run();

    float vel_info;

  private:
    float current_w;
    float target_w;
    AccelStepper step_mt;
};

#endif
