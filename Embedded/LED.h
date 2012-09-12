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
#ifndef LED_H_INCLUDED
#define LED_H_INCLUDED

/* Defines */
#define     N_LED              20

/* Macros */
#define RGB_2_BYTES(rr,gg,bb)   (((uint16_t)gg & 0x001F )<<10) | (((uint16_t)bb & 0x001F)<<5) | ((uint16_t)rr & 0x001F)

/* Type defs */

/* Function Prototypes */
void InitializeLED(void);
void clearDisplay(void);
void updateDisplay(void);

void SetAllPixels(uint16_t cVal);

void cVal2RGB(uint16_t cVal, uint8_t* rVal, uint8_t* gVal, uint8_t* bVal);

/** r,g,b values are from 0 to 1
  * h = [0,360], s = [0,1], v = [0,1]
  *		if s == 0, then h = -1 (undefined) */
void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v );
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );

#endif // LED_H_INCLUDED
