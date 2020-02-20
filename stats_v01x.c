



#include <string.h>
#include <stdlib.h>
#include <stdio.h>
// SimpleLink includes
#include "simplelink.h"

// driverlib includes
#include "hw_ints.h"
#include "hw_types.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "utils.h"
#include "interrupt.h"
#include "pin.h"

// common interface includes
#include "uart_if.h"
#include "uart.h"
#include "common.h"
#include "pinmux.h"
#include "i2c_if.h"
#include "hw_memmap.h"
#include "gpio.h"
#include "gpio_if.h"
// HTTP Client lib
#include <http/client/httpcli.h>
#include <http/client/common.h>
#include <math.h>
#include "wdt.h"
#include "wdt_if.h"

#define alarm_on            0
#define alarm_off           1
#define SH_GPIO_9           9       // Red LED
#define array_size          10

extern int ts_write_cmmnd (unsigned char commnd);
extern int ts_read_range(unsigned char *pucRegValue);
extern int ts1_write_cmmnd (unsigned char commnd);
extern int ts1_read_range(unsigned char *pucRegValue);
extern void hexdec_long( uint32_t count );

extern unsigned int uiGPIOPort;//02/17/2017
extern unsigned char pucGPIOPin;//02/17/2017
extern unsigned char snr_flt;
extern uint8_t Rx_buf[50];
extern unsigned char T_snr [2];
extern unsigned int stl_data1[];
extern unsigned int stl_data2[];
extern unsigned char snr_flg;
extern unsigned char snr_flg1;
extern volatile tBoolean g_bFeedWatchdog;


double sonar_sensr1(unsigned long int *mean){

    unsigned char data_rdy = 1;
    unsigned char i;
    unsigned int temp, T_snr_int;
    double variance = 0;

    for (i = 0; i < (array_size - 1); i++){//[0,1,2,3,4,5,6,7,8,9...] - 50 samples
        stl_data1[i] = stl_data1[(i+1)]; // shift 50 samples (initially all zeros)
    }

    data_rdy = GPIOPinRead(GPIOA0_BASE, 0x80);//Read PIN_62 GPIO_7 - Trash-Sonar Pin 2 - Address Announce / Status
    if (data_rdy == 0){//Read Data when PIN2 is Low
        ts_write_cmmnd ((unsigned char)0x51);//spiked
        if (snr_flt == 1){//I2C Fault detected
            LOOP_FOREVER();
        }
        MAP_UtilsDelay(1280000);//delay 0.08 s #define SEC_TO_LOOP(x)        ((80000000/5)*x) -> 16000 000 - 1sec
    }
    data_rdy = GPIOPinRead(GPIOA0_BASE, 0x80);//Read PIN_62 GPIO_7 - Trash-Sonar Pin 2 - Address Announce / Status
    if (data_rdy == 0){//Read Data when PIN2 is Low
        ts_read_range(T_snr);
        T_snr_int = T_snr[1] << 8;
        T_snr_int = T_snr_int + (unsigned int)T_snr[0];
        stl_data1[(array_size - 1)] = T_snr_int;//The new sample
    }

    *mean = 0;
    for (i = 0; i < (array_size - 1); i++){//Raw data mean based on 0-49 -> 50 samples
        *mean = *mean + stl_data1[i];//sum of 50 samples with the new sample
    }

    //UART_PRINT(" %d ", stl_data1[(array_size - 1)]);// print the last collected sample (current sample [50])
    *mean = *mean / (array_size - 1); // *mean/49

    variance = 0;
    for(i=0; i < (array_size - 1); ++i){//[0,1,2,3,4,5,6,7,8,9]
        temp = stl_data1[i];
        if ((unsigned int) *mean > stl_data1[i])
            temp = *mean - stl_data1[i];
        else
            temp = stl_data1[i] - *mean;
        variance += pow((double) temp, 2);
    }

    return sqrt(variance/(array_size - 2));
}

double sonar_sensr2(unsigned long int *mean){

    unsigned char data_rdy = 1;
    unsigned char i;
    unsigned int temp, T_snr_int;
    double variance = 0;
    //Data shift
    for (i = 0; i < (array_size - 1); i++){//[0,1,2,3,4,5,6,7,8,9...] - 50 samples
        stl_data2[i] = stl_data2[(i+1)]; // shift 51 samples
    }
    //Get new point 51
    data_rdy = GPIOPinRead(GPIOA0_BASE, 0x80);//Read PIN_62 GPIO_7 - Trash-Sonar Pin 2 - Address Announce / Status
    if (data_rdy == 0){//Read Data when PIN2 is Low
        ts1_write_cmmnd ((unsigned char)0x51);
        if (snr_flt == 1){//I2C Fault detected
            LOOP_FOREVER();
        }
        MAP_UtilsDelay(1280000);//delay 0.08 s #define SEC_TO_LOOP(x)        ((80000000/5)*x) -> 16000 000 - 1sec
    }
    data_rdy = GPIOPinRead(GPIOA0_BASE, 0x80);//Read PIN_62 GPIO_7 - Trash-Sonar Pin 2 - Address Announce / Status
    if (data_rdy == 0){//Read Data when PIN2 is Low
        ts1_read_range(T_snr);
        T_snr_int = T_snr[1] << 8;
        T_snr_int = T_snr_int + (unsigned int)T_snr[0];
        stl_data2[(array_size - 1)] = T_snr_int;//Load New sample 10, replace old stl_data2[(array_size - 1)] sample
    }

    *mean = 0;
    //Compute mean based on saple 0-49
    for (i = 0; i < (array_size - 1); i++){//Raw data mean based on 0-9 -> 10 samples
        *mean = *mean + stl_data2[i];//sum of 40 samples with the new sample
    }

    //UART_PRINT(" %d \n", stl_data2[(array_size - 1)]);// print the last collected sample (current sample [50])
    *mean = *mean / (array_size - 1); // *mean/49 Raw data mean

    variance = 0;
    //Compute STDEV based on sample 0-49
    for(i=0; i < (array_size - 1); ++i){//[0,1,2,3,4,5,6,7,8,9]
        temp = stl_data2[i];
        if ((unsigned int) *mean > stl_data2[i])
            temp = *mean - stl_data2[i];
        else
            temp = stl_data2[i] - *mean;
        variance += pow((double) temp, 2);
    }
    return sqrt(variance/(array_size - 2));
}

void prnt_sonar (double st_dev1, double st_dev2, unsigned long int mean_SNSR1, unsigned long int mean_SNSR2){
    UART_PRINT("MEAN1: %d ", mean_SNSR1);
    UART_PRINT("MEAN2: %d ", mean_SNSR2);
    UART_PRINT("STDEV1: %.1f ", st_dev1);
    UART_PRINT("STDEV2: %.1f ", st_dev2);
    if (st_dev1 > 10){//motion sensor
        UART_PRINT("\nMotion L-SNSR %.1f ", st_dev1);
    }
    if (st_dev2 > 10){//motion sensor
        UART_PRINT("Motion R-SNSR %.1f ", st_dev2);
    }
    UART_PRINT("\n");
}



