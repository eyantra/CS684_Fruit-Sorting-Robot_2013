
/**
This is main file of fruit sorting robot FB V part of software. 

This file is genarated as a part of course project of CS 684 course by Team 10 during Autumn 2013 
Project Title: Fruit Sorting Bot 

We have taken reference form the sample codes given by NEX Robotics Pvt. Ltd. & also we are reusing some of the code files provided by them. 
**/

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#include "values.h"
#include "lcd1.c"
#include "Servo1.c"
#include "dc_motor1.c"
#include "ir_proximity.c"
unsigned char data; //to store received data from UDR1
unsigned char wait_for_class_label=0;
unsigned char extract_fruit=1;


unsigned int ir_dispence=255;
unsigned int ir_ip=255;

unsigned int fruit_sensed=1;
unsigned int ip_hungry=1;
unsigned int empty=1;
unsigned int init_signal=0;
unsigned int start_signal=0;


void buzzer_pin_config (void)
{
 DDRC = DDRC | 0x08;		//Setting PORTC 3 as outpt
 PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer
}

/*void motion_pin_config (void)
{
 DDRA = DDRA | 0x0F;
 PORTA = PORTA & 0xF0;
 DDRL = DDRL | 0x18;   //Setting PL3 and PL4 pins as output for PWM generation
 PORTL = PORTL | 0x18; //PL3 and PL4 pins are for velocity control using PWM.
}*/

//Function to initialize ports
void port_init()
{
	//motion_pin_config();
	buzzer_pin_config();
}

void buzzer_on (void)
{
 unsigned char port_restore = 0;
 port_restore = PINC;
 port_restore = port_restore | 0x08;
 PORTC = port_restore;
}

void buzzer_off (void)
{
 unsigned char port_restore = 0;
 port_restore = PINC;
 port_restore = port_restore & 0xF7;
 PORTC = port_restore;
}

//Function To Initialize UART2
// desired baud rate:9600
// actual baud rate:9600 (error 0.0%)
// char size: 8 bit
// parity: Disabled
void uart2_init(void)
{

 UCSR2B = 0x00; //disable while setting baud rate
 UCSR2A = 0x00;
 //UCSR2A = 0x20;
 UCSR2C = 0x06;
 UBRR2L = 0x5F; //set baud rate lo
 UBRR2H = 0x00; //set baud rate hi
 UCSR2B = 0x98;
 //UCSR2B = 0xF8;
}

void distribute_to(int fruit_class){
	 route(fruit_class); 
	 let_fruit_go();
}


SIGNAL(SIG_USART2_RECV) 		// ISR for receive complete interrupt
{
	data = UDR2; 				//making copy of data from UDR2 (through serial communication) in 'data' variable 
	
	switch(data)
	{

		case CLASS_1 :if(wait_for_class_label){
							distribute_to(1);
							wait_for_class_label=0;
							break;
					  }

		case CLASS_2 :if(wait_for_class_label){
							distribute_to(2);
							wait_for_class_label=0;
							break;
					  }

		case CLASS_3 :if(wait_for_class_label){
							distribute_to(3);
							wait_for_class_label=0;
							break;
					  }
		
		case INIT_SIG : init_devices();
						init_signal=1; //set init_signal so that the code in main comes out of bysy waiting for init
						//------- 
						cli();
						UDR2 = data;   //echo data back to PC indicating initialisation complete & i am ready
						sei();
						//---------------
						lcd1_cursor(1,3);
						lcd1_string("Welcome :)                ");
						lcd1_cursor(2,3);
						lcd1_string("I am ready, press start   ");
						break;

		case START_SIG : start_signal=1; //set start_signal so that the code in main comes out of bysy waiting for start
						 break;

		case PAUSE_SIG : start_signal=0; 
						 break;	    

		case STOP_SIG : init_signal=0;start_signal=0; 
						 break;	    

		
		default: lcd1_cursor(1,3);
				 lcd1_string("Got some data from PC");
				 lcd1_cursor(2,3);
				 lcd1_string("Data: ");
				 lcd_print(2,10,data,4);
				 ;break;

	}

}




//Function To Initialize all The Devices
void init_devices()
{
 cli(); //Clears the global interrupts
 port_init();  //Initializes all the ports
 uart2_init(); //Initailize UART2 for serial communiaction
 lcd1_port_init();
 servo_port_init();
 timer1_init();
 ir_port_init();
 adc_init();
 motion_port_init();  //Initializes all the ports
 left_position_encoder_interrupt_init();
 right_position_encoder_interrupt_init();
 lcd1_set_4bit();
 lcd1_init();
 barrior_gate_down(); 	
 empty=0; //currently hardcoding it but eed to sense & update in follo loop


 wait_for_class_label=0;
 extract_fruit=1;
 ir_dispence=255;
 ir_ip=255;
 fruit_sensed=1;
 ip_hungry=1;

 sei();   //Enables the global interrupts

}


void myfun(){
_delay_ms(fun_delay);
lcd1_cursor(1,2);
lcd1_string("---------------------------------");
lcd1_cursor(2,2);
lcd1_string("---------------------------------");
lcd_print(1,14,ir_dispence,3);
lcd_print(2,14,ir_ip,3);
senseAll();
}

void senseAll(){
		//these are defined in values.h ir_dispense_channel 5
	    //ir_ip_channel 4
		ir_dispence = ADC_Conversion(ir_dispence_channel );
		if(ir_dispence > dispence_proximity_threshold){ fruit_sensed=0;}
		else { fruit_sensed=1; }

		ir_ip = ADC_Conversion(ir_ip_channel);
		if(ir_ip > ip_proximity_threshold ) { ip_hungry=1;}
		else { ip_hungry=0;}
		//empty=sense sharp sensor
}

//Main Function
int main(void)
{
	init_devices();
	init_signal=0;start_signal=0;
	lcd1_cursor(1,1);
	lcd1_string("Hukum mere Aakaa....");
	lcd1_cursor(2,1);
	lcd1_string("Waiting for init_signal");
	while(1){
		while(!init_signal);
		while(!start_signal);
		while(start_signal)
		{
			senseAll();
			while (start_signal && !ip_hungry){
				extract_fruit=0;
				stop(); //stops the DC motor controlling piston
				_delay_ms(500);
				//------- send signal to PC asking for class label of this fruit
				cli();
				UDR2=0x3F;
				wait_for_class_label=1;
				sei();
				//---------------
				
				lcd1_cursor(1,2);
				lcd1_string(":) got fruit");
				lcd1_cursor(2,2);
				lcd1_string("piston stop");
				lcd_print(1,14,ir_dispence,3);
				lcd_print(2,14,ir_ip,3);	
								
				while(start_signal && wait_for_class_label); //busy waiting for class label from PC
				senseAll();
			}
			myfun();

			if (start_signal && ip_hungry ){
					extract_fruit=1;
					lcd1_cursor(1,5);
					lcd1_string("hungry!!");
					lcd_print(1,14,ir_dispence,3);
				 	lcd_print(2,14,ir_ip,3);	
			}
		
			while(start_signal && ip_hungry && extract_fruit && !fruit_sensed)
			{
					senseAll();

					backward_mm(dc_motor_move_distance_back); //Moves piston backward by dc_motor_move_distance mm
					if(fruit_sensed) break;
					//_delay_ms(dc_motor_motion_delay);
					//stop();
					//_delay_ms(dc_motor_motion_delay);
					
					lcd1_cursor(1,2);
					lcd1_string("IPU hungry");
					lcd1_cursor(2,2);
					lcd1_string("mvng back");
					lcd_print(1,14,ir_dispence,3);
					lcd_print(2,14,ir_ip,3);	
			}
			myfun();			
	
			while(start_signal && fruit_sensed && ip_hungry && extract_fruit )
			{	
				lcd1_cursor(1,2);
				lcd1_string("ip:no frt");
				lcd1_cursor(2,2);
				lcd1_string("mvng ahead");
				lcd_print(1,14,ir_dispence,3);
				lcd_print(2,14,ir_ip,3);

				forward_mm(dc_motor_move_distance); //Moves piston forward by dc_motor_move_distance distance
				//_delay_ms(dc_motor_motion_delay);
				//stop();
				//_delay_ms(dc_motor_motion_delay);
				senseAll();	
			}
			myfun();			

		}//end of while loop of start 
	} //end of outermost while(1)

	/*
	//now fruit bucket is empty
	lcd1_cursor(1,2);
	lcd1_string("Fruit bucket is empty         ");
	lcd1_cursor(2,2);
	lcd1_string("Now my time to relax :D       ");
	*/
}

void main2(){
	init_devices();
	lcd1_set_4bit();
	lcd1_init();

	lcd1_cursor(1,3);
	lcd1_string("hi");
	/*extract_fruit=1;
	unsigned char x;
	for(x=1; x<6;x++){
	barrior_gate_up();
	_delay_ms(1000);
	barrior_gate_down();
		_delay_ms(1000);
	}
	/*unsigned char x;
	for(x=1; x<41;x++){
		servo_1(x);
		
		lcd_print(2,14,x,3);
		_delay_ms(1000);
		
	}
	for(; x>0;x--){
		servo_1(x);
		
		lcd_print(2,14,x,3);
		_delay_ms(1000);
		
	}
	int cnt;
	cnt=0;
	while (cnt<6){
	while(1){
	servo_1(CLASS1_DIR);
	_delay_ms(2000);
	servo_1(CLASS2_DIR);
	_delay_ms(2000);
	servo_1(CLASS3_DIR);
	_delay_ms(2000);
	
	}*/
	while(1){
		_delay_ms(2000);
		backward_mm(dc_motor_move_distance); //Moves piston backward by dc_motor_move_distance mm
		
			_delay_ms(2000);
		forward_mm(dc_motor_move_distance); //Moves piston forward by dc_motor_move_distance distance
	}
}
