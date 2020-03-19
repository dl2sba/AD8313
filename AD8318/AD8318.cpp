//=================================================================
//
//  Simple library to access the AD8318 ADC with an Arduino. 
//
//	See https://github.com/dl2sba/AD8318
//
//	Version 1.8
//
//  (c) Dietmar Krause, DL2SBA 2017
//
//  License see https://creativecommons.org/licenses/by/4.0/

#include <arduino.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>
#include "AD8318.h"


AD8318::AD8318 ( void ) {
	mK0 = 2.804742194E1;
	mK1 = -3.432439417E-2;
	mK2 = 5.570579072E-6;
	mK3 = -1.015882955E-9;
	mOffset = 0;
}

/**
 *  
 */
void AD8318::readPROGMEM( uint16_t pK0 ) {
	mK0 = pgm_read_float_near(pK0 + 0 * sizeof mK0);
	mK1 = pgm_read_float_near(pK0 + 1 * sizeof mK0);
	mK2 = pgm_read_float_near(pK0 + 2 * sizeof mK0);
	mK3 = pgm_read_float_near(pK0 + 3 * sizeof mK0);
}

/**
 *  
 */
void AD8318::readEEPROM( uint16_t pEEPROMOffset ) {
	EEPROM.get(pEEPROMOffset + 0 * sizeof mK0, mK0);
	EEPROM.get(pEEPROMOffset + 1 * sizeof mK0, mK1);
	EEPROM.get(pEEPROMOffset + 2 * sizeof mK0, mK2);
	EEPROM.get(pEEPROMOffset + 3 * sizeof mK0, mK3);
}

/**
 *  convert value to real value
 */
float AD8318::convert(uint16_t pX, uint8_t pdBm) {
	float val = (mK0 + mK1 * pX + mK2 * pX * pX + mK3 * pX * pX * pX) - mOffset;

	//	 we are in dBm mode ? 
	if ( pdBm ) {
		//	yes
		mUnit = -1;
	} else {
		val =  pow(10.0, val / 10.0);
		
		//	watt mode
		if (val < 0.000001 ) {
			//	nW
			mUnit = 4;
			val *= 1e9;
		} else if (val < 0.001 ) {
			//	nW
			mUnit = 3;
			val *= 1e6;
		} else if (val < 1.0) {
			//	uW
			mUnit = 2;
			val *= 1e3;
		} else if (val < 1000.0 ) {
			//	mW
			mUnit = 1;
		} else {
			//	W
			mUnit = 0;
			val /= 1e3;
		}
	}
	return val;
}
	

/**
 *  get the unit as string depending on the conversion mode and the measured value
 */
const char * AD8318::getUnit(void) {
	static char unit[3];
	if ( mUnit == -1 ) {
		unit[0] = 'd';
		unit[1] = 'B';
	} else {
		unit[1] = 'W';
		
		if ( mUnit == 0 ) {
			unit[0] = ' ';
		} else  if ( mUnit == 1 ) {
			unit[0] = 'm';
		} else  if ( mUnit == 2 ) {
			unit[0] = 'µ';
		} else  if ( mUnit == 3 ) {
			unit[0] = 'n';
		} else  if ( mUnit == 4 ) {
			unit[0] = 'p';
		} else {
			unit[0] = '?';
		}
	}
	
	unit[2] = 0;
	return unit;
}
