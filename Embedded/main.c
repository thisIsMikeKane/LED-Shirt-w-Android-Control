/*
 *  main.c
 *  This project will link a MSP430 with the BlueSMiRF Silver module and a string of i2c driven LEDs
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

extern uint16_t cValOfLED[];

void main(void)
{
    uint16_t i;
    uint8_t j;

    //Stop WDT
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

	/* Disable all interrupts */
	Disable_GLOBAL_INT();

	/* Initialize ports for communication peripherals */
    InitializePorts();

	/* Initialize SPI */
    InitializeSPI();

    /* Initialize serial port connection */
    InitializeUART1();

	/* Initialize LED Display */
	InitializeLED();

    /* Enable interrupts */
    ENABLE_UART1_RX_INT();
	Enable_GLOBAL_INT();

//    Animate_Strobe(true, RGB_2_BYTES(15,0,15), 48);
//    Animate_Fade(true, RGB_2_BYTES(7,31,10), 4);
//    Animate_Rainbow(true, 200, 1.0);
//    Animate_RainbowChaser(true, 60, 1.0, 5);
    Animate_Blob(true, 1200, 2, 0*(RGB_2_BYTES(7,31,10)) );

	/* Wait for serial commands */
	while(1)
	{
        /* Send SPI clock to keep LED PWM running */
        SPI_TX(0x00);
        SPI_Wait();
	};
}
