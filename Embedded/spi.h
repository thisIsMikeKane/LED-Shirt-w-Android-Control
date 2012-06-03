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
#define ENABLE_SPI_STC_INT()    do { SPCR |= BM(SPIE); } while (0)
#define DISABLE_SPI_STC_INT()   do { SPCR &= ~BM(SPIE); } while (0)

#define SPI_Enable()   (PORTB &= ~BIT(CSN))
#define SPI_Disable()   (PORTB |= BIT(CSN))
#define SPI_Wait() while(!(SPSR & (1<<SPIF)))

#define SPI_TX(dbyte) \
	SPDR = dbyte;\
	SPI_Wait();

#define SPI_TX_Addr(addr)\
	SPDR = addr;\
	SPI_Wait();


#define SPI_RX_Addr(addr) \
	do{\
	SPDR = (addr)|0x40;\
	SPI_Wait();\
	}while (0)

#define SPI_RX(readbyte) \
	do{\
	SPDR = 0;\
	SPI_Wait();\
	readbyte = SPDR;\
	}while (0)

#define SPI_RX_WORD(readword) \
	do {\
	SPDR = 0;\
	SPI_Wait();\
	readword = SPDR << 8;\
	SPDR = 0;\
	SPI_Wait();\
	readword |= SPDR;\
	}while(0)

#define SPI_RX_GARBAGE() \
	do{\
	SPDR = 0;\
	SPI_Wait();\
	}while (0)

#endif

