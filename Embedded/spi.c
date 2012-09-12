/*
 *  spi.c
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

#include <string.h>
#include  <msp430g2553.h>
#include "definitions.h"

/* Transmit cData out through SPI, and receive the SPI input data. */
unsigned char SPI_MasterTransceive(unsigned char cData)
{
	UCB0TXBUF = cData;                     // Send next value
	SPI_Wait();              			   // USCI_A0 TX buffer ready?
	return UCA0RXBUF;
}
