#include <avr/io.h>
#include <util/delay.h>
#include "lgserial.h"
#include "util.h"
#include "lgnetwork.h"

class Controller
{
public:
    Controller ();
    void setup();
    void loop();

private:
    /* data */
    LGNetwork network;
};
