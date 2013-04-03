/* Name: main.c
 * Author: Team 3 LG
 * Copyright: All Rights Reserved.
 */

// Include the program we're building.
#include CONTROLLER_FILE

int main(void)
{
    Controller c = Controller();
    c.setup();
	
    for(;;){
        c.loop();
    }

    return 0;   /* never reached */
}
