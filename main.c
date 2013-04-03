/* Name: main.c
 * Author: Team 3 LG
 * Copyright: All Rights Reserved.
 */

// Include the program we're building
#ifdef MAKE_BASESTATION
#include "controller_basestation.c"
#endif

int main(void)
{
    Controller c = Controller();
	
    for(;;){
        c.loop();
    }

    return 0;   /* never reached */
}
