#include "can.h"

#ifndef SOLENOID_H
#define SOLENOID_H

void solenoid_init(void);
void solenoid_fire(void);
void solenoid_reset(void);
void solenoid_control(CanMsg* rx_message);

#endif