/*
 *  main.c
 *  Handle UART Commands
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

#include "Serial.h"
#include "definitions.h"

/* Global variables for UART data transfer */
unsigned char USART_TX_FIFO[256], USART_RX_FIFO[256], USART_RX_Received, USART_RX_Length;

/* Private function prototypes */
void USART_Flush(void);
unsigned char ReceiveFromUART1(void);
void SerialAcknowledge(void);

void USART_Transmit(unsigned char data)
{
	while (!(IFG2 & UCA0TXIFG));                 // USCI_A0 TX buffer ready?
	UCA0TXBUF = data;
}

/* Transmits the bytes in the USART software FIFO */
void USART_FIFO_Send(unsigned char USART_size)
{
    unsigned char counter;

    /* Write vector of USART Values to USART */
    for(counter=0;counter<USART_size;counter++)
    {
        USART_Transmit(USART_TX_FIFO[counter]);
    }
}

unsigned char ReceiveFromUART1(void)
{
    unsigned char returnbyte;

	returnbyte = UCA0RXBUF;

	return returnbyte;
}

void SerialAcknowledge(void)
{
    unsigned short i, checksum = 0;

    for(i=USART_RX_Received-USART_RX_Length;i<USART_RX_Received;i++)
    {
        checksum += USART_RX_FIFO[i];
    }

    memcpy(&USART_TX_FIFO[0], " >>> ",5);
    memcpy(&USART_TX_FIFO[5], &checksum,2);
    memcpy(&USART_TX_FIFO[7], " ACK ",5);
    USART_TX_FIFO[8] = 18;
    memcpy(&USART_TX_FIFO[13]," !!! ",5);
    USART_FIFO_Send(18);
}

void ReceiveFromSerial(void)
{
    uint8_t server_cmd;
    uint8_t GIE_was_true;

//    UDR1 = UDR1;

//    Stop_Animation();
//    clearDisplay();
//    Animate_Strobe(true, RGB_2_BYTES(15,0,15), 48);

	/* Store Existing Interrupt State */
    GIE_was_true = SR & BIT(GIE);
	Disable_GLOBAL_INT();

    /* Copy incoming USART bytes to FIFO */

    while((UCSR1A & (1<<RXC1)) && (USART_RX_Received < 12 || memcmp(&USART_RX_FIFO[USART_RX_Received-5]," !!! ",5)))
    {
        USART_RX_FIFO[USART_RX_Received] = ReceiveFromUART1();
        USART_RX_Received++;
    }

    /* If we haven't received an end of frame, return program functionality */
    if(USART_RX_Received < 12 || memcmp(&USART_RX_FIFO[USART_RX_Received-5]," !!! ",5))
    {
        /* Enable interrupts if necessary */
        if (GIE_was_true)
        {
        	Enable_GLOBAL_INT();
        }

        return;
    }
    else
    {
        /* Check to make sure that packet is legitimate and that buffer has not overflowed */
        USART_RX_Length = USART_RX_FIFO[USART_RX_Received-6];
        if(USART_RX_Length <= USART_RX_Received && memcmp(&USART_RX_FIFO[USART_RX_Received-USART_RX_Length]," >>> ",5))
        {
        	/* Enable interrupts if necessary */
			if (GIE_was_true)
			{
				Enable_GLOBAL_INT();
			}

            return;
        }
        else if(USART_RX_Length > USART_RX_Received)
        {
            /* Flush USART RX FIFO */
            USART_RX_Received = 0;

        	/* Enable interrupts if necessary */
			if (GIE_was_true)
			{
				Enable_GLOBAL_INT();
			}

            return;
        }

	}

    /* Save the command sent by the server */
	server_cmd = USART_RX_FIFO[USART_RX_Received-USART_RX_Length+5];

    /* Process packet */
    switch(server_cmd)
    {
        case SET_ALL_RGB:

            Stop_Animation();

            SetAllPixels(RGB_2_BYTES(USART_RX_FIFO[USART_RX_Received-USART_RX_Length+6],
                                     USART_RX_FIFO[USART_RX_Received-USART_RX_Length+7],
                                     USART_RX_FIFO[USART_RX_Received-USART_RX_Length+8] ));

            updateDisplay();
            break;

        case ANI_STROBE:

            break;

        case ANI_FADE:

            break;

        case ANI_RAINBOW:

            Animate_Rainbow(true,
                USART_RX_FIFO[USART_RX_Received-USART_RX_Length+6],
                (float)USART_RX_FIFO[USART_RX_Received-USART_RX_Length+7] / 255.0);

//            Animate_Rainbow(true, 4, 1.0);
            break;

        case ANI_RAIN_CHASE:
            Animate_RainbowChaser(true,
                USART_RX_FIFO[USART_RX_Received-USART_RX_Length+6],
                (float)USART_RX_FIFO[USART_RX_Received-USART_RX_Length+7] / 255.0,
                USART_RX_FIFO[USART_RX_Received-USART_RX_Length+8]);
            break;

        case ANI_BLOB:
//            Animate_Blob(true,
//                (((uint16_t)USART_RX_FIFO[USART_RX_Received-USART_RX_Length+6] << 8) +
//                    (uint16_t)USART_RX_FIFO[USART_RX_Received-USART_RX_Length+7]),
//                USART_RX_FIFO[USART_RX_Received-USART_RX_Length+8],
//                RGB_2_BYTES(
//                    USART_RX_FIFO[USART_RX_Received-USART_RX_Length+9],
//                    USART_RX_FIFO[USART_RX_Received-USART_RX_Length+10],
//                    USART_RX_FIFO[USART_RX_Received-USART_RX_Length+11]));

            Animate_Blob(true,
                (((uint16_t)USART_RX_FIFO[USART_RX_Received-USART_RX_Length+6] << 8) +
                    (uint16_t)USART_RX_FIFO[USART_RX_Received-USART_RX_Length+7]),
                USART_RX_FIFO[USART_RX_Received-USART_RX_Length+8],
                RGB_2_BYTES(
                    USART_RX_FIFO[USART_RX_Received-USART_RX_Length+9],
                    USART_RX_FIFO[USART_RX_Received-USART_RX_Length+10],
                    USART_RX_FIFO[USART_RX_Received-USART_RX_Length+11]));

//            Animate_Blob(true, 1200, 2,
//                RGB_2_BYTES(
//                    USART_RX_FIFO[USART_RX_Received-USART_RX_Length+9],
//                    USART_RX_FIFO[USART_RX_Received-USART_RX_Length+10],
//                    USART_RX_FIFO[USART_RX_Received-USART_RX_Length+11]));
            break;
//
//        case ANI_BLOB_FLASH:
//
//            break;
    }

    /* Flush USART RX FIFO */
    USART_RX_Received = 0;

	/* Enable interrupts if necessary */
	if (GIE_was_true)
	{
		Enable_GLOBAL_INT();
	}
}
