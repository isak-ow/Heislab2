#pragma once
#include "driver/elevio.h"
#include "queue.h"

typedef enum State
{
    IDLE,
    MOVING,
    DEST_REACHED,
    STOP
} State;

int valid_floor();
void reset_elevator();
void open_door();

