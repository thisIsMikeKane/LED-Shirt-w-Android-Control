/*
 *  Animations.c
 *  Functions that specify frames for different animations
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

/* Global variables */
extern BOOL        cValIsDirty;
extern uint16_t    cValOfLED[N_LED];

uint16_t    frame;
uint8_t     curAnimation;

/** Function to run to animate the LED display at a given refresh rate */
void Animate_Display_ISR(void)
{
    switch(curAnimation)
    {
        case ANI_STROBE:
        {
            Animate_Strobe(0,0,0);
        }
        break;

        case ANI_FADE:
        {
            Animate_Fade(0,0,0);
        }
        break;

        case ANI_RAINBOW:
        {
            Animate_Rainbow(0,0,0);
        }
        break;

        case ANI_RAIN_CHASE:
        {
            Animate_RainbowChaser(0,0,0,0);
        }
        break;

        case ANI_BLOB:
        {
            Animate_Blob(0,0,0,0);
        }
    }
}

/** Start the Animation ISR to fire at 24 frames per second */
void Start_Animation(void)
{
    /* Initialize Animation Interrupt
     * =============================
     * Timer3/CounterA.
     * CTC, Clear Timer on Compare Match Mode.
     * TOP value = OCR3A.
     * Prescaler = 1024, so clkT3 = 8MHz / 1024 = 7.813KHz = 128us
     * OCR3A = Each frame */
    TCCR3A  = 0;
    TCCR3B  = 0;
    TCNT3   = 0x0000;
    ETIFR  |= BIT(OCF3A);

//    OCR3A = 130; // 0.01664s per frame ~ 60 frames per second
    OCR3A = 260; // ~30 frames per second

    /* Start Timer */
    ETIMSK |=  BIT(OCIE3A);
    TCCR3B = BIT(WGM32) | BIT(CS32) | BIT(CS30);
}

/** Stop the Animation ISR */
void Stop_Animation(void)
{
    ETIMSK &= ~BIT(OCIE3A);
    TCCR3A = 0;
    TCCR3B = 0;
}

/** Strobe all the LEDs a single color on and off at a given rate
/ @param    <restart> reset the cVal and rate parameters and start the animation
/ @param    <cVal> Color value to set all LEDs to (valid only with restart==true)
/ @param    <rate> The number of frames, at 30 frames/sec, to keep the LEDs on (valid only with restart==true)
/
/ @note     Set all params to 0 during normal operation
*/
void Animate_Strobe(BOOL restart, uint16_t cVal, uint8_t rate)
{
    static uint16_t m_cVal;
    static uint8_t m_rate;

    if(restart)
    {
        Stop_Animation();

        curAnimation = ANI_STROBE;
        m_cVal = cVal;
        m_rate = rate;
        frame = 0;

        Start_Animation();
    }

    if(frame < m_rate)
    {
        SetAllPixels(m_cVal);
    }
    else
    {
        SetAllPixels(0x0000);
    }

    if(frame < m_rate*2)
    {
        frame++;
    }
    else
    {
        frame = 0;
    }
}

/** Fade a single color on all LEDs
/ @param    <restart> reset the cVal and rate parameters and start the animation
/ @param    <cVal> The color to fade on and off
/ @param    <rate> The number of frames to hold each color level
/
/ @note     Set all params to 0 during normal operation
*/
void Animate_Fade(BOOL restart, uint16_t i_cVal, uint8_t i_rate)
{
    static uint16_t cVal;
    static uint8_t rate, rVal, gVal, bVal;

    if(restart)
    {
        Stop_Animation();

        curAnimation = ANI_FADE;
        cVal = i_cVal;
        rate = i_rate;
        frame = 0;

        Start_Animation();
    }

    cVal2RGB(cVal, &rVal, &gVal, &bVal);

    if(frame < 32*rate)
    {
        // Fade in
        rVal = (uint8_t)(((uint16_t)rVal * (frame/(uint16_t)rate)) / 31);
        gVal = (uint8_t)(((uint16_t)gVal * (frame/(uint16_t)rate)) / 31);
        bVal = (uint8_t)(((uint16_t)bVal * (frame/(uint16_t)rate)) / 31);
    }
    else
    {
        // Fade out
        rVal = (uint8_t)(((uint16_t)rVal * ((64*(uint16_t)rate-frame-1)/(uint16_t)rate)) / 31);
        gVal = (uint8_t)(((uint16_t)gVal * ((64*(uint16_t)rate-frame-1)/(uint16_t)rate)) / 31);
        bVal = (uint8_t)(((uint16_t)bVal * ((64*(uint16_t)rate-frame-1)/(uint16_t)rate)) / 31);
    }

    SetAllPixels(RGB_2_BYTES(rVal,gVal,bVal));

    if(frame > 32*rate*2-2)
    {
        frame = 0;
    }
    else
    {
        frame++;
    }
}

/** Cycle the a rainbow through all the LEDs at once
/ @param    <restart> reset al the parameters and start the animation
/ @param    <i_rate> The number of frames to hold each color level
/ @param    <i_brightness> Brightness value between 0.0-1.0
/
/ @note     Set all params to 0 during normal operation
*/
void Animate_Rainbow(BOOL restart, uint8_t i_rate, float i_brightness)
{
    static uint16_t rate;
    static float brightness;

    float rFloat, gFloat, bFloat;

    if(restart)
    {
        Stop_Animation();

        if(curAnimation != ANI_RAINBOW)
        {
            frame = 0;
        }

        curAnimation = ANI_RAINBOW;
        rate = (i_rate+1)*10;
        brightness = i_brightness;

        Start_Animation();
    }

    HSVtoRGB(&rFloat, &gFloat, &bFloat, (float)frame / (float)rate * 360.0, 1.0, brightness );

    SetAllPixels(RGB_2_BYTES((uint8_t)(31.0*rFloat), (uint8_t)(31.0*gFloat), (uint8_t)(31.0*bFloat)));

    if(frame<rate-1)
    {
        frame++;
    }
    else
    {
        frame = 0;
    }
}

/** Fade a rainbow down the line of LEDs
/ @param    <restart> reset all the parameters and start the animation
/ @param    <i_rate> The number of frames to hold each color level
/ @param    <i_brightness> Brightness value between 0.0-1.0
/ @param    <i_step> Number of color levels between each LED (for rate=2, N_LED=10 => i_step = 0-18)
/
/ @note     Set all params to 0 during normal operation
*/
void Animate_RainbowChaser(BOOL restart, uint8_t i_rate, float i_brightness, uint8_t i_step)
{

}


/** Randomly turn LEDs on in a slow fade
/ @param    <restart> reset all the parameters and start the animation
/ @param    <i_prob> The probablility that a blob starts on any given frame (0-65535)
/ @param    <i_rate> The number of frames each color level is held for a blob
/ @param    <i_cVal> The color of blobs (0 = random for each blob)
/
/ @note     Set all params to 0 during normal operation
*/
void Animate_Blob(BOOL restart, uint16_t i_prob, uint8_t i_rate, uint16_t i_cVal)
{
    static uint16_t prob, cVal;
    static uint8_t rate;

    static uint16_t blobLevelOfLED[N_LED];
    static uint16_t currentHueOfLED[N_LED];

    uint8_t rVal, gVal, bVal;
    uint8_t i, tempChar;

    if(restart)
    {
        Stop_Animation();

        curAnimation = ANI_BLOB;
        rate = i_rate;
        prob = i_prob;
        cVal = i_cVal;
        for(i=0;i<N_LED;i++)
        {
            blobLevelOfLED[i] = 0;
            cValOfLED[i] = 0;
        }

        /* Convert cVal to RGB */
        cVal2RGB(cVal, &rVal, &gVal, &bVal);

        Start_Animation();
    }

    /* Configure blob on LEDs */
    for(i=0;i<N_LED;i++)
    {
        /* If not a blob, randomly turn into blob */
        if(blobLevelOfLED[i] == 0)
        {
            cValOfLED[i] = 0;

            tempChar = rand();
            if( (((uint16_t)tempChar<<8) + (uint16_t)rand()) < prob )
            {
                blobLevelOfLED[i] = 1;

                if(cVal == 0)
                {
                    currentHueOfLED[i] = ((uint16_t)rand() << 8) + (uint16_t)rand();
                }
                else
                {
                    currentHueOfLED[i] = cVal;
                }
            }
        }

        /* If it is a blob, fade on then off */
        if(blobLevelOfLED[i] > 0)
        {
            /* get RGB of current LED */
            cVal2RGB(currentHueOfLED[i], &rVal, &gVal, &bVal);

            /* Compute current blob intensity */
            if(blobLevelOfLED[i] < 32*rate)
            {
                // Fade in
                cValOfLED[i] = RGB_2_BYTES(
                    (uint8_t)(((uint16_t)rVal * (blobLevelOfLED[i]/(uint16_t)rate)) / 31),
                    (uint8_t)(((uint16_t)gVal * (blobLevelOfLED[i]/(uint16_t)rate)) / 31),
                    (uint8_t)(((uint16_t)bVal * (blobLevelOfLED[i]/(uint16_t)rate)) / 31));
            }
            else
            {
                // Fade out
                cValOfLED[i] = RGB_2_BYTES(
                    (uint8_t)(((uint16_t)rVal * ((64*(uint16_t)rate-blobLevelOfLED[i]-1)/(uint16_t)rate)) / 31),
                    (uint8_t)(((uint16_t)gVal * ((64*(uint16_t)rate-blobLevelOfLED[i]-1)/(uint16_t)rate)) / 31),
                    (uint8_t)(((uint16_t)bVal * ((64*(uint16_t)rate-blobLevelOfLED[i]-1)/(uint16_t)rate)) / 31));
            }

            /* Increment blob level */
            if(blobLevelOfLED[i] > 32*rate*2-2)
            {
                blobLevelOfLED[i] = 0;
            }
            else
            {
                blobLevelOfLED[i]++;
            }
        }

        /* Since this might take a long time, keep 0s on SPI */
        SPI_TX(0x00);
        SPI_Wait();
    }
}

/** r,g,b values are from 0 to 1
  * h = [0,360], s = [0,1], v = [0,1]
  *		if s == 0, then h = -1 (undefined) */
