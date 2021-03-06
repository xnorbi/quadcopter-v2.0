#include "init.h"

void initInterrupts()		//MAX priority is 3!
{
	enable_irq(INT_PIT0-16);		//PID Interrupt, Initialize the NVIC to enable the specified IRQ
	set_irq_priority (INT_PIT0-16, 2);	//Set priority 2
	enable_irq(INT_UART4_RX_TX-16);			//Interrupt from data on bleutooth module	
	set_irq_priority (INT_UART4_RX_TX - 16, 1);
	enable_irq(INT_ADC0 - 16);		// Initialize the NVIC to enable the specified IRQ
	set_irq_priority (INT_ADC0 - 16, 3); //Set priority  */
	enable_irq(INT_ADC1 - 16);		// Initialize the NVIC to enable the specified IRQ
	set_irq_priority (INT_ADC1 - 16, 3); //Set priority  */
	//enable_irq(INT_TPM2 - 16);		//SDcard log interrupt Initialize the NVIC to enable the specified IRQ
	//set_irq_priority (INT_TPM2 - 16, 3); //Set priority 3
	enable_irq(INT_PORTA - 16);		// Initialize the NVIC to enable the specified IRQ
	set_irq_priority (INT_PORTA - 16, 1); //Set priority
	enable_irq(INT_PORTE - 16);		// Initialize the NVIC to enable the specified IRQ
	set_irq_priority (INT_PORTE - 16, 1); //Set priority
}


void Init()
{
	InitClock();
	InitSysTick();
	_SLCDModule_Init();
	InitADC();
	InitADCpins();
//	DMA_ADC_init(DMA_BASE_PTR,ADC_SOURCE,CHANNEL_2,(uint32_t)(&ADC0_RA),(uint32_t)(&adc_data[0]),15,32); //atalakitas nagysaga 32bit??
//	initLEDs();			//A panelon levo LEDek inicializalasa	vagy ez
	init_Motor_PWM();
	init_Ticker();		//FTM1 Timer id�m�r�sre haszn�llom, csak bekapcsolom
//	init_SPI1();		//SPI0 inicialz�l�sa be�litasa
//	init_SD();			//SD k�rtya inicializalasa
	Init_I2C();		//
	rtc_init(0,999,0,0,FALSE);
	//RTC();				//RTC_TSR regiszter m�sodperc sz�ml�l� bekapcsol�sa
	initTimer0();		//Timer for PID interrupt
	initTimer1();		//Timer for SDcard log interrupt
	initTimer2();		//Timer for Turnigy 9X v2 receiver
	InitUARTs();
//	DMA_uart_init(DMA_BASE_PTR,UART0_SOURCE,CHANNEL_1,(uint32_t)(&UART0_D),(uint32_t)(&uart_data),1,8);	//beallitani az interruptot m�g!!!
//	initBluetooth(); //Interrupt on lost signal
	initInterrupts();
	init_turnigy9x();
	init_MPU6050();
//	init_BMP180();

}

/*
 * for K20
void initClock(void)
{
	SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK); // Enable clock gate for ports to enable pin routing 

	pll_init(8000000, LOW_POWER, CRYSTAL, 4, 25, MCGOUT);
}
*/
