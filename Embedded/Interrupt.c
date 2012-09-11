/*
 *  main.c
 *  Specify interrupt functions for specific interrupts
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

/* Interrupt handlers for timer compare interrupts */
#pragma interrupt_handler TIMER3_COMPA_INT:iv_TIMER3_COMPA

/* Interrupt that fires when a byte is ready to be read in from UART */
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    ReceiveFromSerial();
}

/* General interrupt handler for timerA, comp0 interrupt */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    Animate_Display_ISR();
    updateDisplay();
}
