#include "Wire.h"
#include "USART.h"
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define DEVICE (0x53) // Device address as specified in data sheet
uint8_t _buff[1];
uint8_t _buff2[2];
char POWER_CTL = 0x2D; //Power Control Register
char DATA_FORMAT = 0x31;
char DATAX0 = 0x32; //X-Axis Data 0
char DATAX1 = 0x33; //X-Axis Data 1
void readAccel();
void writeTo(uint8_t address, uint8_t val) ;
void readFrom(uint8_t address, int num, uint8_t _buff[]) ;
int main()
{
	beginAsMaster(); // join i2c bus (address optional for master)
	sei();
	initUSART();
	printString("init \n");
	//Put the ADXL345 into +/- 4G range by writing the value 0x01 to the DATA_FORMAT register.
	writeTo(DATA_FORMAT, 0x01);
	//Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
	writeTo(POWER_CTL, 0x08);
while(1)
{
	readAccel(); // read the x/y/z tilt
	_delay_ms(500); // only read every 0,5 seconds
}
}

void readAccel() {
	uint8_t howManyuint8_tsToRead = 1;
	readFrom( DATAX0, howManyuint8_tsToRead, _buff);
	readFrom( DATAX1, howManyuint8_tsToRead, _buff2); //read the acceleration data from the ADXL345
	// each axis reading comes in 10 bit resolution, ie 2 uint8_ts. Least Significat uint8_t first!!
	// thus we are converting both uint8_ts in to one int
	int x = (((int)_buff2[0]) << 8) | _buff[0];
	printString("x: ");
	printBinaryByte( x );
	
}
void writeTo(uint8_t address, uint8_t val) {
	beginTransmission(DEVICE); // start transmission to device
	write(address); // send register address
	write(val); // send value to write
	endTransmissionThenStop(); // end transmission
}
// Reads num uint8_ts starting from address register on device in to _buff array
void readFrom(uint8_t address, int num, uint8_t _buff[]) {
	beginTransmission(DEVICE); // start transmission to device
	write(address); // sends address to read from
	endTransmissionThenStop(); // end transmission
	beginTransmission(DEVICE); // start transmission to device
	requestFrom(DEVICE, num); // request num uint8_ts from device
	int i = 0;
	while(available()) // device may send less than requested (abnormal)
	{
		_buff[i] = read(); // receive a uint8_t
		i++;
	}
	endTransmissionThenStop(); // end transmission
}