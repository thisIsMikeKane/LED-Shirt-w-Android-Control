****************
* PROJECT INFO *
****************
This project containes an Android app, written in Java, and embedded C code meant 
for an AVR microcontroller. Together with the hardware described at:
	http://makeprojects.com/Project/Android-Controlled-LED-Light-Shirt/1586/1
the user should be able to create a string of LEDs with dynamic RGB light patterns
controlled by an Android phone over a Bluetooth connection.

*************************
* Source Code Structure *
*************************
The root directory contains two folders 

/AndroidApp/ :: Contains the Eclipse project and source for the Android app. The 
origional project code mainly resides in the java package com.AmishRobot.LedController
 
/Embedded/ :: Contains the source code for the AVR microcontroller used with the
Bluetooth module to control the string of LEDs. The code was ment to be compiled
with Image Craft ICC AVR compiler https://www.imagecraft.com/ and programmed 
with an in system programer (ISP).

****************
* LICENSE INFO *
****************
This project was created by Michael Kane who maintains the copyright for origional
source code and has licensed it as discribed throughout the project. If you wish to 
redistribute or profit from this project I kindly request that you inform me via
email at thisIsMikeKane@gmail.com

The software in this project, licensed under GPLv3 uses code copyrighted by 
Google and Yuku Sugianto both licensed under the GPLv3 compatible ApacheV2 license.
For details see the gpl.txt and the header of each source file.

The hardware documentation for this project is documented at:
	http://makeprojects.com/Project/Android-Controlled-LED-Light-Shirt/1586/1
and is licensed under the Createive Commons CC BY-NC-SA license.
	http://creativecommons.org/licenses/by-nc-sa/3.0/legalcode

**********	
* ENJOY! *
**********