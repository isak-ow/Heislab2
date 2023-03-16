#include "queue.h"
#include <stdio.h>

int main()
{
    elevio_init();
    reset_elevator();

    State state = IDLE;
    MotorDirection current_dir = DIRN_STOP;
    int queue[4][3];
    int last_valid_floor = -1;
    printf("Før forloop\n");
    for (int floor = 0; floor < 4; floor++)
    {
        for (ButtonType button = BUTTON_HALL_UP; button <= BUTTON_CAB; button++)
        {
            queue[floor][button] = 0;
        }
    }
    clear_queue(queue);
    printf("etter forloop\n");

    while (1)
    {
        printf("startbigwhile\n");
        update_queue(queue);
        printf("insidewhile\n");

        if (valid_floor())
        {
            last_valid_floor = elevio_floorSensor();
            elevio_floorIndicator(last_valid_floor);
        }
        
        if (elevio_stopButton())
        {
            clear_queue(queue);
            current_dir = DIRN_STOP;
            elevio_motorDirection(DIRN_STOP);
            state = STOP;
        }

        switch (state)
        {
        case IDLE:
            printf("indidle\n");

            elevio_motorDirection(DIRN_STOP);

            if (check_queue_empty(queue))
            {
                current_dir = DIRN_STOP;
                printf("queueempty\n");
                state = IDLE;
                break;
            }

            else if (check_current_floor(last_valid_floor, queue))
            {
                printf("gotodest\n");
                state = DEST_REACHED;
                break;
            }

            state = MOVING;
            break;

        case MOVING:
            printf("inmoving\n");
            printf("%d",current_dir);
            if (current_dir == DIRN_STOP)
            {
                if (requests_above(last_valid_floor, queue))
                {
                    printf("dirstopreqabove\n");
                    elevio_motorDirection(DIRN_UP);
                    current_dir = DIRN_UP;
                }

                if (requests_below(last_valid_floor, queue))
                {
                    printf("dirstopreqbelow\n");
                    elevio_motorDirection(DIRN_DOWN);
                    current_dir = DIRN_DOWN;
                }

                state = MOVING;
            }

            else if (current_dir == DIRN_UP)
            {
                if (!requests_above(last_valid_floor,queue))
                {
                    current_dir = DIRN_STOP;
                    elevio_motorDirection(DIRN_STOP);
                    state = IDLE;
                    break;
                }
                elevio_motorDirection(DIRN_UP);
                printf("Dirupmov\n");
                if ((elevio_floorSensor() == 3 && queue[3][BUTTON_HALL_DOWN]) || (elevio_floorSensor() == 3 && current_dir == DIRN_UP))
                {
                    current_dir = DIRN_STOP;
                    state = DEST_REACHED;
                    break;
                }
                if (valid_floor())
                {
                    /* if (queue_hall_up_empty(queue))
                    {
                        if (queue[elevio_floorSensor()][BUTTON_HALL_DOWN] == 1)
                        {
                            state = DEST_REACHED;
                            break;
                        }
                    } */
                    if ((queue[elevio_floorSensor()][BUTTON_HALL_UP] == 1) || (queue[elevio_floorSensor()][BUTTON_CAB] == 1))
                    {
                        printf("dirupdest\n");
                        state = DEST_REACHED;
                        break;
                    }
                }
                state = MOVING;
            }

            else
            {
                if (!requests_below(last_valid_floor,queue))
                {
                    current_dir = DIRN_STOP;
                    elevio_motorDirection(DIRN_STOP);
                    state = IDLE;
                    break;
                }
                elevio_motorDirection(DIRN_DOWN);
                printf("DIRdownmove\n");
                if ((elevio_floorSensor() == 0 && queue[0][BUTTON_HALL_UP]) || (elevio_floorSensor() == 0 && current_dir == DIRN_DOWN))
                {
                    current_dir = DIRN_STOP;
                    state = DEST_REACHED;
                    break;
                }
                if (valid_floor())
                {
                    /* if (queue_hall_down_empty(queue))
                    {
                        if (queue[elevio_floorSensor()][BUTTON_HALL_UP] == 1)
                        {
                            state = DEST_REACHED;
                            break;
                        }
                    } */
                    if ((queue[elevio_floorSensor()][BUTTON_HALL_DOWN] == 1) || (queue[elevio_floorSensor()][BUTTON_CAB] == 1))
                    {
                        state = DEST_REACHED;
                        break;
                    }
                }
                state = MOVING;
            }
            break;


        case DEST_REACHED:

            printf("destreached\n");
            elevio_motorDirection(DIRN_STOP);
            printf("dooropen\n");
            open_door();
            clear_floor(elevio_floorSensor(), queue);

            if (check_queue_empty(queue))
            {
                state = IDLE;
                break;
            }

            state = MOVING;
            break;

        case STOP:
            while (elevio_stopButton())
            {
                if (valid_floor())
                {
                    open_door();
                }
                continue;         
            }
            if (valid_floor())
            {
                state = IDLE;
                break;
            }
            state = MOVING;
            break;
    }
}
    return 0;
}
