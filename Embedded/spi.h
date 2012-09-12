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

#ifndef __spi_h
#define __spi_h

/* SPI Functions */
unsigned char SPI_MasterTransceive(unsigned char cData);

/* Macros */
#define SPI_Enable()   	(P1OUT &= ~BIT(CSN))
#define SPI_Disable()   (P1OUT |= BIT(CSN))
#define SPI_Wait() 		while (!(IFG2 & UCB0TXIFG))

#define SPI_TX(dbyte) \
	UCB0TXBUF = dbyte;\
	SPI_Wait();

#define SPI_RX(readbyte) \
	do{\
	UCB0TXBUF = 0;\
	SPI_Wait();\
	readbyte = UCB0RXBUF;\
	}while (0)

#define SPI_RX_WORD(readword) \
	do {\
	UCB0TXBUF = 0;\
	SPI_Wait();\
	readword = UCB0RXBUF << 8;\
	UCB0TXBUF = 0;\
	SPI_Wait();\
	readword |= UCB0RXBUF;\
	}while(0)

#define SPI_RX_GARBAGE() \
	do{\
	UCB0TXBUF = 0;\
	SPI_Wait();\
	}while (0)

#endif

