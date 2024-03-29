#include "fsm.h"
#include <unistd.h>
#include "time.h"

int valid_floor()
{
    if (elevio_floorSensor() != -1)
    {
        return 1;
    }
    return 0;
}


void reset_elevator()
{
    for (int floor = 0; floor < 4; floor++)
    {
        for (ButtonType button = BUTTON_HALL_UP; button <= BUTTON_CAB; button++)
        {
            elevio_buttonLamp(floor,button,0);
        }
    }
    
    while (elevio_floorSensor() == -1)
    {
        elevio_motorDirection(DIRN_DOWN);
    }
    elevio_motorDirection(DIRN_STOP);
    elevio_doorOpenLamp(0);
}


void open_door(int queue[4][3])
{
    elevio_doorOpenLamp(1);
    sleep(3);
    while (elevio_obstruction())
    {
        continue;
    }
    elevio_doorOpenLamp(0);
}


/*  void open_door(int queue[4][3])
{
    elevio_doorOpenLamp(1);
    clock_t start_t = clock();
    clock_t end_t = clock() + CLOCKS_PER_SEC*3;
    while (start_t < end_t)
    {
        update_queue(queue);
        while (elevio_obstruction())
        {
            continue;
        }
        start_t = clock();
    }
    elevio_doorOpenLamp(0);
}  */