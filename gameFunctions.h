
#include "Nokia5110.h"
#include "images.h"
#include "AI.h"

#include "UART.h"
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include <stdlib.h>
// *************************** Capture image dimensions out of BMP**********
#define PLAYERH = 6;
#define PLAYERW = 8;
int states[6][7] = {
	{0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0}
};
int Mode = 0;
char mod1 [14] = "Single P",mod2 [19] = "Multi Ps";
void initialize_screen(void);
void updateDisplay(void);
void updateArrow(int x);
void next_game(void);
void update_selection(int);
void Delay100ms(unsigned long count); // time delay in 0.1 seconds
void set_mode (int x) {
 Mode = x;
}	
int  waitPlayUart(){
	char x;
	int x1 = 0;
	x = UART_InChar();
	x1 = x-'0';
	return x1;
}
void playUart(int i){
	char x = i+'0';
	UART_OutChar(x);
}
void printWinning(int winningtemp){
	Delay100ms(40);
	Nokia5110_Clear();
	Nokia5110_SetCursor(2, 3);
	if(winningtemp == 1)
		Nokia5110_OutString("U Won!");
	else if (winningtemp == 2)
    Nokia5110_OutString("U LOSE!"); 
	else if (winningtemp == 3)
    Nokia5110_OutString("TIE");
	exit(0);
}

void toGame() {
	 int i,count=-1,j=-1;
		for( i = 1; i < 43 ; i++)
		{
		  	if(i%7 == 1)
					count++;
				j++;
            if(j==7)
                j=0;
				 if(input[i] == 'X')
					states[count][j] = 2;
				else if(input[i] == 'O')
					states[count][j] = 1;
				else
					states[count][j] = 0;
		}
}
void submitGame(int pos) {//player
	int winningtemp;
  PlayPosition('X',pos);
	if( Mode == 2 )
		playUart(pos);
	toGame();
	updateDisplay();
	winningtemp = winning();
  if(winningtemp != 0)
  {
   printWinning(winningtemp);
  }
}

void updateArrow(int x) {
	int count,rows=5,cols=0;

	for(count=0;count<7;count++,cols+=8) {
		if(count==x)
			Nokia5110_PrintBMP(cols,rows, arrow, 3);
		else
			Nokia5110_PrintBMP(cols,rows, empty, 3);
	}
	Nokia5110_DisplayBuffer();
}
void uartPlay() { // UART
	int O = 11,winningtemp;
	
		O = waitPlayUart();
	PlayPosition('O',O);
	toGame();
	updateDisplay();
	winningtemp = winning();
   if(winningtemp != 0)
    {
			printWinning(winningtemp);
    }		
}
void AIPlay() {// AI 

	int winningtemp;
	input[AIManager()]='O';
	toGame();
	updateDisplay();
	winningtemp = winning();
  if(winningtemp != 0)
    {
			printWinning(winningtemp);
    }		
}
void initialize_screen() {
	int c = 0 ; 
	char welcome[8]="Welcome!";
	Nokia5110_ClearBuffer();
	Nokia5110_DisplayBuffer();
	Nokia5110_SetCursor(3,2);
	for (c=0;c<7;c++) {
			Nokia5110_OutChar(welcome[c]);
			Delay100ms(3);
	}
	Delay100ms(20);
  clean();
}
void updateDisplay() {
		int rows=12,cols,rowsCounter=0,colsCounter=0;
		Nokia5110_ClearBuffer();
		for(rowsCounter=5;rowsCounter>=0;rowsCounter--,rows+=6) {
			cols=0;
			for(colsCounter=0;colsCounter<7;colsCounter++, cols+=8){
						if(states[rowsCounter][colsCounter] == 0)
								Nokia5110_PrintBMP(cols,rows, empty_slot, 3); // empty slot
						
						else if(states[rowsCounter][colsCounter] == 1) 
								Nokia5110_PrintBMP(cols,rows, player1, 3); // empty slot
						
						else if(states[rowsCounter][colsCounter] == 2) 
								Nokia5110_PrintBMP(cols,rows, player2, 3); // empty slot
				}
		}
		Nokia5110_DisplayBuffer();
}
void update_selection(int m) {
	int i ;
	Nokia5110_Clear();
	for ( i = 1; i < 3 ; i++ ){
		Nokia5110_SetCursor(1, i+1);
		if (m == i)
			Nokia5110_OutChar('>');
		else
			Nokia5110_OutChar(' ');
		
		Nokia5110_SetCursor(3, 2);
		Nokia5110_OutString("Single P");
		Nokia5110_SetCursor(3, 3);
		Nokia5110_OutString("Multi P");
	}
}
