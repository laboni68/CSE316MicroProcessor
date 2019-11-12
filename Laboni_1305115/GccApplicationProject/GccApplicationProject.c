/*
 * GccApplicationProject.c
 *
 * Created: 5/15/2017 9:55:07 PM
 *  Author: Laboni
 */ 
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include<avr/io.h>
#define F_CPU 8000000UL
#include<C:\Users\Laboni\Documents\Atmel Studio\6.2\GccApplicationProject\GccApplicationProject\lcd.h>
//#include<lcd.h>
#include<util/delay.h>
#include <stdio.h>
#include <stdlib.h>



void usartinit();
unsigned int z;
unsigned char cmd1[]={"AT"};
unsigned char cmd2[]={"AT+CMGF=1"};
unsigned char cmd3[]={"AT+CMGS="};
unsigned char cmd4[]={"Impurity detected! Change water."};
unsigned char cmd5[]={"+8801842291568"};
void sendMessage();
int main(void)
{

	DDRD = 0xff;
	DDRC = 0xff;
	MCUCSR = (1<<JTD);
	MCUCSR = (1<<JTD);
	unsigned int analog_light_sensor_value;
	Lcd4_Init();
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Micro Controller");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Course No:316");
	_delay_ms(500);
	_delay_ms(500);
	Lcd4_Clear();
	Lcd4_Set_Cursor(1,0);
	Lcd4_Write_String("Laboni/115");
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("Farhin/105");
	_delay_ms(500);
	int min=10,max=1023,flag=0,dirt=0;
	int MAX_LED=0,MIN_ROOM_LIGHT=9999;
	ADMUX = 0b01100000;
	ADCSRA = 0b10000010;
	usartinit();
	while(1)
	{
		char buffer[5];
		char max_val[5];
		char min_val[5];
		Lcd4_Clear();
		Lcd4_Set_Cursor(1,0);
		Lcd4_Write_String("Value:");
		Lcd4_Set_Cursor(1,7);
		ADCSRA |= (1<<ADSC);
		while( ADCSRA & (1<<ADSC) )
		;
		analog_light_sensor_value= ADCL;
		analog_light_sensor_value = (analog_light_sensor_value>>6) | (ADCH<<2);
		snprintf(buffer, sizeof buffer, "%d", analog_light_sensor_value);
		Lcd4_Write_String(buffer);
		if(analog_light_sensor_value>MAX_LED&&MAX_LED<=max)
			MAX_LED=analog_light_sensor_value;
		if(analog_light_sensor_value<MIN_ROOM_LIGHT&&MIN_ROOM_LIGHT>min)
			MIN_ROOM_LIGHT=analog_light_sensor_value;
		if(flag==0&&analog_light_sensor_value<=(MAX_LED-20)&&analog_light_sensor_value>(MIN_ROOM_LIGHT+30))
		{
			dirt++;
			if(dirt==5)
			{
				flag=1;
				sendMessage();	
			}
		}
		Lcd4_Set_Cursor(2,0);
		Lcd4_Write_String("MX:");
		Lcd4_Set_Cursor(2,3);
		snprintf(max_val, sizeof max_val, "%d", MAX_LED);
		Lcd4_Write_String(max_val);
		Lcd4_Set_Cursor(2,8);
		Lcd4_Write_String("Mn:");
		Lcd4_Set_Cursor(2,11);
		snprintf(min_val, sizeof min_val, "%d", MIN_ROOM_LIGHT);
		Lcd4_Write_String(min_val);
		_delay_ms(500);
		
	}
}

void usartinit()
{
	UBRRH=00;
	UBRRL=77;
	UCSRB|=(1<<RXEN)|(1<<TXEN);
	UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}
void sendMessage()
{
	//Start of message sending code
	for(z=0;cmd1[z]!='\0';z++)
	{
		UDR = cmd1[z];
		_delay_ms(100);
	}
	UDR = ('\r');
	_delay_ms(500);
	for(z=0;cmd2[z]!='\0';z++)
	{
		UDR = cmd2[z];
		_delay_ms(100);
	}
	UDR = ('\r');
	_delay_ms(500);
	for(z=0;cmd3[z]!='\0';z++)
	{
		UDR = cmd3[z];
		_delay_ms(100);
	}
	UDR = ('\"');
	_delay_ms(100);
	for(z=0;cmd5[z]!='\0';z++)
	{
		UDR = cmd5[z];
		_delay_ms(100);
	}
	UDR = ('\"');
	_delay_ms(100);
	UDR = ('\r');
	_delay_ms(500);
	for(z=0;cmd4[z]!='\0';z++)
	{
		UDR = cmd4[z];
		_delay_ms(100);
	}
	UDR = (26);// ctrlZ-> to send the messge
	_delay_ms(100);
	Lcd4_Set_Cursor(2,0);
	Lcd4_Write_String("MSG SENT!");
	_delay_ms(500);
	Lcd4_Clear();
	//end of message sending code
}