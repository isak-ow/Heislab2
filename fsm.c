#include "fsm.h"
#include "elevio/elevio.h"

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
}