/*
 *  Initialize.c
 *  This file contains functions to initialize the hardware on the MCU
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

#include <stdlib.h>
#include <stdio.h>
#include <iom128v.h>
#include "definitions.h"

void InitializePorts(void)
{
    /* Special Function I/O register initialization */
	/* Pullups in I/O ports are disabled */
	/* Keeps I/0 pins safe from being changed randomly by pullups */
	SFIOR |= BIT(PUD);

    /* Set MOSI, SCK, and SS output, all others input
     * pin    2    1       0  of Port B.*/
    DDRB = DDRB | BIT(MOSI) | BIT(SCK) | BIT(CSN);

    /* Set Chip Select (CSN) line low */
	PORTB = BIT(MOSI) | BIT(SCK) | BIT(CSN);
}

void InitializeSPI(void)
{
    /* Initialize SPI */
	SPCR = (1<<SPE) | (1<<MSTR);
	SPSR = BIT(SPI2X);
}

void InitializeUART1(BYTE baudrate)
{
	/* Set baud rate */
	UBRR1H = (BYTE)(baudrate>>8);
	UBRR1L = (BYTE)baudrate;

	/* u2x */
	//UCSR1A = (1<<U2X1);

    /* Eight bits per character */
	//UCSR1C = 0x06;

	/* Enable receiver and transmitter */
//	UCSR1A = BIT(U2X1);
	UCSR1B = BIT(RXEN1) | BIT(TXEN1);
	UCSR1C = BIT(UCSZ10) | BIT(UCSZ11);
}
