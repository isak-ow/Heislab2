#include "queue.h"

int requests_above(int currentfloor, int queue[4][3])
{
    for (int floor = currentfloor; floor < 4; floor++)
    {
        for (ButtonType button = BUTTON_HALL_UP; button <= BUTTON_CAB; button++)
        {
            if (queue[floor][button])
            {
                return 1;
            }
        }
    }
    return 0;
}

int requests_below(int currentfloor, int queue[4][3])
{
    for (int floor = 0; floor <= currentfloor; floor++)
    {
        for (ButtonType button = BUTTON_HALL_UP; button <= BUTTON_CAB; button++)
        {
            if (queue[floor][button])
            {
                return 1;
            }
        }
    }
    return 0;
}

void clear_queue(int queue[4][3])
{
    for (int floor = 0; floor < 4; floor++)
    {
        for (ButtonType button = BUTTON_HALL_UP; button <= BUTTON_CAB; button++)
        {
            queue[floor][button] = 0;
            elevio_buttonLamp(floor, button, 0);
        }
    }
}

void update_queue(int queue[4][3])
{
    for (int floor = 0; floor < 4; floor++)
    {
        for (ButtonType button = BUTTON_HALL_UP; button <= BUTTON_CAB; button++)
        {
            if (elevio_callButton(floor, button))
            {
                queue[floor][button] = 1;
                elevio_buttonLamp(floor, button, 1);
            }
        }
    }
}

int check_queue_empty(int queue[4][3])
{
    for (int floor = 0; floor < 4; floor++)
    {
        for (ButtonType button = BUTTON_HALL_UP; button <= BUTTON_CAB; button++)
        {
            if (queue[floor][button])
            {
                return 0;
            }
        }
    }
    return 1;
}

void clear_floor(int floor,int queue[4][3])
{
    for (ButtonType button = BUTTON_HALL_UP; button <= BUTTON_CAB; button++)
    {
        queue[floor][button] = 0;
        elevio_buttonLamp(floor,button,0);
    }
}

int check_current_floor(int floor, int queue[4][3])
{
    if (queue[floor][BUTTON_HALL_UP] == 1)
    {
        return 1;
    }

    else if(queue[floor][BUTTON_HALL_DOWN] == 1)
    {
        return 1;
    }

    else if(queue[floor][BUTTON_CAB] == 1)
    {
        return 1;
    }

    else{
        return 0;
    }
}