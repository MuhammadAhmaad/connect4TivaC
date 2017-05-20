// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground
#include <time.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "Random.h"
#include "TExaS.h"
#include "gameFunctions.h"
int mode = 0, s = 1, waiting = 1, beginOrWait = 0, postition = 1, co = 0;
unsigned long In, Out;  // input from PF4
void PortF_Init(void);

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x000000FF;     		// 1) F clock
  delay = SYSCTL_RCGC2_R;           		// delay    
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   		// 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;          		// allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        		// 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   		// 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = ~(0x11);          	 	// 5) PF0-PF4 input   
  GPIO_PORTF_AFSEL_R = 0x00;        		// 6) no alternate function
  GPIO_PORTF_PUR_R  = 0x11;          		// enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R  = 0x1F;         		// 7) enable digital pins PF4-PF0  
	GPIO_PORTF_IS_R = 0x00;								// Set the interrupt sense to edge-sensitive
	GPIO_PORTF_IBE_R = 0x00;							// Interrupt controlled by both IEV	
	GPIO_PORTF_IEV_R = 0x00;
	GPIO_PORTF_IM_R = 0x00;								// Listen to on PF0, PF1, PF4
	NVIC_EN0_R = 0x40000000; 
	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000;
	GPIO_PORTF_ICR_R = 0x1F;
}
void startingMulti() {
	int randomNO;
	int ranUART;
	SYSCTL_RCGC2_R &= ~(0x00000020);
	srand(co);   // should only be called once
  randomNO= rand();
	randomNO %=9;
	playUart(randomNO);
  ranUART =	waitPlayUart();
  if( randomNO > ranUART) {
		beginOrWait = 1;
		waiting=0;
		updateDisplay();
		SYSCTL_RCGC2_R |= (0x00000020);
   }
  else{
	  beginOrWait = 2;
   }
}
void playSwitches(int sw1,int sw2){
	if(sw1==0)
	{
		Delay100ms(10);
				if(postition==7)
			postition=1;
		else 
			postition++;
		
		updateArrow(postition-1);
	}
	else if (sw2==0)
	{
		Delay100ms(100);
		submitGame(postition);
    waiting=1;
		postition = 1;
	}
}
void SwitchModes(int sw1,int sw2) {
	if (sw1 == 0) {
		s = (s == 1? 2:1);
		update_selection(s);
		Delay100ms(10);
	}
	if (sw2 == 0) {
		mode = s;
		Delay100ms(10);
	}
}
int main(void){// you cannot use both the Scope and the virtual Nokia (both need UART0)
	TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
	Random_Init(1);
	PortF_Init();
  UART_Init();
	Nokia5110_Init();
  Nokia5110_ClearBuffer();
  initialize_screen();
  Nokia5110_Clear();
	update_selection(1);
	while (mode == 0) {
		In  = GPIO_PORTF_DATA_R&0x10; // read PF4 into In
		Out = GPIO_PORTF_DATA_R&0x01;
		SwitchModes(In,Out);
		co++;
	}
	Nokia5110_Clear();
	if ( mode == 2 )
		startingMulti();
	if ( mode == 1 ){
		beginOrWait = 2;
	}
	set_mode(mode);
	while(1){
		 if(beginOrWait==1){
			 if(waiting==0){
				 SYSCTL_RCGC2_R |= (0x00000020);
				 In = GPIO_PORTF_DATA_R&0x10; // read PF4 into In
		     Out= GPIO_PORTF_DATA_R&0x01;
		     playSwitches(In,Out);
			 }
			 else {
				uartPlay();
				waiting=0;
				SYSCTL_RCGC2_R |= (0x00000020);
			 }
		 }
     else if (beginOrWait==2){
	     if(waiting==1){
				 if ( mode == 1 ) {
						AIPlay();
						updateArrow(0);
				 }
				 else if ( mode == 2 )
				 uartPlay();
				 waiting=0;
				 SYSCTL_RCGC2_R |= (0x00000020);
			 }
			 else{
				  In = GPIO_PORTF_DATA_R&0x10; // read PF4 into In
					Out=GPIO_PORTF_DATA_R&0x01;
				  playSwitches(In,Out);
			 }
     }
	 }
}
void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 72724;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
