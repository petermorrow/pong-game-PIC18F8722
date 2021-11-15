// FILE: An atttempt at a finalised Mini - Project. 
// AUTH: Peter Morrow
// DATE: 12/04/2018
// DESC: This variation includeds code from all previous attempts, with the inclusion of Code for an LCD Display
// which: Should allow the game to display words on the LCD, and attempt to include a difficulty setting to the game
// NOTE: Tested in MPLAB IDE and on a PIC18F8722

#include <p18f8722.h>			// Include header file for PIC18F microcontroller 
#include <delays.h>				// Include header file for delay subroutine
#include "PICLCDcode.h"			// Include header file for basic LCD code (allows LCD to be programmed) 
#include "Messages.h"			// Include header file for desired messages to be displayed
				
 
#pragma config OSC = HSPLL 		// Oscillator selection: HS with PLL enabled
#pragma config FCMEN = OFF 		// Fail-safe clock monitor disabled
#pragma config IESO = OFF 		// Internal/External Oscillator switchover disabled
#pragma config PWRT = OFF 		// Power-up Timer disabled
#pragma config BOREN = OFF 		// Brown out reset disabled
#pragma config WDT = OFF 		// Watchdog timer Off
#pragma config MCLRE = ON 		// MCLR pin enabled, RE3 input pin disabled
#pragma config LVP = OFF 		// Single supply ICSP disabled
#pragma config XINST = OFF 		// Instruction set extension and indexed addressing
								// mode disabled (legacy mode)


void Direction_Right(void);		// Function prototype for LED moving from left to right (Player 1's serve)
void Direction_Left(void);		// Function prototype for LED moving from right to left (Player 2's serve)
void Player_1_Wins(void);		// Function prototype for when player 1 wins
void Player_2_Wins(void);		// Function prototype for when player 2 wins
void My_ISR(void);				// Function prototype for our ISR

int Delay_time; 				// variable for delay time
								// this is written outside of any function in order to make this variable golbal
								// It can be used by any function, and its designated value will be remembered


void main(void){ 

	unsigned char Player_1_Winner = 0; 		// Player 1 is the winner (when this is high = 1)
	unsigned char Player_2_Winner = 0;		// Player 2 is the winner (when this is high = 1)
	unsigned char i;
	unsigned char Game_Difficulty;	   		//	variable to set game difficulty

	//Setting up LCD

	unsigned char d = 200;					// delay between character display
	unsigned char b = 0x31;

	LCDInit();
	LCDClear();								// Clears any text on LCD
	LCDLine_1();							// Begin writing on line 1 of LCD
	d_write_line_delay(Welcome, 50);		//Presents words 'WELCOME TO PONG' on LED screen 

	Delay10KTCYx(200);

	LCDInit();
	LCDClear();								// Clears any text on LCD
	LCDLine_1();							// Begin writing on line 1 of LCD
	d_write_line_delay(Diff, 50); 			// Presents words 'SELECT DIFF' on LED screen 

	LCDLine_2();							// Begin writing on line 2 of LCD
	d_write_line_delay(Button, 50); 		// Presents words 'RB0=EASYRA5=HARD' on LED screen 



	//*************************This section allows user to select a difficulty setting****************************
	// An easy setting, sets all delay times longer. Hard will set delay times shorter.

	INTCONbits.GIE = 0; 					// Dis-able all interrupts globally
	for(i = 0; i < 20; i++){				// For loop gives user a set time to choose a setting
											// After this time the loop will end, and the game can begin 

		if (PORTBbits.RB0 == 0) Game_Difficulty = 0; 				// Player 1 can set Difficulty to easy
		if (PORTBbits.RB2 == 0) Game_Difficulty = 1;				// Player 2 can set Difficulty to hard
			
		if (Game_Difficulty == 0) Delay_time = 500;					// Easy setting = longer delay time
		if (Game_Difficulty == 1) Delay_time = 100;					// Hard setting = shorter delay time
		Delay10KTCYx(200);		
	} //End for loop

	if (Game_Difficulty == 0) {	LCDInit();							// If easy setting selected
								LCDClear();							// Clears any text on LCD
								LCDLine_1();						// Begin writing on line 1 of LCD
								d_write_line_delay(easy, 50);		// Presents words 'EASY SETTING' on LED screen 
								Delay_time = 500;

						}
	if (Game_Difficulty == 1) {	LCDInit();							// If hard setting selected
								LCDClear();							// Clears any text on LCD
								LCDLine_1();						// Begin writing on line 1 of LCD
								d_write_line_delay(hard, 50); 		// Presents words 'HARD SETTING' on LED screen 
								Delay_time = 100;

						}
	Delay10KTCYx(200);

	//***********************************************************************************************************


	//Set up the respective PORTS and BITs
	TRISD = 0x00;			//Set up PORTD as an output 
	TRISBbits.TRISB0 = 1;	//set up Bit 0 of PORTB as an input (Left button)
	PORTAbits.RA5 = 1;		//Set up Bit 5 of PORTA as an input (Right button)

	// Set up RA5 as a digital input
	ADCON1bits.PCFG0=1;
    ADCON1bits.PCFG1=1;
    ADCON1bits.PCFG2=1;
    ADCON1bits.PCFG3=1;


	//Set up interrupts
	INTCONbits.INT0IF = 0;		// Clear RB0 interrupt flag
	INTCONbits.INT0IE = 1;		// Enable RB0 as interrupt source
	INTCON2bits.INTEDG0 = 0;	// Make RB0 negative edge - remember button press high to low

	INTCON3bits.INT2IF = 0;		// Clear RB2 interrupt flag
	INTCON3bits.INT2IE = 1;		// Enable RB2 as interrupt source
	INTCON2bits.INTEDG2 = 0;	// Make RB2 negative edge - remember button press high to low	
	//We will hard wired RA5 to RB2, in order to allow RA5 (Player 2) to trigger an interrupt 

	INTCONbits.GIE = 1; 		// Enable all interrupts globally
	PORTD = 0;					// Set PORTD to zero (Turn off all LEDs)

		while (1) {				// While loop will keep looping forever, unless interrupted, or game is reset or stopped

					if (PORTD == 128) Player_2_Winner = 1; 					// If LED closest to Player 1 is still on
																			// Player 2 should win (appropriate variable = high)
					if (PORTD == 1) Player_1_Winner = 1; 					// If LED closest to Player 2 is still on
																			// Player 2 should win (appropriate variable = high)

					if (Player_2_Winner == 1){	Player_2_Wins();			// Player 2 wins

												Delay10KTCYx(200);			// Delay 
												Player_2_Winner = 0;		// Set variable low 
												Player_1_Winner = 0;		// Set variable low 
						} // End if statement
					if (Player_1_Winner == 1){	Player_1_Wins(); 			// Player 1 wins
												Delay10KTCYx(200);			// Delay 
												Player_1_Winner = 0;		// Set variable low 
												Player_2_Winner = 0;		// Set variable low 
						} // End if statement								
		} //End while				
} // End main				







//****************************************Direction and Player Functions************************************


void Direction_Right(){
	unsigned char i;
 	
	PORTD = 128;					// Turns on LED closest to Player 1
	Delay10KTCYx(Delay_time);		// Delay for specified time
 
	for(i = 0; i < 7; i++){										// Loop for 7 times 
							if (PORTBbits.RB2 == 0){			// If RB2 (Button RA5) is pressed while the 
																// Funtion Direction_Right is taking place
								
								Player_1_Wins();				// Player 1 will automatically win
								Delay10KTCYx(Delay_time);
									_asm						
										GOTO 0x00				// Three lines of assembly code tells
									_endasm						// the PIC18 to 'go to' the begining of the program
																// This will break out of the interrupt and 
																// re-start the game 
								} // End if statement 

							PORTD = PORTD >> 1;					// Decrement PORTD by a vaule of one 
																// This will 'move' the illuminated LED across one
																// position to the right
							Delay10KTCYx(Delay_time);			// Delay for specified time
							} // End for loop
	} // End Direction_Right




void Direction_Left(){
	unsigned char i;

	PORTD = 1;						// Turns on LED closest to Player 2
	Delay10KTCYx(Delay_time);		// Delay for specified time
						 
	for(i = 0; i < 7; i++){										// Loop for 7 times 
							if (PORTBbits.RB0 == 0){			// If RB0 (Player 1s button) is pressed while the 
																// Funtion Direction_left is taking place

								Player_2_Wins();				// Player 1 will automatically win
								Delay10KTCYx(Delay_time);
									_asm
										GOTO 0x00				// Three lines of assembly code tells
									_endasm						// the PIC18 to 'go to' the begining of the program
																// This will break out of the interrupt and 
																// re-start the game 

								} // End if statement 

							PORTD = PORTD << 1;					// Increment PORTD by a vaule of one 
																// This will 'move' the illuminated LED across one
																// position to the left
							Delay10KTCYx(Delay_time);			// Delay for specified time
							} // End for loop
		

						} // End Direction_Left




void Player_1_Wins(){
	unsigned char i;
						for(i = 0; i < 5; i++){					// Loop for 5 times 
							PORTD = 128;						// Turn on LED closest to Player 1 
							Delay10KTCYx(200);					// Delay 
							PORTD = 0;							// Turn off LED closest to Player 1 
							Delay10KTCYx(200);					// Delay 
						} // End for loop
} // End Player_1_Wins



void Player_2_Wins(){
	unsigned char i;
						for(i = 0; i < 5; i++){					// Loop for 5 times 
							PORTD = 1;							// Turn on LED closest to Player 2 
							Delay10KTCYx(200);					// Delay 
							PORTD = 0;							// Turn off LED closest to Player 2 
							Delay10KTCYx(200);					// Delay 
						} // End for loop
} // End Player_2_Wins



//************************************************ ISR Function**************************************************




#pragma code HighPriority_Interrupt=0x08
void HighPriority_Interrupt(void){
	
	_asm							// Three lines of assembly code tells
		GOTO My_ISR					// the PIC18 to 'go to' the Interrupt Service Routine 
	_endasm

}

#pragma interrupt My_ISR
void My_ISR(void){
	



		if(INTCONbits.INT0IF == 1){
		// If RB0 (Button 1) has caused interrupt
		INTCONbits.INT0IF = 0;		// Clear RB0 interrupt flag

		Direction_Right();			//Run code within Direction_Right function 			
		} // End if statement

		if(INTCON3bits.INT2IF == 1){
		// If RB2 (Which is hard wired to RA5) has caused interrupt
		INTCON3bits.INT2IF = 0;		//Clear RB1 interrupt flag
	
		Direction_Left();			//Run code within Direction_Right function 		
		}// End if statement
} //End My_ISR

