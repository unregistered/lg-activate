#include "lg_ssd.h"
 
void update_relay(int on_off_b){
	if (on_off_b == 0) {	//relay is off
		//set output bits to turn relay off
	}
	else {	//on_off_b == 1
		//set output bits to turn relay on
	}	
}//end update_relay

void update_LED(int mode){
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
	else { //mode = 2	//AUTO
		PORTB &= ~(1 << PB0); 
		PORTB &= ~(1 << PB1);
		PORTB |= 1 << PB2;
	}  
	
}//end update_LED

void reset_SSDs(){
	PORTD &= ~(1 << PD3);
	PORTD &= ~(1 << PD4);
    PORTD |= 1 << PD0;
	PORTD |= 1 << PD1;
	PORTD |= 1 << PD3;
	PORTD |= 1 << PD4;
	PORTD &= ~(1 << PD3);
	PORTD &= ~(1 << PD4);
    PORTD |= 1 << PD0;
	PORTD |= 1 << PD1;
	PORTD |= 1 << PD3;
	PORTD |= 1 << PD4;
	PORTD &= ~(1 << PD3);
	PORTD &= ~(1 << PD4);
    PORTD |= 1 << PD0;
	PORTD |= 1 << PD1;
	PORTD |= 1 << PD3;
	PORTD |= 1 << PD4;
	PORTD &= ~(1 << PD3);
	PORTD &= ~(1 << PD4);
    PORTD |= 1 << PD0;
	PORTD |= 1 << PD1;
	PORTD |= 1 << PD3;
	PORTD |= 1 << PD4;
	PORTD &= ~(1 << PD3);
	PORTD &= ~(1 << PD4);
    PORTD |= 1 << PD0;
	PORTD |= 1 << PD1;
	PORTD |= 1 << PD3;
	PORTD |= 1 << PD4;
	PORTD &= ~(1 << PD3);
	PORTD &= ~(1 << PD4);
    PORTD |= 1 << PD0;
	PORTD |= 1 << PD1;
	PORTD |= 1 << PD3;
	PORTD |= 1 << PD4;
	PORTD &= ~(1 << PD3);
	PORTD &= ~(1 << PD4);
    PORTD |= 1 << PD0;
	PORTD |= 1 << PD1;
	PORTD |= 1 << PD3;	
	PORTD |= 1 << PD4;
}

void spin_SSDs(){
	int i = 0;
	//	PORTD &= ~(1 << PD2);//reset
	//	PORTD |= 1 << PD2;	//set
	update_LED(2);	//finished resetting on blue
	
	//first turn all segments "off" by sending 1's to all of them
	reset_SSDs();
	_delay_ms(500);
	
	update_LED(1);	//led is green, send in an a
	
	for (; i<20; i++){
		//a
		reset_SSDs();
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD &= ~(1 << PD0);//to ssd0
		PORTD &= ~(1 << PD1);//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		_delay_ms(100);
		
		update_LED(0);//led is red when a is "on"
		
		//b
		reset_SSDs();
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD &= ~(1 << PD0);//to ssd0
		PORTD &= ~(1 << PD1);//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		_delay_ms(100);
		
		//c
		reset_SSDs();
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD &= ~(1 << PD0);//to ssd0
		PORTD &= ~(1 << PD1);//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		_delay_ms(100);
		
		//d
		reset_SSDs();
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD &= ~(1 << PD0);//to ssd0
		PORTD &= ~(1 << PD1);//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		_delay_ms(100);
		
		//e
		reset_SSDs();
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD &= ~(1 << PD0);//to ssd0
		PORTD &= ~(1 << PD1);//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		_delay_ms(100);
		
		//f
		reset_SSDs();
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD &= ~(1 << PD0);//to ssd0
		PORTD &= ~(1 << PD1);//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		
		PORTD &= ~(1 << PD3);
		PORTD &= ~(1 << PD4);
		PORTD |= 1 << PD0;//to ssd0
		PORTD |= 1 << PD1;//to ssd1
		PORTD |= 1 << PD3;
		PORTD |= 1 << PD4;
		_delay_ms(100);
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
  	PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;//becomes 1, creates positive clock edge, passing 0s above into shift registers
	//f
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);;
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
}

//////function to write 0 to ssd1:
void Display0_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
  	PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
}

//////function to write 1 to ssd0:
void Display1_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
  	PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
}

//////function to write 1 to ssd1:
void Display1_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
}


//////function to write 2 to ssd0:
void Display2_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
  	PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
  	PORTD &= ~(1 << PD0);;
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
    PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
  	PORTD &= ~(1 << PD0);;
	PORTD |= 1 << PD3;
	//a
 	PORTD &= ~(1 << PD3);
  	PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
}

//////function to write 2 to ssd1:
void Display2_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
}



//////function to write 3 to ssd0:
void Display3_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
}

//////function to write 3 to ssd1:
void Display3_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
}


//////function to write 4 to ssd0:
void Display4_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
}

//////function to write 4 to ssd1:
void Display4_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
}



//////function to write 5 to ssd0:
void Display5_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
  	PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
}

//////function to write 5 to ssd1:
void Display5_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
}



//////function to write 6 to ssd0:
void Display6_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
  PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
}

//////function to write 6 to ssd1:
void Display6_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
}



//////function to write 7 to ssd0:
void Display7_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
    PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
}

//////function to write 7 to ssd1:
void Display7_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
  	PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
}



//////function to write 8 to ssd0:
void Display8_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
}

//////function to write 8 to ssd1:
void Display8_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
}



//////function to write 9 to ssd0:
void Display9_SSD0(){
	//CLK0 is PD3...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//f
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//e
	PORTD &= ~(1 << PD3);
	  PORTD |= 1 << PD0;
	PORTD |= 1 << PD3;
	//d
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//c
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//b
	PORTD &= ~(1 << PD3);
    PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
	//a
	PORTD &= ~(1 << PD3);
	  PORTD &= ~(1 << PD0);
	PORTD |= 1 << PD3;
}

//////function to write 9 to ssd1:
void Display9_SSD1(){
	//CLK1 is PD4...
	//0 turns the segment on
	//g
	PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//f
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//e
	PORTD &= ~(1 << PD4);
	  PORTD |= 1 << PD1;
	PORTD |= 1 << PD4;
	//d
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//c
	PORTD &= ~(1 << PD4);
    PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//b
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
	//a
	PORTD &= ~(1 << PD4);
	  PORTD &= ~(1 << PD1);
	PORTD |= 1 << PD4;
}