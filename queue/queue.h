#pragma once
#include "fsm/fsm.h"

int requests_above(int currentfloor, int* queue[4][3]);
int requests_below(int currenfloor, int* queue[4][3]);
void clear_queue(int* queue[4][3]); //also sets lights
void update_queue(int* queue[4][3]); //also sets lights
int check_queue_empty(int* queue[4][3]);
void clear_floor(int floor, int* queue[4][3]); //turns off buttonlights as well
int check_current_floor(int floor, int* queue[4][3]);

