#ifndef ACCEL_H
#define ACCEL_H

#include <Adafruit_LSM6DSO32.h>

extern bool trigger;

namespace ACCEL
{
    void setup();
    void update();
}


#endif