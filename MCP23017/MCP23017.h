/* MCP23017 Library for mbed
 * Copyright (c) 2014, Takuya Urakawa
 * 
 * This library is released under the MIT License
 * See http://opensource.org/licenses/mit-license.php
 */

#ifndef _MCP23017_H_
#define _MCP23017_H_

#include "mbed.h"

/** \def usefull defines */
#define MCP23017_DEFAULT_ADDR 0x40
#define MCP23017_PORTA 0x00
#define MCP23017_PORTB 0x01

namespace MCP23017 {	
	
/** MCP23017 class
 *
 *	Allow access MCP23017 I2C 16bit io expander IC
 *  But this library can not controll interrupt functions
 *
 *  Example:
 *  @code
 *  #include "mbed.h"
 *  #include "MCP23017.h"
 *
 *  I2C i2c(dp5, dp27); // LPC1114
 *  MCP23017::MCP23017 io(&i2c,MCP23017_DEFAULT_ADDR);
 *
 *  int main(void)
 *  {	
 *	  i2c.frequency(400000);
 *	
 *	  // software reset
 *	  io.init();
 *	
 *	  io.setDirrection(MCP23017_PORTA, 0xFF);    // set all of the PORTA pins to input
 *	  io.setDirrection(MCP23017_PORTB, 0x00);    // set all of the PORTB pins to output
 *	  io.setPullUp(MCP23017_PORTA, 0xFF);        // activate all of the PORTA pin pull-ups
 *	  io.setInputPolarity(MCP23017_PORTA, 0xFF); // invert all of the PORTA pins input polarity
 *
 *    while(true){
 *	  	// write PORTA to PORTB
 *  	io.write(1,io.read(0));
 *	  }
 *
 *	  return 0;
 *  }
 *  @endcode
 */
class MCP23017{
public:
	
	/** Constructor
	 *
	 *  @param _i2c   pointer to I2C class
	 *  @param _addr  i2c device address
	 */
    MCP23017(I2C &_i2c, char _addr);

	/** Software Reset
	 */
	void init(void);

	/** Configure MCP23017 
	 *  Write IOCON register
	 *  See MCP23017 datasheet p18
	 *  http://ww1.microchip.com/downloads/jp/DeviceDoc/21952b.pdf
	 *  
	 *  @attention BANK bit must set 0
	 *  
	 *  @param _value  write byte  
	 */
	void setConfig(char _value);

	/** Set I/O dirrection
	 *  
	 *  @param _port   port(MCP23017_PORTA or MCP23017_PORTB)
	 *  @param _value  write byte(0=OUTPUT, 1=INPUT)
	 */
	void setDirrection(char _port, char _value);
	
	/** Set pullup
	 *  
	 *  @param _port   port (MCP23017_PORTA or MCP23017_PORTB)
	 *  @param _value  write byte (0=Disable, 1=Enable)
	 */
	void setPullUp(char _port, char _value);
	
	/** Set Input Polarity
	 *  
	 *  @param _port   port(MCP23017_PORTA or MCP23017_PORTB)
	 *  @param _value  write byte (0=normal, 1=inverted)
	 */
	void setInputPolarity(char _port, char _value);
	
	/** Write GPIO
	 *
	 *  @param _port   port(MCP23017_PORTA or MCP23017_PORTB)
	 *  @param _value  write byte 
	 */
	void write(char _port, char _value);
	
	/** Read GPIO
	 *
	 *  @param _port   port(MCP23017_PORTA or MCP23017_PORTB)
	 *  @return        byte
	 */
	char read(char _port);
	
	
	// void setInterruptConfig(char _port, char _intcon, char _defval);
	// void interrupt(char _port, char _value);
	// char readIntcap(char _port);


private:
    I2C  &mI2c;
	char mWriteAddr;
	char mReadAddr;


	inline void i2cSend(char _command, char _data1, char _data2){
		char commands[3];
		commands[0] = _command;
		commands[1] = _data1;
		commands[2] = _data2;

		mI2c.write(mWriteAddr, commands, 3);
	}
	
	inline void i2cSend(char _command, char _data1){
		char commands[2];
		commands[0] = _command;
		commands[1] = _data1;
		
		mI2c.write(mWriteAddr, commands, 2);
	}
	
	inline char i2cRead(char _command){
		char commands[2];
		commands[0] = _command;
		mI2c.write(mWriteAddr, commands, 1);
		//return (char)mI2c->read(0);
		mI2c.read(mReadAddr,commands,1);
		return commands[0];
	}
	
	
	// registors
	enum MCP23017_REG {
		// IO Dirrection
		REG_IODIR = 0x00,

		// Input polarity
		REG_IPOL = 0x02,

		// Interrupt-On-Change enable
		REG_GPINTEN = 0x04,

		// Default intterupt comparison value
		// Refer to INTCON
		REG_DEFVAL = 0x06,

		// Intterupt compare value
		// 1 = compare against DEFVAL value
		// 0 = compare against the previous value
		REG_INTCON = 0x08,

		// IC setting
		// bank/mirror/seqop/disslw/haen/odr/intpol/notimp
		// must be set bank = 0 for using this library
		// See MCP23017 datasheet
		REG_IOCON = 0x0A,

		// Pullup
		REG_GPPU = 0x0C,

		// Interrupt Flag
		// 1 = pin caused interrupt
		// 0 = no interrupt 
		// Read-Only
		REG_INTF = 0x0E,

		// Interrupt capture
		// value of GPIO at time of last interrupt
		// Read-Only
		REG_INTCAP = 0x10,

		// GPIO
		REG_GPIO = 0x12,

		// Output Latch
		REG_OLAT = 0x14
	};
	
}; // end of class
}; // end of namespace

#endif

