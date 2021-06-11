/* MCP23017 Library for mbed
 * Copyright (c) 2014, Takuya Urakawa
 * 
 * This library is released under the MIT License
 * See http://opensource.org/licenses/mit-license.php
 */

#include "MCP23017.h"

namespace MCP23017 {

MCP23017::MCP23017(I2C &_i2c, char _addr): 
	mI2c(_i2c)
{
	mWriteAddr = _addr & 0xFE;
	mReadAddr = _addr | 0x01;
}

void MCP23017::init(void){
	
	// init config
	i2cSend(REG_IOCON, 0);

	// port 0
	i2cSend(REG_GPIO, 0, 0);

	// port dirrection all input
	i2cSend(REG_IODIR, 0, 0);

	// interupt off
	i2cSend(REG_GPINTEN, 0, 0);

	// clear interrupt
	read(MCP23017_PORTA);
	read(MCP23017_PORTB);
}

void MCP23017::setConfig(char _value){
	
	i2cSend(REG_IOCON, _value);
}

void MCP23017::setDirrection(char _port, char _value){
	
	i2cSend(REG_IODIR + _port, _value);
}

void MCP23017::setPullUp(char _port, char _value){
	
	i2cSend(REG_GPPU + _port, _value);
}

void MCP23017::setInputPolarity(char _port, char _value){
	
	i2cSend(REG_IPOL + _port, _value);
}


void MCP23017::write(char _port, char _value){
	
	i2cSend(REG_GPIO + _port, _value);
}

char MCP23017::read(char _port){
	
	return i2cRead(REG_GPIO + _port);
}

//void MCP23017::setInterruptConfig(char _port, char _intcon, char _defval){
//	
//	i2cSend(REG_INTCON + _port, _intcon);
//	i2cSend(REG_DEFVAL + _port, _defval);

//}

//void MCP23017::interrupt(char _port, char _value){

//	i2cSend(REG_GPINTEN + _port, _value);
//	//Read from GPIO to clear interrupt flag
//	read(_port);
//}

//char MCP23017::readIntcap(char _port){

//	return i2cRead(REG_INTCAP + _port);
//}

} // end of namespace

