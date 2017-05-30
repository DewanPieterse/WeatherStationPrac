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

#define DELAY1	800
#define DELAY2	900
//====================================================================
// GLOBAL VARIABLES
//====================================================================
unsigned char isStarted = 0;
unsigned char count = 0b0;				/*declare the count variable and
										  initialising it to 0*/
//====================================================================
// FUNCTION DECLARATIONS
//====================================================================
void initPorts(void);
void Delay(void);
void main(void);
char * ConverttoBCD(char count);

//====================================================================
// MAIN FUNCTION
//====================================================================
void initPorts(void)
{
	//set LEDs to outputs
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; 		//enable clock for LEDs
	GPIOB->MODER |= GPIO_MODER_MODER0_0; 	//set LED0 to output
	GPIOB->MODER |= GPIO_MODER_MODER1_0; 	//set LED1 to output
	GPIOB->MODER |= GPIO_MODER_MODER2_0; 	//set LED2 to output
	GPIOB->MODER |= GPIO_MODER_MODER3_0; 	//set LED3 to output
	GPIOB->MODER |= GPIO_MODER_MODER4_0; 	//set LED4 to output
	GPIOB->MODER |= GPIO_MODER_MODER5_0; 	//set LED5 to output
	GPIOB->MODER |= GPIO_MODER_MODER6_0; 	//set LED6 to output
	GPIOB->MODER |= GPIO_MODER_MODER7_0; 	//set LED7 to output

	// set buttons to inputs
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 		//enable clock for push-buttons
	GPIOA->MODER &= ~GPIO_MODER_MODER0; 	//set SW0 to input
	GPIOA->MODER &= ~GPIO_MODER_MODER1; 	//set SW1 to input
	GPIOA->MODER &= ~GPIO_MODER_MODER2; 	//set SW2 to input
	GPIOA->MODER &= ~GPIO_MODER_MODER3; 	//set SW3 to input

	// enable pull-down resistors
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

char * ConverttoBCD(char count1)
{
	static char bcd[4];
	char *bcdptr;
	bcdptr = bcd;
	int temp = 0;
	//int shift = 0;
	int i = 4;

	while (count1 > 0)
	{
		temp = (count1 % 10);// << (shift++ << 2);
		bcdptr[i] = temp;
		count1 /= 10;
		i--;
	}
	int j;
	/*for (j = 0; j < 4; j++)
	{
		bcdptr[j] += 0x30;
	}*/

	return bcdptr;

}

void main (void)
{
	init_LCD();							// Initialise LCD
	initPorts();						// Initialise ports
	lcd_putstring("EEE2046F Prac 3B");	// Display string on line 1
	lcd_command(LINE_TWO);				// Move cursor to line 2
	lcd_putstring("Dewan Pieterse");	// Display string on line 2

	for(;;)								// Loop forever
	{
		if ((GPIOA->IDR & SW0) == 0 )//&& (isStarted == 0))
		{
			lcd_command(CLEAR);
			lcd_putstring("Weather Station");
			lcd_command(LINE_TWO);
			lcd_putstring("Press SW2");
			isStarted = 1;
		}

		else if (isStarted && (GPIOA->IDR & SW1) == 0)
		{
			count ++;					//Increments the count value by 1
			GPIOB->ODR = count;			//Displaying count on the LEDs
			lcd_command(CLEAR);			//Clears LCD before printing again
			lcd_putstring("Rain bucket tip");//Prints to LCD
			Delay();					/*Delay to prevent multiple counts
										  on one press of the button.		///////////*/
		}

		/*else if (isStarted && ((GPIOA->IDR & SW2) == 0))
		{
			lcd_command(CLEAR);
			lcd_putstring("Rainfall:");
			lcd_command(LINE_TWO);
			lcd_putstring(" ");
			lcd_putstring(" mm");
		}*/

		if ((GPIOA->IDR & SW2) == 0)			//This is for f)///////////////////////////////////////////////////////////////////////////////////
		{
			lcd_command(CLEAR);
			lcd_putstring("Count:");
			lcd_command(LINE_TWO);
			char *temp1;
			temp1 = ConverttoBCD(count);
			int i;
			for (i = 0; i < 5; i++)
			{
				char j = temp1[i];
				lcd_putchar(j);
				if (i == 3)
				{
					lcd_putstring(".");
				}
			}
			//lcd_putstring(" mm");
		}
	}
}												// End of main

// END OF PROGRAM
