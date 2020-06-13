// ********************************************************************************************
// ********************************************************************************************
// ********************************************************************************************
//			
//			[ NOKIA 5110 - XO Game ]
//
//			Created By	:		Mohammed Ahmed Osman Ahmed
//
//			BN					:		343037
//
// 			Date				:		May 23, 2020
//
//			Time				:		10:26 PM 
//
// ********************************************************************************************
// ********************************************************************************************
// ********************************************************************************************


// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the edX Lab 15
// In order for other students to play your game
// 1) You must leave the hardware configuration as defined
// 2) You must not add/remove any files from the project
// 3) You must add your code only this this C file
// I.e., if you wish to use code from sprite.c or sound.c, move that code in this file
// 4) It must compile with the 32k limit of the free Keil

// April 10, 2014
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURCoordinatePositionE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which DrawXOGameScreen ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which DrawXOGameScreen ~80mA total

#include "tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "Random.h"
#include "TExaS.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Timer2_Init(unsigned long period);
void DelayTime(unsigned long count); // time delay in 0.1 seconds
char Winner_Checker(void);
unsigned long TimerCount;
unsigned long Semaphore;

// My Global Variables
int Coordinate[3][3];
char Winner=0;
int CoordinatePosition, HorizontalStep, VerticalStep;
int Player;
int Completed;

// My Functions Header
int XOGameSplashScreen(void);
void StartXOGame(int);
void DrawXOGameScreen(void);
int XOGameExecution(void);
void DelayTime(unsigned long);
char Winner_Checker(void);

// *************************** Images ***************************




const unsigned char logo [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xE0, 0xF0, 0xF8, 0xF8, 0xFC,
0xFC, 0xFC, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE,
0xFE, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xE0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x7F, 0x7F, 0xFF, 0x7F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x33, 0xFF, 0xFF,
0x87, 0x01, 0x00, 0x38, 0x38, 0x01, 0x01, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC,
0xF8, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xEE, 0xC0, 0xC0, 0xF0, 0xF0, 0xE2, 0xE7, 0x7F, 0x7F, 0x7F, 0x3F, 0x00, 0x00,
0x00, 0x1F, 0x1F, 0x8F, 0x8F, 0x8E, 0xCE, 0xC6, 0xC6, 0xC7, 0xE7, 0xE7, 0xE3, 0xF3, 0xF7, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xE7, 0xE3, 0xF3, 0xF1, 0x79, 0x18, 0x18, 0x9C, 0x9C, 0x1C, 0x3E,
0xFE, 0xFE, 0xF0, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xF7, 0xC7, 0x07, 0x0F, 0x03, 0x23, 0x7F,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x03, 0x0F, 0x1F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xE0, 0xE3, 0xC7,
0xC7, 0xE0, 0xF0, 0xFC, 0xFF, 0xFF, 0xFE, 0xE0, 0x80, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xFC,
0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x0F, 0x0F, 0x1F, 0x1F,
0x3F, 0x3F, 0x3F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F,
0x3F, 0x1F, 0x1F, 0x0F, 0x0F, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 
};

const unsigned char SmallEnemy30PointA[] = {
 0x42, 0x4D, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x80,
 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0xC0, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF,
 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x0F, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
 0xFF, 0x0F, 0xF0, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};



// *************************** Capture image dimensions out of BMP**********

	void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}
void PORTE_INIT(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000010;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTE_LOCK_R = 0x4C4F434B;   // 2) unlock PortE
  GPIO_PORTE_CR_R = 0x03;           // allow changes to PE2       
  GPIO_PORTE_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTE_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R = 0x02;          // 5) PE0 input, PE1 output   
  GPIO_PORTE_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTE_PDR_R= 0x01;          // enable pulldown resistors on PE0       
  GPIO_PORTE_DEN_R = 0x01;          // 7) enable digital pins PE0-PE1
}


// ********************************************************************************************
//
// 			(1) set_Cursor Function
//
// ********************************************************************************************
void set_Cursor() {
	
		int HorizontalCoordinate,VerticalCoordinate;
	
		// If The New CoordinatePositionition Is Empty, 
		if( Coordinate[CoordinatePosition/3][CoordinatePosition%3] == 0 ) {
			
				HorizontalCoordinate=( (CoordinatePosition%3) * HorizontalStep );
				VerticalCoordinate=( (CoordinatePosition/3) * VerticalStep );
				Coordinate[CoordinatePosition/3][CoordinatePosition%3] = '_';
				Nokia5110_SetCursor( HorizontalCoordinate, VerticalCoordinate );
				Nokia5110_OutChar('_');
				Nokia5110_SetCursor( HorizontalCoordinate, VerticalCoordinate );
			
		} 
		
}

// ********************************************************************************************
//
// 			(2) remove_Cursor Function
//
// ********************************************************************************************
void remove_Cursor(){
	
		int HorizontalCoordinate, VerticalCoordinate;
	
		if( Coordinate[CoordinatePosition/3][CoordinatePosition%3] == '_' ) {
			
				HorizontalCoordinate = ( (CoordinatePosition%3) * HorizontalStep );
				VerticalCoordinate = ( (CoordinatePosition/3) * VerticalStep );
				Coordinate[CoordinatePosition/3][CoordinatePosition%3] = 0;
				Nokia5110_SetCursor( HorizontalCoordinate, VerticalCoordinate );
				Nokia5110_OutChar(' ');
			
		}
		
}


// ********************************************************************************************
//
// 			(3) XOGameSplashScreen Function
//
// ********************************************************************************************
int XOGameSplashScreen() {
		char OK;
	
		// GameState 
		int GameState;
		GameState=0;
		
		Nokia5110_SetCursor(0,0);
		Nokia5110_OutString("[ XO Game ]");
		Nokia5110_SetCursor(0,2);
		Nokia5110_OutString("Press Ok");
		Nokia5110_SetCursor(0,3);
		Nokia5110_OutString("To Start ..");
		Nokia5110_SetCursor(0,5);
		Nokia5110_OutString("BN: 343037");
	
		while(1){
				OK = GPIO_PORTE_DATA_R&0x01;     // read PE0 into OK
				if(OK){ //
						GameState=1;
						return GameState;
				}
		}
}


// ********************************************************************************************
//
// 			(4) StartXOGame Function
//
// ********************************************************************************************
void StartXOGame(int a){
	
		char i,j, GameState;
		GameState = a;
	
		//char OK;
		//OK = GPIO_PORTE_DATA_R&0x01;     // read PE0 into OK

		CoordinatePosition=0; 
		Completed=1;

		if(GameState==1) {
				HorizontalStep=2,VerticalStep=1;
		}

		// Variable Stores The Player Type: 0 For X, 1 For O
		Player=0;

		for(i=0;i<3;i++){
				for(j=0;j<3;j++) {
						Coordinate[i][j]=0;
				}
		}

}


// ********************************************************************************************
//
// 			(5) DrawXOGameScreen Function : Sets Nokia5110 Pixels
//
// ********************************************************************************************
void DrawXOGameScreen() {
	
		int i,j;
		int x=28, y=16;
	
		for(i=0;i<3;i++){
			
				for(j=0;j<48;j++)Nokia5110_SetPixel(x*i,j);
				for(j=0;j<84;j++)Nokia5110_SetPixel(j,y*i);
			
		}
		
		Nokia5110_SetCursor(0,5);
		Nokia5110_OutString("[\"X\" Turn]");
}


// ********************************************************************************************
//
// 			(6) XOGameExecution Function
//
// ********************************************************************************************
int XOGameExecution() {

		while(1){
			
				unsigned long NextButton,PreviousButton,OK;
				
				// Read PF4 Into "NextButton" Variable
				NextButton = GPIO_PORTF_DATA_R&0x10;
			
				// Read PF0 Into "PreviousButton" Variable
				PreviousButton = GPIO_PORTF_DATA_R&0x01;
			
				// Read PE0 Into "OK" Variable
				OK = GPIO_PORTE_DATA_R&0x01;
				
				// If NextButton Is Pressed
				if( !(NextButton) ) {
						
						// If The NextButton Is Pressed,
						// The Cursor Will Be Moved To The Next Coordinate
						remove_Cursor();				
						CoordinatePosition++;
					
						// If The Cursor Is At The Last Coordinate And The Player Presses The NextButton,
						// The Cursor Will Be Moved To The First Coordinate  Which Is [0][0]
						if( CoordinatePosition>8 )
								CoordinatePosition = 0;
						
						// Do Not Execute Any Thing Until The Player Release His Finger From The NextButton
						while(!(GPIO_PORTF_DATA_R&0x10));
						
						// Set The Cursor
						set_Cursor();
						
				}
				
				// If PreviousButton Is Pressed
				if( !(PreviousButton) ) {
					
						// If The PreviousButton Is Pressed,
						// The Cursor Will Be Moved To The Previous Coordinate
						remove_Cursor();
						CoordinatePosition--;
						
						// If The Cursor Is At The First Coordinate And The Player Presses The "PreviousButton",
						// The Cursor Will Be Moved To The Last Coordinate Which Is [2][1]
						if( CoordinatePosition<0 )
								CoordinatePosition = 8;
						
						// Do Not Execute Any Thing Until The Player Release His Finger From The "PreviousButton"
						while( !(GPIO_PORTF_DATA_R&0x01) );
						
						// Set The Cursor
						set_Cursor();
						
				}
				
				// If OK Button Is Pressed
				if((OK)){
					
						// Do Not Execute Any Thing Until The Player Release His Finger From The OK Button
						while( GPIO_PORTE_DATA_R&0x01 );
						
						// If Player "X"
						if( Player==0 ) {
							
								if(Coordinate[CoordinatePosition/3][CoordinatePosition%3]=='_') {
									
										// Print "X" In The Current Coordinate Position
										Nokia5110_OutChar('X');
									
										// Then, Store "X" In The Coordinate Array
										Coordinate[CoordinatePosition/3][CoordinatePosition%3]='X';
										
										// Then, Move To The Coordinate Position
										CoordinatePosition++;
										
										// Set "CoordinatePosition" To 8 If It Is Large Than 8
										if(CoordinatePosition>8)
												CoordinatePosition=8;
										
										// After Player "X" Played His Turn, This Message Will Alert The Player "O" To Take His Turn
										Nokia5110_SetCursor(0,5);
										Nokia5110_OutString("[\"O\" Turn]");
										
										// Set The Cursor
										set_Cursor();
										
										// Selecting The "O" Player To Play His Turn
										Player=1;
										
								}
								
						} else { // If Player "O" (When Player==1)
							
									if(Coordinate[CoordinatePosition/3][CoordinatePosition%3]=='_') {
										
											// Print "X" In The Current Coordinate Position
											Nokia5110_OutChar('O');
										
											// Then, Store "X" In The Coordinate Array
											Coordinate[CoordinatePosition/3][CoordinatePosition%3] = 'O';
											
											// Then, Move To The Coordinate Position
											CoordinatePosition++;
										
											// Set "CoordinatePosition" To 8 If It Is Large Than 8
											if(CoordinatePosition>8)
													CoordinatePosition=8;
											
											// After Player "O" Played His Turn, This Message Will Alert The Player "X" To Take His Turn
											Nokia5110_SetCursor(0,5);
											Nokia5110_OutString("[\"X\" Turn]");
											
											// Set The Cursor
											set_Cursor();
											
											// Selecting The "X" Player To Play His Turn
											Player=0;
									}
						}
						
						// Calling The "Winner_Checker" Function In Order To Check Who Is The Winner
						Winner = Winner_Checker();
						
				}
				
				// Printing Some Messages To Nokia 5110 Screen In Case Of The Winner Is Either "X" Or "O"
				if( Winner ) {
					
						// If "X" Wins
						if( Winner=='X' ) {
								if(Completed){
										Nokia5110_Clear();
										Nokia5110_SetCursor(0,0);
										Nokia5110_OutString("[X Wins]");
										Nokia5110_SetCursor(0,1);
										Nokia5110_OutString("Congrats X!");
										Nokia5110_SetCursor(0,3);
										Nokia5110_OutString("Hard Luck");
										Nokia5110_SetCursor(0,4);
										Nokia5110_OutString("For\"O\"");
										Completed=0;
										return 0;
								}
						}
						
						// If "O" Wins
						if( Winner=='O' ) {
								if(Completed){
										Nokia5110_Clear();
										Nokia5110_SetCursor(0,0);
										Nokia5110_OutString("[O Wins]");
										Nokia5110_SetCursor(0,1);
										Nokia5110_OutString("Congrats O!");
										Nokia5110_SetCursor(0,3);
										Nokia5110_OutString("Hard Luck");
										Nokia5110_SetCursor(0,4);
										Nokia5110_OutString("For\"X\"");
										Completed=0;
										return 0;
								}
						}
						
						// If No One Wins
						if( Winner=='D' ) {
								if(Completed){
										Nokia5110_Clear();
										Nokia5110_SetCursor(0,0);
										Nokia5110_OutString("[Draw]");
										Completed=0;
										return 0;
								}
						}
						
				}
					
		}
	
}



// ********************************************************************************************
//
// 			(7) Winner_Checker Function
//
// ********************************************************************************************
char Winner_Checker(void){
	
		int i, j;
	
		int RowXScore=0, RowOScore=0, ColumnXScore=0, ColumnOScore=0, FirstDiagonalXScore=0, FirstDiagonalOScore=0, SecondDiagonalXScore=0, SecondDiagonalOScore=0;
		
		int DrawXOGameScreen = 1; // DrawXOGameScreen
	
		// ------------------------
		// [A] Check For The Winner
		// ------------------------
		for( i=0; i<3; i++ ) {
				
				// Reset The Values For The Next Iteration
				RowXScore=0, RowOScore=0, ColumnXScore=0, ColumnOScore=0;
			
				// ------------------------------
				// [1] Check All Rows And Columns
				// ------------------------------
			
				for(j=0;j<3;j++){
						
						// (I) Check X In Rows
						// X Wins When All Elements On One Row Are 'X'
						// When [0][0] && [0][1] & [0][2] == 'X'
						// OR 	[1][0] && [1][1] & [1][2] == 'X'
						// OR 	[2][0] && [2][1] & [2][2] == 'X'
						if(Coordinate[i][j]=='X')
								RowXScore++;
						
						// (II) Check O In Rows
						// O Wins When All Elements On One Row Are 'O'
						// When [0][0] && [0][1] & [0][2] == 'O'
						// OR 	[1][0] && [1][1] & [1][2] == 'O'
						// OR 	[2][0] && [2][1] & [2][2] == 'O'
						if(Coordinate[i][j]=='O')
								RowOScore++;
						
						// (III) Check X In Columns
						// X Wins When All Elements On One Columns Are 'X'
						// When [0][0] && [1][0] & [2][0] == 'X'
						// OR 	[0][1] && [1][1] & [2][1] == 'X'
						// OR 	[0][2] && [1][2] & [2][1] == 'X'
						if(Coordinate[j][i]=='X')
								ColumnXScore++;
						
						// (IV) Check I In Columns
						// O Wins When All Elements On One Columns Are 'O'
						// When [0][0] && [1][0] & [2][0] == 'O'
						// OR 	[0][1] && [1][1] & [2][1] == 'O'
						// OR 	[0][2] && [1][2] & [2][1] == 'O'
						if(Coordinate[j][i]=='O')
								ColumnOScore++;
						
						// Check If No DrawXOGameScreen
						if(Coordinate[i][j]==0)
									DrawXOGameScreen=0;
				}
				
				// ----------------------------
				// [2] Check The First Diagonal
				// ----------------------------
				
				// (I) X Wins When [0][0] && [1][1] & [2][2] == 'X'
				if(Coordinate[i][i]=='X')
						FirstDiagonalXScore++;
				// (II) O Wins When [0][0] && [1][1] & [2][2] == 'O'
				if(Coordinate[i][i]=='O')
						FirstDiagonalOScore++;
				
				// -----------------------------
				// [3] Check The Second Diagonal
				// -----------------------------
				
				// (I) X Wins When [0][2] && [1][1] & [2][0] == 'X'
				if(Coordinate[i][2-i]=='X')
						SecondDiagonalXScore++;
				// (II) O Wins When [0][2] && [1][1] & [2][0] == 'O'
				if(Coordinate[i][2-i]=='O')
						SecondDiagonalOScore++;
				
				// ---------------------------------------------
				// [4] If One Score Reaches 3, Return The Winner
				// ---------------------------------------------
				if( RowXScore == 3 || ColumnXScore == 3 || FirstDiagonalXScore == 3 || SecondDiagonalXScore == 3 )
						return 'X';
				if( RowOScore == 3 || ColumnOScore == 3 || FirstDiagonalOScore == 3 || SecondDiagonalOScore == 3 )
						return 'O';
			
		}
		
		// ------------------------
		// [B] Check If DrawXOGameScreen
		// ------------------------
		if(DrawXOGameScreen)
				return 'D';
		
		return 0;
		
}

// ********************************************************************************************
//
// 			(8) DelayTime Function
//
// ********************************************************************************************
void DelayTime(unsigned long Val){
	
		unsigned long volatile DelayedTime;
	
		while( Val > 0 ){
			
				DelayedTime = 727240;
			
				while(DelayedTime)
						DelayedTime--;
				
				Val--;
				
		}
		
}


// ********************************************************************************************
//
// 			(9) Main Function
//
// ********************************************************************************************
int main(void){ // Start Main()
	
		// Set System Clock To 80 MHz
		TExaS_Init(SSI0_Real_Nokia5110_Scope);
		Random_Init(1);
	
		// Initialize All Registers And Their Ports
		Nokia5110_Init();
	
		// Clear Buffer
		Nokia5110_ClearBuffer();
	
		// DrawXOGameScreen Buffer
		Nokia5110_DisplayBuffer();
	
		// Initialize Port F
		PortF_Init();
	
		// Initialize Port E
		PORTE_INIT();
	    
		// Nokia5110_PrintBMP(0,0,SmallEnemy30PointA,0);
		// Nokia5110_DisplayBuffer();
		
		// Clear Nokia5110 Screen
		Nokia5110_Clear();
		
		// The Splash Screen Of My XO-Game
		StartXOGame( XOGameSplashScreen() );
		
		// Clear Nokia5110 Screen
		Nokia5110_Clear();
		
		// Delay 1 Second
		DelayTime(1);
		
		// Draw The Screen In Order To Start Playing
		DrawXOGameScreen();
		
		// SET The Cursor At The First Coordinate
		set_Cursor();
		Nokia5110_SetCursor(0,0);
	
		// Call XOGameExecution Function
		XOGameExecution();

} // End Main()
