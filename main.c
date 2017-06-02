//********************************************************************
//*                    EEE2046F C template                           *
//*==================================================================*
//* WRITTEN BY:    	Dewan Pieterse              		             *
//* DATE CREATED:   17/05/2017                                       *
//* MODIFIED:       Never                                            *
//*==================================================================*
//* PROGRAMMED IN: 	Eclipse Luna Service Release 1 (4.4.1)           *
//* DEV. BOARD:    	UCT STM32 Development Board                      *
//* TARGET:	   		STMicroelectronics STM32F051C6                   *
//*==================================================================*
//* DESCRIPTION:    C program for a rain sensor using a tipping      *
//*                 bucket. Buttons 0,1,2 are used to initialise	 *
//*					the weather station. Button 1 increments the	 *
//					bucket tips and 2 displays the total rainfall	 *
//					counted. Increments are also displayed on LEDs.  *
//********************************************************************
// INCLUDE FILES
//====================================================================
#include  "lcd_stm32f0.h"
#include  "stm32f0xx.h"
#include  <stdio.h>
//====================================================================
// SYMBOLIC CONSTANTS
//====================================================================
#define SW0	GPIO_IDR_0
#define SW1	GPIO_IDR_1
#define SW2	GPIO_IDR_2
#define SW3	GPIO_IDR_3

#define D0	GPIO_ODR_0
#define D1	GPIO_ODR_1
#define D2	GPIO_ODR_2
#define D3	GPIO_ODR_3
#define D4	GPIO_ODR_4
#define D5	GPIO_ODR_5
#define D6	GPIO_ODR_6
#define D7	GPIO_ODR_7

#define DELAY1	900
#define DELAY2	900
//====================================================================
// GLOBAL VARIABLES
//====================================================================
unsigned char isStarted = 0;
unsigned char count = 0;			/*declare the count variable and
									  initialising it to 0          */
unsigned char bcd[16];					//Create an char array
unsigned char *bcdptr;
//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
void initPorts(void);
void Delay(void);
void main(void);
unsigned char * ConverttoBCD(unsigned char count1);

//====================================================================
// MAIN FUNCTION
//====================================================================
void initPorts(void)
{
	//set LEDs to outputs (MODER = 01
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; 		//enable clock for LEDs
	GPIOB->MODER |= GPIO_MODER_MODER0_0; 	//set LED0 to output
	GPIOB->MODER |= GPIO_MODER_MODER1_0; 	//set LED1 to output
	GPIOB->MODER |= GPIO_MODER_MODER2_0; 	//set LED2 to output
	GPIOB->MODER |= GPIO_MODER_MODER3_0; 	//set LED3 to output
	GPIOB->MODER |= GPIO_MODER_MODER4_0; 	//set LED4 to output
	GPIOB->MODER |= GPIO_MODER_MODER5_0; 	//set LED5 to output
	GPIOB->MODER |= GPIO_MODER_MODER6_0; 	//set LED6 to output
	GPIOB->MODER |= GPIO_MODER_MODER7_0; 	//set LED7 to output

	// set buttons to inputs (MODER = 00)
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 		//enable clock for push-buttons
	GPIOA->MODER &= ~GPIO_MODER_MODER0; 	//set SW0 to input
	GPIOA->MODER &= ~GPIO_MODER_MODER1; 	//set SW1 to input
	GPIOA->MODER &= ~GPIO_MODER_MODER2; 	//set SW2 to input
	GPIOA->MODER &= ~GPIO_MODER_MODER3; 	//set SW3 to input

	// enable pull-down resistors (PUPDR = 01)
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0; 	//enable pull up for SW0
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR1_0; 	//enable pull up for SW1
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR2_0; 	//enable pull up for SW2
	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR3_0; 	//enable pull up for SW3
}
void Delay(void)
{
	volatile int i,j;
	for (i = 0; i < DELAY1; i++)
	{for (j = 0; j < DELAY2; j++);}
}

unsigned char * ConverttoBCD(unsigned char countbcd)
{

	bcdptr = bcd;							//Point the pointer to the array address
	unsigned int countbcdx = countbcd * 2;

	unsigned char thousands,hundreds,tens,ones,decimals;

	thousands = ((countbcdx / 10000) + 48);	//Get the thousands value
	countbcdx = countbcdx % 10000;			//Modulus of countbcd to get rid of the thousands
	hundreds = ((countbcdx / 1000) + 48);	//Get the hundreds value
	countbcdx = countbcdx % 1000;			//Modulus of countbcd to get rid of the hundreds
	tens = ((countbcdx / 100) + 48);		//Get the tens value
	countbcdx = countbcdx % 100;			//Modulus of countbcd to get rid of the thousands
	ones = ((countbcdx / 10) + 48);			//Get the ones value
	decimals = ((countbcdx % 10) + 48);		//Get the decimal value

	bcd[0] = thousands;						//Assign the values to the array
	bcd[1] = hundreds;
	bcd[2] = tens;
	bcd[3] = ones;
	bcd[4] = 46;
	bcd[5] = decimals;
	bcd[6] = 32;							//Assign " mm" to array
	bcd[7] = 109;
	bcd[8] = 109;

	return bcdptr;							//Return the array pointer

}
void main (void)
{
	init_LCD();								// Initialise LCD
	initPorts();							// Initialise ports
	lcd_command(CLEAR);						// Clears LCD before printing again
	lcd_putstring("EEE2046F Prac 3B");		// Display string on line 1
	lcd_command(LINE_TWO);					// Move cursor to line 2
	lcd_putstring("Dewan Pieterse");		// Display string on line 2

	for(;;)									// Loop forever
	{
		if ((GPIOA->IDR & SW0) == 0 )		//check if button 0 is pressed
		{
			lcd_command(CLEAR);				// Clear lcd screen
			lcd_putstring("Weather Station");// Write to lcd
			lcd_command(LINE_TWO);			// Go to line 2 of the lcd
			lcd_putstring("Press SW2");		// Write to lcd
			isStarted = 1;					// Variable to keep track of whether button 0 is pressed
			Delay();
		}

		else if (isStarted && ((GPIOA->IDR & SW1) == 0))
		{
			count ++;						//Increments the count value by 1
			GPIOB->ODR = count;				//Displaying count on the LEDs
			lcd_command(CLEAR);				//Clears LCD before printing again
			lcd_putstring("Rain bucket tip");//Prints to LCD
			Delay();						/*Delay to prevent multiple counts
										  	  on one press of the button.*/
		}

		else if (isStarted && ((GPIOA->IDR & SW2) == 0))
		{
			lcd_command(CLEAR);				//Clears LCD before printing again
			lcd_putstring("Rainfall:");		//Prints to LCD
			lcd_command(LINE_TWO);			//go to line 2 of the lcd
			lcd_putstring(ConverttoBCD(count));//prints the returned ASCII string to lcd
			Delay();
		}
	}
}											// End of main

// END OF PROGRAM
