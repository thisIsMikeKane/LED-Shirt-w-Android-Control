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
#include  <msp430g2553.h>
#include "definitions.h"

void InitializeClk(void)
{
	// Set MCU speed to 1MHz using internal digitally controlled oscillator (DCO)
	BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
	DCOCTL = CALDCO_1MHZ;
}

void InitializePorts(void)
{
    /* Special Function I/O register initialization */
	// See definitions.h for alignment
	P1SEL  = BIT(RXD) | BIT(TXD) | BIT(SCK) | BIT(MOSI) | BIT(MISO);
	P1SEL2 = BIT(RXD) | BIT(TXD) | BIT(SCK) | BIT(MOSI) | BIT(MISO);

	P1DIR  = BIT(CSN) | BIT(LED1) | BIT(LED2);

	/* Pull-ups in I/O ports are disabled */
	P1REN = 0;

	/* Set Chip Select (CSN) line low */
	P1OUT  &= ~BIT(CSN);
}

void InitializeSPI(void)
{
    /* Initialize SPI on USCI-B */
	// Master 4-wire synchronous mode
	UCB0CTL0 = UCMST | UCMODE_1 | UCSYNC;
	// Clock source from SMCLK
	UCB0CTL1 = UCSSEL_2;
	// Baud rate set at 1MHz assuming 1MHz MCU speed
	UCB0BR0 = 0;
	UCB0BR1 = 0;

	// **Initialize USCI state machine**
	UCB0CTL1 &= ~UCSWRST;
}

void InitializeUART1()
{
	/* Initialize UART on USCI-A */
	// Buad rate of 115200 used by the BlueSMiRF
	UCA0CTL1  = 0;
	UCA0CTL1  = UCSSEL_2;                  		// SMCLK
	UCA0BR0   = 8;                              // 1MHz 115200
	UCA0BR1   = 0;                              // 1MHz 115200
	UCA0MCTL  = UCBRS2 + UCBRS0;               	// Modulation UCBRSx = 5
	UCA0CTL1 &= ~UCSWRST;                     	// **Initialize USCI state machine**
	IE2      |= UCA0RXIE;                       // Enable USCI_A0 RX interrupt
}
