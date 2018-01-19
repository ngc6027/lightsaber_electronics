#include <xc.h>
#include <p32xxxx.h>
#include <plib.h>
#include <peripheral/adc10.h>
#include <peripheral/int.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mic_adc.h"

#define ADC_PRI     1
#define ADC_SUBPRI  0

// 2 kHz sampling rate @ 2.5 MHz PBCLK
#define ADCS_SET    47
#define SAMC_SET    13

static uint16_t adc_val;
static int diff;

void __ISR(_ADC_VECTOR, IPL1AUTO) adc_handler(void)
{
    // smoothing function 
    int val1, val2, val3, val4;
    static int time0 = 0, time1 = 0;
    
    val1 = ADC1BUF0;
    val2 = ADC1BUF1;
    val3 = ADC1BUF2;
    
    if(time0 == 0){
        time0 = _mfc0(9, 0);
    } else {
        time1 = _mfc0(9, 0);
        diff = time1 - time0;
        time0 = 0;
    }
    
    mAD1ClearIntFlag();
    return;
}

void initialize_adc()
{
    adc_val = 0;
    
    // PORTSetPinsAnalogIn(IOPORT_B, 0x04);
    
    // This enables ADC port 0 and disables all others.
    // TODO: Need 3 inputs: z-axis, y-axis, x-axis
    OpenADC10(ADC_FORMAT_INTG | ADC_AUTO_SAMPLING_ON | ADC_CLK_AUTO,
              ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE | 
                  ADC_SAMPLES_PER_INT_3 | ADC_SCAN_ON, 
              ADC_CONV_CLK_PB, 
              ENABLE_AN2_ANA | ENABLE_AN3_ANA | ENABLE_AN4_ANA,       // TODO: choose 3 analog ports here
              ~(SKIP_SCAN_AN2 | SKIP_SCAN_AN3 | SKIP_SCAN_AN4)
             );
    AD1CON3bits.ADCS = ADCS_SET;
    AD1CON3bits.SAMC = SAMC_SET;
    
    
    SetChanADC10(ADC_CH0_NEG_SAMPLEA_NVREF);
    //AD1CHS = (2 << 16);
    
    // configure interrupts for the ADC module
    ConfigIntADC10(ADC_INT_ON | ADC_INT_PRI_1 | ADC_INT_SUB_PRI_0);
    //IFS1bits.AD1IF = 0;
    //IPC6bits.AD1IP = ADC_PRI;
    //IPC6bits.AD1IS = ADC_SUBPRI;
    //IEC1bits.AD1IE = 1;
    
    // turn on the ADC module
    EnableADC10();
    AD1CON1bits.ASAM = 1;
    
    return;
}
