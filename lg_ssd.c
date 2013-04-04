#include "lg_ssd.h"
 

 
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