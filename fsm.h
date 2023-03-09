#pragma once
#include "elevio/elevio.h"

typedef enum State
{
    IDLE,
    MOVING,
    DEST_REACHED,
    STOP
} State;

int valid_floor();
void reset_elevator();


