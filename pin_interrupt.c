/*
 * pin_interrupt.c
 *
 *  Created on: Feb 18, 2020
 *      Author: Roman.Chak
 */

void pin_interrupt(void){
    asm(" NOP");
    //GPIOPinWrite(GPIOA1_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_3);

    GPIOIntDisable(GPIOA0_BASE,GPIO_PIN_6);
    GPIOIntClear(GPIOA0_BASE,GPIO_PIN_6);
    IntDisable(INT_GPIOA0);
    //GPIO_IF_GetPortNPin(SH_GPIO_6,&uiGPIOPort,&pucGPIOPin);
    //Door = GPIO_IF_Get(SH_GPIO_6,uiGPIOPort,pucGPIOPin);
    //if (Door == 1){
    //GPIO_IF_GetPortNPin(SH_GPIO_9,&uiGPIOPort,&pucGPIOPin); // Computes port and pin number from the GPIO number
    //GPIO_IF_Set(SH_GPIO_9,uiGPIOPort,pucGPIOPin,alarm_on);
    UART_PRINT("\n\rDoor swing");
    MAP_UtilsDelay(10000000);
    door_flg = 1;
    //}
    IntPendClear(INT_GPIOA0);
    IntEnable(INT_GPIOA0);
    GPIOIntEnable(GPIOA0_BASE, GPIO_PIN_6);
    timerA0_start((unsigned long)5000);
}


