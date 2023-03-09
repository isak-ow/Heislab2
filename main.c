#pragma once
#include "fsm/fsm.h"
#include "queue/queue.h"

int main()
{
    elevio_init();
    reset_elevator();

    State state = IDLE;
    MotorDirection current_dir = DIRN_STOP;
    int last_valid_floor = elevio_floorSensor();
    int queue[4][3];

    for (int floor = 0; floor < 4; floor++)
    {
        for (ButtonType button = BUTTON_HALL_UP; button <= BUTTON_CAB; button++)
        {
            queue[floor][button] = 0;
        }
    }

    while (1)
    {
        update_queue(&queue);

        if (valid_floor())
        {
            last_valid_floor = elevio_floorSensor();
        }

        if (elevio_stopButton())
        {
            clear_queue(&queue);
            state = STOP;
        }

        switch (state)
        {
        case IDLE:
            elevio_motorDirection(DIRN_STOP);
            if (check_current_floor(last_valid_floor, &queue))
            {
                state = DEST_REACHED;
                break;
            }

            else if (queue_empty())
            {
                current_dir = DIRN_STOP;
                state = IDLE;
                break;
            }

            state = MOVING;
            break;

        case MOVING:
            switch (current_dir)
            {
            case DIRN_STOP:
                if (requests_above(last_valid_floor, &queue))
                {
                    elevio_motorDirection(DIRN_UP);
                    current_dir = DIRN_UP;
                }

                if (requests_below(last_valid_floor, &queue))
                {
                    elevio_motorDirection(DIRN_DOWN);
                    current_dir = DIRN_DOWN;
                }

                state = MOVING;
                break;

            case DIRN_UP:
                if (valid_floor())
                {
                    last_valid_floor = elevio_floorSensor();
                    if (queue[last_valid_floor][BUTTON_HALL_UP] || queue[last_valid_floor][BUTTON_CAB])
                    {
                        state = DEST_REACHED;
                        break;
                    }
                }
                state = MOVING;
                break;

            case DIRN_DOWN:
                last_valid_floor = elevio_floorSensor();
                if (valid_floor())
                {
                    if (queue[last_valid_floor][BUTTON_HALL_DOWN] || queue[last_valid_floor][BUTTON_CAB])
                    {
                        state = DEST_REACHED;
                        break;
                    }
                }
                state = MOVING;
                break;
            }

        case DEST_REACHED:

            elevio_motorDirection(DIRN_STOP);

            if (current_dir == DIRN_UP)
            {
                elevio_floorIndicator(elevio_floorSensor() - 1); // maybe use last_valid_floor
            }
            else if (current_dir == DIRN_DOWN)
            {
                elevio_floorIndicator(elevio_floorSensor() + 1);
            }

            elevio_floorIndicator(elevio_floorSensor());
            open_door();
            clear_floor(elevio_floorSensor(), &queue);

            if (check_queue_empty(&queue))
            {
                state = IDLE;
                break;
            }

            state = MOVING;
            break;

        case STOP:
            elevio_motorDirection(DIRN_STOP);
            current_dir = DIRN_STOP;
            int valid = valid_floor();

            if (elevio_stopButton())
            {
                break;
            }

            switch (valid)
            {
            case 0:
                if (!queue_empty())
                {
                    state = MOVING;
                }
                break;

            case 1:
                elevio_doorOpenLamp(1);
                // sleep for three seconds
                elevio_doorOpenLamp(0);
                state = IDLE;
                break;
            }
        }
    }
    return 0;
}