/**********************************************************************
* Filament Control for dsPIC
* © Harpreet Singh, 2017
* Version  = 77 
* FileName:        main.c
* Dependencies:    Header (33FJ256GP710A.h) files
* Processor:       dsPIC33FJ256GP710A
* Compiler:        CCS v5.074 or Higher
*
************************************************************************/

#include <33FJ256GP710A.h>
#device ADC = 12 //enable it for 4095
#fuses XT,NOWDT,NOPROTECT
#use delay(crystal = 8Mhz)
#use spi(SLAVE, SPI2, BITS = 8, MODE = 1, ENABLE = PIN_G9, stream = SPI_2)

#define Heater1 PIN_F2
#define Heater2 PIN_F3
#define Heater3 PIN_F4
#define Heater4 PIN_F5

int8 SPI_Flag = 0, Byte_Count = 0, Rx, Tx, Cmand, ProbeID = 2,count = 0;
int8 Version = 0x77, TempLowDisplay = 7, VoltageLowDisplay = 8, TempSetPointLow = 9, HVSetPointLow = 10, TempHiDisplay = 11;
int8 PWM1 = 0, PWM2 = 0, PWM3 = 0, PWM4 = 0;
float Current, Alpha = 0.0024169921875;  //
int8  HVSetPointHi = 1, TempSetPointHi = 23;
int Value, duty1, duty2, duty3, duty4;

#INT_SPI2

void spi2_slave_isr(void)
{
  Rx = spi_xfer_in(SPI_2, 8);
  Byte_Count++; 
 
  switch(Byte_Count)
            {
            case 1:
                spi_prewrite(Version);
                if(Cmand == 1)
                   TempSetPointLow = Rx;
                else if(Cmand == 2)
                   HVSetPointLow = Rx;
                break;
      
            case 2: 
                Cmand = Rx;
                if(Cmand == 1)
                   spi_prewrite(TempLowDisplay);
                else if(Cmand == 2)
                        spi_prewrite(VoltageLowDisplay);
                else if(Cmand == 3)
                        spi_prewrite(ProbeID);
                break;
            
            
            case 3: 
                 
                if(Cmand == 1)
                   spi_prewrite(TempHiDisplay);
                else if(Cmand == 2)
                    {PWM1 = Rx;
                    spi_prewrite(0);}
                else if(Cmand == 3)
                   spi_prewrite(0);
                else if(Cmand == 7)
                    {PWM2 = Rx;
                    spi_prewrite(0);}
                break;      
            
            
            case 4: 
                if(Cmand == 1)
                   TempSetPointHi = Rx;
                else if(Cmand == 2)
                    {PWM3 = Rx;
                    spi_prewrite(0);}
                else if(Cmand == 7)
                    {PWM4 = Rx;
                    spi_prewrite(0);}
                Byte_Count = 0;
                break;
             
            }
}



void main()
{    
   output_float(PIN_G9); // SS as an input
   setup_adc_ports(sAN0 | sAN2 | sAN3| sAN4, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);
   set_adc_channel(3);
   
    //Frequency 100Khz set up for PWM 1,2,3,4   
    setup_timer2(TMR_INTERNAL | TMR_DIV_BY_1, 39);
    setup_compare(1, COMPARE_PWM | COMPARE_TIMER2);
    set_pwm_duty(1,0);
    setup_compare(2, COMPARE_PWM | COMPARE_TIMER2);
    set_pwm_duty(2,0);
    setup_compare(3, COMPARE_PWM | COMPARE_TIMER2);
    set_pwm_duty(3,0);
    setup_compare(4, COMPARE_PWM | COMPARE_TIMER2);
    set_pwm_duty(4,0);
    
   enable_interrupts(INT_SPI2);
   enable_interrupts(INTR_GLOBAL);
   
   TempLowDisplay = 0;
   VoltageLowDisplay = 0;
   TempHiDisplay = 1;
 
   while(1)
    {
       duty1 = PWM1 * 0.4;
       set_pwm_duty(1,duty1);
       
       duty2 = PWM2 * 0.4;
       set_pwm_duty(2,duty2);
       
       duty3 = PWM3 * 0.4;
       set_pwm_duty(3,duty3);
       
       duty4 = PWM4 * 0.4;
       set_pwm_duty(4,duty4);

      
    }
}   
