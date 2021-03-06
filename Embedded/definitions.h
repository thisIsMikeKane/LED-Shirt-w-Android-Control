/*
 *  definitions.c
 *  This file contains functions that will be used throughout program to access hardware features
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

#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

/* AVR Interrupt enable/disable functions */
#define Enable_GLOBAL_INT()  SEI();
#define Disable_GLOBAL_INT() CLI();

/* Port B */
#define CSN               	 0  // PB.0 - Output: Radio Enable (active low)
#define SCK                	 1  // PB.1 - Output: SPI Serial Clock (SCLK)
#define MOSI               	 2  // PB.2 - Output: SPI Master out - slave in (MOSI)
#define MISO               	 3  // PB.3 - Input:  SPI Master in - slave out (MISO)

/* Type definitions */
typedef unsigned char		BOOL;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned char		UINT8;
typedef unsigned short		UINT16;
typedef unsigned long		UINT32;
typedef signed char			INT8;
typedef signed short		INT16;
typedef signed long			INT32;
typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned long		uint32_t;
typedef signed char			int8_t;
typedef signed short		int16_t;
typedef signed long			int32_t;

#ifndef BIT
#define BIT(x)	(1 << (x))
#endif

#define     FALSE           0
#define     TRUE            1
#define     false           0
#define     true            1

/* Miscellaneous Functions */
void delayTime(int timeout);

#define LSB(word)	( word&0xFF);
#define MSB(word)   ( word>>8  );
#define BM(n) 		(1 << (n))

/* AVR assembly level functions */
#define SEI()	asm("sei")
#define CLI()	asm("cli")
#define NOP()	asm("nop")
#define _SEI()	asm("sei")
#define _CLI()	asm("cli")
#define _NOP()	asm("nop")

/* UART1 functions */
#define CLEAR_UART1_TX_INT()        do { UCSR1A &= ~BM(UDRE1); } while (0)

#define ENABLE_UART1_RX_INT()       do { UCSR1B |= BM(RXCIE1); } while (0)
#define DISABLE_UART1_RX_INT()      do { UCSR1B &= ~BM(RXCIE1); } while (0)

#define UART1_WAIT()                do { while (!(UCSR1A & BM(UDRE1))); CLEAR_UART1_TX_INT(); } while (0)
#define UART1_SEND(x)               do { UDR1 = (x); } while (0)
#define UART1_WAIT_AND_SEND(x)      do { UART1_WAIT(); UART1_SEND(x); } while (0)
#define UART1_RECEIVE(x)            do { (x) = UDR1; } while (0)
#define UART1_WAIT_AND_RECEIVE(x)   do { UDR = 0; UART1_WAIT(); UART1_RX(x); } while (0)

/* Project files includes */
//#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iom128v.h>
#include <math.h>
#include "Initialize.h"
#include "Serial.h"
#include "spi.h"
#include "LED.h"
#include "Animations.h"

#endif // DEFINITIONS_H_INCLUDED
