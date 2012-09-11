/*
 *  LED.c
 *  Functions to control the LEDs based on the LPD6803 IC
 *
 * @author Mike Kane
 *     This file is part of LED-Shirt-w-Android-Control.
 *     
 *     Copyright (C) 2011 Michael Kane
 *     https://github.com/thisIsMikeKane/LED-Shirt-w-Android-Control#led-shirt-w-android-control
 *     
 *     LED-Shirt-w-Android-Control is free software: you can redistribute it 
 *     and/or modify it under the terms of the GNU General Public License as 
 *     published by the Free Software Foundation, either version 3 of the 
 *     License, or (at your option) any later version.
 *     
 *     LED-Shirt-w-Android-Control is distributed in the hope that it will 
 *     be useful, but WITHOUT ANY WARRANTY; without even the implied warranty 
 *     of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *     
 *     You should have received a copy of the GNU General Public License
 *     along with LED-Shirt-w-Android-Control.  
 *     If not, see <http://www.gnu.org/licenses/>.
 */
#include "definitions.h"

/* Global variables */
uint16_t    cValOfLED[N_LED];
uint8_t     curLED=0;
BOOL        cValIsDirty;
uint8_t     writeState;

/* Functions */
/** The bootup process for the LED display */
void InitializeLED(void)
{
    uint32_t i;

    /* Restart at pixel 0 */
    curLED = 0;
    writeState = 0;

    /* Send some zeros for good luck */
    SPI_MasterTransceive(0x00);
    SPI_MasterTransceive(0x00);
    SPI_MasterTransceive(0x00);
    SPI_MasterTransceive(0x00);

    /* Wait a bit */
    delayTime(30000);
    delayTime(30000);

    /* Set all pixels off */
    clearDisplay();
}


/** Set all pixels off */
void clearDisplay(void)
{
    uint8_t i;

    for(i=0;i<N_LED;i++)
    {
        cValOfLED[i] = 0x0000;
    }

    updateDisplay();
}


void updateDisplay(void)
{
    uint8_t i;

    SPI_TX(0x00);
    SPI_TX(0x00);
    SPI_TX(0x00);
    SPI_TX(0x00);

    for(i=0;i<N_LED;i++)
    {
        SPI_TX(0x80 | (uint8_t)(cValOfLED[i] >> 8));
        SPI_TX((uint8_t)cValOfLED[i]);
    }
}


/** Set all pixels to the same color */
void SetAllPixels(uint16_t cVal)
{
    uint16_t i;


    for(i=0;i<N_LED;i++)
    {
        cValOfLED[i] = cVal;
    }

}

/** Return RGB values between 0-31 for a given cVal word */
void cVal2RGB(uint16_t cVal, uint8_t* rVal, uint8_t* gVal, uint8_t* bVal)
{
    *rVal = (uint8_t)(cVal       & 0x001F);
    *gVal = (uint8_t)((cVal>>10) & 0x001F);
    *bVal = (uint8_t)((cVal>>5 ) & 0x001F);
}

/** r,g,b values are from 0 to 1
  * h = [0,360], s = [0,1], v = [0,1]
  *		if s == 0, then h = -1 (undefined) */
/* FROM: http://stackoverflow.com/a/6809200 */
void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
{
	float min, max, delta;

	min = ( (r<g) && (r<b) ) ? (r) : (((g<r) && (g<b)) ? (g) :(b)) ;
	max = ( (r>g) && (r>b) ) ? (r) : (((g>r) && (g>b)) ? (g) :(b)) ;
	*v = max;				// v

	delta = max - min;

	if( max != 0 )
		*s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}

	if( r == max )
		*h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		*h = 4 + ( r - g ) / delta;	// between magenta & cyan

	*h *= 60;				// degrees
	if( *h < 0 )
		*h += 360;

}

/** r,g,b values are from 0 to 1
  * h = [0,360], s = [0,1], v = [0,1]
  *		if s == 0, then h = -1 (undefined) */
/* FROM: http://stackoverflow.com/a/6809200 */
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
	int i;
	float f, p, q, t;

	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}

	h /= 60;			// sector 0 to 5
	i = floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1 - s );
	q = v * ( 1 - s * f );
	t = v * ( 1 - s * ( 1 - f ) );

	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}
}
