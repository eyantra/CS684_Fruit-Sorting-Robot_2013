#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define RS 0
#define RW 1
#define EN 2
#define lcd1_port PORTC

#define sbit(reg,bit)	reg |= (1<<bit)			// Macro defined for Setting a bit of any register.
#define cbit(reg,bit)	reg &= ~(1<<bit)		// Macro defined for Clearing a bit of any register.

void init_ports();
void lcd_reset();
void lcd_init();
void lcd1_wr_command(unsigned char);
void lcd1_wr_char(char);
void lcd1_line1();
void lcd1_line2();
void lcd1_string(char*);

unsigned int temp;
unsigned int unit;
unsigned int tens;
unsigned int hundred;
unsigned int thousand;
unsigned int million;

//Function to configure LCD port
void lcd1_port_config (void)
{
 DDRC = DDRC | 0xF7; //all the LCD pin's direction set as output
 PORTC = PORTC & 0x80; // all the LCD pins are set to logic 0 except PORTC 7
}

//Function to Initialize PORTS
void lcd1_port_init()
{
	lcd1_port_config();
}

//Function to Reset LCD
void lcd1_set_4bit()
{
	_delay_ms(1);

	cbit(lcd1_port,RS);				//RS=0 --- Command Input
	cbit(lcd1_port,RW);				//RW=0 --- Writing to LCD
	lcd1_port = 0x30;				//Sending 3
	sbit(lcd1_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd1_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd1_port,RS);				//RS=0 --- Command Input
	cbit(lcd1_port,RW);				//RW=0 --- Writing to LCD
	lcd1_port = 0x30;				//Sending 3
	sbit(lcd1_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd1_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd1_port,RS);				//RS=0 --- Command Input
	cbit(lcd1_port,RW);				//RW=0 --- Writing to LCD
	lcd1_port = 0x30;				//Sending 3
	sbit(lcd1_port,EN);				//Set Enable Pin
	_delay_ms(5);					//Delay
	cbit(lcd1_port,EN);				//Clear Enable Pin

	_delay_ms(1);

	cbit(lcd1_port,RS);				//RS=0 --- Command Input
	cbit(lcd1_port,RW);				//RW=0 --- Writing to LCD
	lcd1_port = 0x20;				//Sending 2 to initialise LCD 4-bit mode
	sbit(lcd1_port,EN);				//Set Enable Pin
	_delay_ms(1);					//Delay
	cbit(lcd1_port,EN);				//Clear Enable Pin

	
}

//Function to Initialize LCD
void lcd1_init()
{
	_delay_ms(1);

	lcd1_wr_command(0x28);			//LCD 4-bit mode and 2 lines.
	lcd1_wr_command(0x01);
	lcd1_wr_command(0x06);
	lcd1_wr_command(0x0E);
	lcd1_wr_command(0x80);
		
}

	 
//Function to Write Command on LCD
void lcd1_wr_command(unsigned char cmd)
{
	unsigned char temp;
	temp = cmd;
	temp = temp & 0xF0;
	lcd1_port &= 0x0F;
	lcd1_port |= temp;
	cbit(lcd1_port,RS);
	cbit(lcd1_port,RW);
	sbit(lcd1_port,EN);
	_delay_ms(5);
	cbit(lcd1_port,EN);
	
	cmd = cmd & 0x0F;
	cmd = cmd<<4;
	lcd1_port &= 0x0F;
	lcd1_port |= cmd;
	cbit(lcd1_port,RS);
	cbit(lcd1_port,RW);
	sbit(lcd1_port,EN);
	_delay_ms(5);
	cbit(lcd1_port,EN);
}

//Function to Write Data on LCD
void lcd1_wr_char(char letter)
{
	char temp;
	temp = letter;
	temp = (temp & 0xF0);
	lcd1_port &= 0x0F;
	lcd1_port |= temp;
	sbit(lcd1_port,RS);
	cbit(lcd1_port,RW);
	sbit(lcd1_port,EN);
	_delay_ms(5);
	cbit(lcd1_port,EN);

	letter = letter & 0x0F;
	letter = letter<<4;
	lcd1_port &= 0x0F;
	lcd1_port |= letter;
	sbit(lcd1_port,RS);
	cbit(lcd1_port,RW);
	sbit(lcd1_port,EN);
	_delay_ms(5);
	cbit(lcd1_port,EN);
}


//Function to bring cursor at home position
void lcd1_home()
{
	lcd1_wr_command(0x80);
}


//Function to Print String on LCD
void lcd1_string(char *str)
{
	while(*str != '\0')
	{
		lcd1_wr_char(*str);
		str++;
	}
}

//Position the LCD cursor at "row", "column".

void lcd1_cursor (char row, char column)
{
	switch (row) {
		case 1: lcd1_wr_command (0x80 + column - 1); break;
		case 2: lcd1_wr_command (0xc0 + column - 1); break;
		case 3: lcd1_wr_command (0x94 + column - 1); break;
		case 4: lcd1_wr_command (0xd4 + column - 1); break;
		default: break;
	}
}

//Function To Print Any input value upto the desired digit on LCD
void lcd1_print (char row, char coloumn, unsigned int value, int digits)
{
	unsigned char flag=0;
	if(row==0||coloumn==0)
	{
		lcd1_home();
	}
	else
	{
		lcd1_cursor(row,coloumn);
	}
	if(digits==5 || flag==1)
	{
		million=value/10000+48;
		lcd1_wr_char(million);
		flag=1;
	}
	if(digits==4 || flag==1)
	{
		temp = value/1000;
		thousand = temp%10 + 48;
		lcd1_wr_char(thousand);
		flag=1;
	}
	if(digits==3 || flag==1)
	{
		temp = value/100;
		hundred = temp%10 + 48;
		lcd1_wr_char(hundred);
		flag=1;
	}
	if(digits==2 || flag==1)
	{
		temp = value/10;
		tens = temp%10 + 48;
		lcd1_wr_char(tens);
		flag=1;
	}
	if(digits==1 || flag==1)
	{
		unit = value%10 + 48;
		lcd1_wr_char(unit);
	}
	if(digits>5)
	{
		lcd1_wr_char('E');
	}
	
}

void lcd1_init_devices (void)
{
 cli(); //Clears the global interrupts
 lcd1_port_init();
 sei();   //Enables the global interrupts
}		

//Main Function
/*int main(void)
{
	
	init_devices();
	lcd1_set_4bit();
	lcd1_init();
	while(1)
	{
		lcd1_cursor(1,3);
		lcd1_string("FIRE BIRD 5");
		lcd1_cursor(2,1);
		lcd1_string("NEX ROBOTICS IND");
	}
}
*/
