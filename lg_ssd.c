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
	DDRD &= ~(1 << DDD5);		//set PORTD bit 5 for input(?)
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

void update_relay(int status){
	if (status == 0) {	//relay is off
		PORTD |= 1 << PD6;	//turns relay off
	}
	else {	//status == 1
		PORTD &= ~(1 << PD6);	//turns relay on
	}
}//end update_relay


void update_LED(int mode){
	//red is off
	//green is on
	//blue is sync
	if (mode == 0){		//OFF
		PORTB |= 1 << PB0; // Set LEDR to 1
		PORTB &= ~(1 << PB1); //LEDG to 0
		PORTB &= ~(1 << PB2); //LEDB to 0
	}
	else if (mode == 1){	//ON
		PORTB &= ~(1 << PB0);
		PORTB |= 1 << PB1;
		PORTB &= ~(1 << PB2);
	}
	else { //mode = 2	//SYNCING
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

void spin_SSDs(int times){
	int i = 0;

	//first turn all segments "off" by sending 1's to all of them
	reset_SSDs();
	sleep(500);


	for (; i<times; i++){
		//a
		reset_SSDs();
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB &= ~(1 << PB3);//to ssd0
		PORTB &= ~(1 << PB4);//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		sleep(100);

		//b
		reset_SSDs();
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB &= ~(1 << PB3);//to ssd0
		PORTB &= ~(1 << PB4);//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		sleep(100);

		//c
		reset_SSDs();
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB &= ~(1 << PB3);//to ssd0
		PORTB &= ~(1 << PB4);//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		sleep(100);

		//d
		reset_SSDs();
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB &= ~(1 << PB3);//to ssd0
		PORTB &= ~(1 << PB4);//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		sleep(100);

		//e
		reset_SSDs();
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB &= ~(1 << PB3);//to ssd0
		PORTB &= ~(1 << PB4);//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		sleep(100);

		//f
		reset_SSDs();
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB &= ~(1 << PB3);//to ssd0
		PORTB &= ~(1 << PB4);//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;

		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTB |= 1 << PB3;//to ssd0
		PORTB |= 1 << PB4;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		sleep(100);
	}//endfor


}



void update_ssd0(int ssd_val){
	if (ssd_val == 0){
		Display0_SSD0();
	}
	else if (ssd_val == 1){
		Display1_SSD0();
	}
	else if (ssd_val == 2){
		Display2_SSD0();
	}
	else if (ssd_val == 3){
		Display3_SSD0();
	}
	else if (ssd_val == 4){
		Display4_SSD0();
	}
	else if (ssd_val == 5){
		Display5_SSD0();
	}
	else if (ssd_val == 6){
		Display6_SSD0();
	}
	else if (ssd_val == 7){
		Display7_SSD0();
	}
	else if (ssd_val == 8){
		Display8_SSD0();
	}
	else {  //ssd_val == 9
		Display9_SSD0();
	}
}


void update_ssd1(int ssd_val){
	if (ssd_val == 0){
		Display0_SSD1();
	}
	else if (ssd_val == 1){
		Display1_SSD1();
	}
	else if (ssd_val == 2){
		Display2_SSD1();
	}
	else if (ssd_val == 3){
		Display3_SSD1();
	}
	else if (ssd_val == 4){
		Display4_SSD1();
	}
	else if (ssd_val == 5){
		Display5_SSD1();
	}
	else if (ssd_val == 6){
		Display6_SSD1();
	}
	else if (ssd_val == 7){
		Display7_SSD1();
	}
	else if (ssd_val == 8){
		Display8_SSD1();
	}
	else {  //ssd_val == 9
		Display9_SSD1();
	}

}

//////function to write 0 to ssd0:
void Display0_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);//set to zero
  	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;//becomes 1, creates positive clock edge, passing 0s above into shift registers
	//f
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);;
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
}

//////function to write 0 to ssd1:
void Display0_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
  	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
}

//////function to write 1 to ssd0:
void Display1_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
  	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
}

//////function to write 1 to ssd1:
void Display1_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
}


//////function to write 2 to ssd0:
void Display2_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
  	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
  	PORTB &= ~(1 << PB3);;
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
    PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
  	PORTB &= ~(1 << PB3);;
	PORTD |= 1 << PD3;
	//a
 	PORTD &= ~(1 << PD3);
  	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
}

//////function to write 2 to ssd1:
void Display2_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
}



//////function to write 3 to ssd0:
void Display3_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
}

//////function to write 3 to ssd1:
void Display3_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
    PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
    PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
}


//////function to write 4 to ssd0:
void Display4_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
}

//////function to write 4 to ssd1:
void Display4_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
}



//////function to write 5 to ssd0:
void Display5_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
  	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
}

//////function to write 5 to ssd1:
void Display5_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
}



//////function to write 6 to ssd0:
void Display6_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
}

//////function to write 6 to ssd1:
void Display6_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
}



//////function to write 7 to ssd0:
void Display7_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
    PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
}

//////function to write 7 to ssd1:
void Display7_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
  	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
}



//////function to write 8 to ssd0:
void Display8_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
}

//////function to write 8 to ssd1:
void Display8_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
}



//////function to write 9 to ssd0:
void Display9_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	PORTB |= 1 << PB3;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
    PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
	PORTB &= ~(1 << PB3);
	PORTD |= 1 << PD3;
}

//////function to write 9 to ssd1:
void Display9_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	PORTB |= 1 << PB4;
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
    PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	PORTB &= ~(1 << PB4);
	PORTD |= 1 << PD4;
}
