/* Name: main.c
 * Author: Team 3 LG
 * Copyright: All Rights Reserved.
 */

// Include the program we're building.
#include CONTROLLER_FILE
#include "util.h"

int main(void)
{
    init_timer();

    Controller c = Controller();
    c.setup();

    for(;;){
        c.loop();
    }

    return 0;   /* never reached */
}

