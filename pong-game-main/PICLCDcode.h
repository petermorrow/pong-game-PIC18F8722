//*****************************************************************
// PICDEM HPC Explorer 18 LCD function
// Filename : pic18lcd.h
//
// By Simon Cotton
// 
// 2011-11-25
//*****************************************************************
#ifndef __PIC18LCD_H
#define __PIC18LCD_H
 
#include <p18F8722.h>
#include <delays.h>
 
 #define        LCD_CS				(LATAbits.LATA2)                //LCD chip select
 #define        LCD_CS_TRIS         (TRISAbits.TRISA2)      		//LCD chip select
 #define        LCD_RST             (LATFbits.LATF6)                //LCD chip select
 #define        LCD_RST_TRIS        (TRISFbits.TRISF6)      //LCD chip select
 
 #define LCD_TXSTA_TRMT         	(TXSTAbits.TRMT)
 #define LCD_SPI_IF                 (PIR1bits.SSPIF)
 #define LCD_SCK_TRIS               (TRISCbits.TRISC3)
 #define LCD_SDO_TRIS               (TRISCbits.TRISC5)
 #define LCD_SSPBUF                 (SSPBUF)
 #define LCD_SPICON1                (SSP1CON1)
 #define LCD_SPICON1bits            (SSP1CON1bits)
 #define LCD_SPICON2               	(SSP1CON2)
 #define LCD_SPISTAT                (SSP1STAT)
 #define LCD_SPISTATbits            (SSP1STATbits)

 //extern void Delay(void);
 //extern void SDelay(void);
 
 #pragma code
 
 //*****************************************************************
 // LCD busy delay
 //*****************************************************************
 void LCDBusy(void);
 //*****************************************************************
 // Write to MCP923S17 Port A
 //*****************************************************************
 void WritePortA(char b);
 //*****************************************************************
 // Write to MCP923S17 Port B
 //*****************************************************************
 void WritePortB(char b);
 //*****************************************************************
 // Write the data to the display
 //*****************************************************************
 void d_write(char b);
 //*****************************************************************
 // Write the line to the display
 //*****************************************************************
 void d_write_line(char* b);
 //*****************************************************************
 // Write the line to the display with delay between characters
 //*****************************************************************
 void d_write_line_delay(char* b, unsigned char d);
 //*****************************************************************
 // Send a instruction to the display
 //*****************************************************************
 void i_write(char b);
 //*****************************************************************
 // Write to line 1 of the display
 //*****************************************************************
 void LCDLine_1(void);
 //*****************************************************************
 // Write to line 1 of the display
 //*****************************************************************
 void LCDLine_2(void);
 //*****************************************************************
 // To clear the display
 //*****************************************************************
 void LCDClear(void);
 //******************************************************************
 // Function to write to the PORT
 //******************************************************************
 void InitWrite(char b);
 //*****************************************************************
 // Initialize MCP923S17 Port A
 //*****************************************************************
 void InitPortA_SPI(char b);
 //*****************************************************************
 // Initialize MCP923S17 Port B
 //*****************************************************************
 void InitPortB_SPI(char b);
 //*****************************************************************
 // Initialize MCP923S17 SPI
 //*****************************************************************
 void InitSPI(void);
 //******************************************************************
 // LCD Initialization function
 //******************************************************************
 void LCDInit(void);


