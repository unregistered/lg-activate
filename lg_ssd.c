#include "lg_ssd.h"
#include "util.h"

void init_SSDs() {
    DDRB |= 1 << DDB0;
    DDRB |= 1 << DDB1;
    DDRB |= 1 << DDB2;
    DDRB |= 1 << DDB3;
    DDRB |= 1 << DDB4;
    DDRD |= 1 << DDD2;
    DDRD |= 1 << DDD3;
    DDRD |= 1 << DDD4;
    DDRD &= ~(1 << DDD5);       //set PORTD bit 5 for input(?)
    DDRD |= 1 << DDD6;//for testing reading the  button

    //set io pins to 0 or 1
    PORTB &= ~(1 << PB0); // Set LEDR to 0
    PORTB &= ~(1 << PB1); //LEDG to 0
    PORTB &= ~(1 << PB2); //LEDB to 0

    PORTB &= ~(1 << PB3);//shift reg input 0
    PORTB &= ~(1 << PB4);//shift reg input 1
    PORTD |= 1 << PD2;  //PD2 clears shift registers, want set to 1
    PORTD &= ~(1 << PD3);//clk to 0
    PORTD &= ~(1 << PD4);//clk to 0
    PORTD &= ~(1 << PD6);
}

void SSD0_shiftin(bool data) {
    PORTD &= ~(1 << PD3); //set clock to zero
    if(data)
        PORTB |= 1 << PB3;
    else
        PORTB &= ~(1 << PB3);
    PORTD |= 1 << PD3; //becomes 1, creates positive clock edge, passing 0s above into shift registers
}

void SSD1_shiftin(bool data) {
    PORTD &= ~(1 << PD4);
    if(data)
        PORTB |= 1 << PB4;
    else
        PORTB &= ~(1 << PB4);
    PORTD |= 1 << PD4;
}

void display_vector(uint8_t vtor, void (*fp)(bool data)) {
    uint8_t mask = 0x40;
    for(int i=0; i < 7; i++) {
        uint8_t result = vtor & mask;
        (*fp)(result);
        mask = mask >> 1;
    }
}

void update_relay(uint8_t status){
    if (status == SYSTEM_OFF) {  //relay is off
        PORTD |= 1 << PD6;  //turns relay off
    }
    else {  //status == 1
        PORTD &= ~(1 << PD6);   //turns relay on
    }
}//end update_relay


void update_LED(uint8_t mode){
    //red is off
    //green is on
    //blue is sync
    if (mode == SYSTEM_OFF){     //OFF
        PORTB |= 1 << PB0; // Set LEDR to 1
        PORTB &= ~(1 << PB1); //LEDG to 0
        PORTB &= ~(1 << PB2); //LEDB to 0
    }
    else if (mode == SYSTEM_ON){    //ON
        PORTB &= ~(1 << PB0);
        PORTB |= 1 << PB1;
        PORTB &= ~(1 << PB2);
    }
    else { //mode = 2   //SYNCING
        PORTB &= ~(1 << PB0);
        PORTB &= ~(1 << PB1);
        PORTB |= 1 << PB2;
    }

}//end update_LED


void reset_SSDs(){
    PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD4);
    PORTB |= 1 << PB3;
    PORTB |= 1 << PB4;
    PORTD |= 1 << PD3;
    PORTD |= 1 << PD4;
    PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD4);
    PORTB |= 1 << PB3;
    PORTB |= 1 << PB4;
    PORTD |= 1 << PD3;
    PORTD |= 1 << PD4;
    PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD4);
    PORTB |= 1 << PB3;
    PORTB |= 1 << PB4;
    PORTD |= 1 << PD3;
    PORTD |= 1 << PD4;
    PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD4);
    PORTB |= 1 << PB3;
    PORTB |= 1 << PB4;
    PORTD |= 1 << PD3;
    PORTD |= 1 << PD4;
    PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD4);
    PORTB |= 1 << PB3;
    PORTB |= 1 << PB4;
    PORTD |= 1 << PD3;
    PORTD |= 1 << PD4;
    PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD4);
    PORTB |= 1 << PB3;
    PORTB |= 1 << PB4;
    PORTD |= 1 << PD3;
    PORTD |= 1 << PD4;
    PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD4);
    PORTB |= 1 << PB3;
    PORTB |= 1 << PB4;
    PORTD |= 1 << PD3;
    PORTD |= 1 << PD4;
}

void spin_SSDs(){
    unsigned long t = millis()/100;
    uint8_t current_segment = t % 6;

    reset_SSDs();

    SSD0_shiftin(0);
    for(int i=0; i < current_segment; i++) {
    	SSD0_shiftin(1);
    }

    SSD1_shiftin(0);
    for(int i=0; i < current_segment; i++) {
    	SSD1_shiftin(1);
    }




}



void update_ssd0(uint8_t ssd_val){
    uint8_t vtor;

    switch(ssd_val) {
        case 0:
            vtor = 0x40;
            break;

        case 1:
            vtor = 0x79;
            break;

        case 2:
            vtor = 0x24;
            break;

        case 3:
            vtor = 0x30;
            break;

        case 4:
            vtor = 0x19;
            break;

        case 5:
            vtor = 0x12;
            break;

        case 6:
            vtor = 0x02;
            break;

        case 7:
            vtor = 0x78;
            break;

        case 8:
            vtor = 0x00;
            break;

        case 9:
            vtor = 0x10;
            break;

        default:
            break;
    }

    display_vector(vtor, SSD0_shiftin);
}


void update_ssd1(uint8_t ssd_val){
    uint8_t vtor;

    switch(ssd_val) {
        case 0:
            vtor = 0x40;
            break;

        case 1:
            vtor = 0x79;
            break;

        case 2:
            vtor = 0x24;
            break;

        case 3:
            vtor = 0x30;
            break;

        case 4:
            vtor = 0x19;
            break;

        case 5:
            vtor = 0x12;
            break;

        case 6:
            vtor = 0x02;
            break;

        case 7:
            vtor = 0x78;
            break;

        case 8:
            vtor = 0x00;
            break;

        case 9:
            vtor = 0x10;
            break;

        default:
            break;
    }

    display_vector(vtor, SSD1_shiftin);
}
