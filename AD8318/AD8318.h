//=================================================================
//
//  Simple library to calculate dBm from the output voltage of an AD8318
//
//  http://elsenaju.info/Rechner/Ausgleichsgerade.htm
//  http://www.xuru.org/rt/PR.asp#Manually
//
//
//	Version 1.8.1
//
//  (c) Dietmar Krause, DL2SBA 2017
//
//  License see https://creativecommons.org/licenses/by/4.0/

#ifndef _AD8318_H
#define _AD8318_H

class AD8318 {
  public:
	/**
	 *  Create a new instance 
	 *  
	 */
    AD8318( void );

		
	/**
	 *  convert value to real value
	 *
	 *	pX	Value to convert
	 *	pdBm	true	convert to dBm
	 *			false	convert to W
	 */
	float convert(uint16_t pX, uint8_t pdBm);

	/**
	 *  get the unit as string depending on the conversion mode and the measured value
	 */
	const char * getUnit(void);
	
	/**
	 *  EEPROM interface
	 *  
	 *  read a set of k-factors from EEPROM at given offset
	 */
	void readEEPROM( uint16_t pOffset );
	
	/**
	 *  PROGMEM interface
	 *  
	 *  read a set of k-factors from progmem at given offset
	 */
	void readPROGMEM( uint16_t pOffset );
	
	/**
	 *  set the poweroffset in dBm
	 */
	inline void setOffset(int8_t pOffset) { mOffset = pOffset; };
	
	/**
	 *  get the size of a parameter set in float objects
	 */
	inline uint16_t getParmSize(void) { return 4; };
	
	
    float mK0;
    float mK1;
    float mK2;
    float mK3;
	
	int8_t mOffset;
	int8_t mUnit;
};


#endif
