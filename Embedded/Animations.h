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
#ifndef ANIMATIONS_H_INCLUDED
#define ANIMATIONS_H_INCLUDED

/* Defines */
#define     SET_ALL_RGB     1
#define     ANI_STROBE      2
#define     ANI_FADE        3
#define     ANI_RAINBOW     4
#define     ANI_RAIN_CHASE  5
#define     ANI_BLOB        6
#define     ANI_BLOB_FLASH  7

/* Function prototypes */
void Animate_Display_ISR(void);
void Start_Animation(void);
void Stop_Animation(void);

void Animate_Strobe(BOOL restart, uint16_t cVal, uint8_t rate);
void Animate_Fade(BOOL restart, uint16_t cVal, uint8_t rate);
void Animate_Rainbow(BOOL restart, uint8_t i_rate, float i_brightness);
void Animate_RainbowChaser(BOOL restart, uint8_t i_rate, float i_brightness, uint8_t i_step);
void Animate_Blob(BOOL restart, uint16_t i_prob, uint8_t i_rate, uint16_t i_cVal);


#endif // ANIMATIONS_H_INCLUDED
