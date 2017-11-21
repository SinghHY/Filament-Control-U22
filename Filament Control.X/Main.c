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
int8 Version = 0x77;
int8 PWM1 = 0, PWM2 = 0, PWM3 = 0, PWM4 = 0;
int8 AmpSPI1 = 15, AmpSPI2 = 25, AmpSPI3 = 35, AmpSPI4 = 45;
float Current1, Current2, Current3, Current4, Alpha = 0.008056640625;  //
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
                    spi_prewrite(0);
                else if(Cmand == 2)
                    spi_prewrite(0);
                break;
      
            case 2: 
                Cmand = Rx;
                if(Cmand == 1)
                    spi_prewrite(0);
                else if(Cmand == 2)
                        spi_prewrite(3);
                else if(Cmand == 3)
                        spi_prewrite(ProbeID);
                break;
            
            
            case 3: 
                 
                if(Cmand == 1)
                    spi_prewrite(0);
                else if(Cmand == 2)
                    {PWM1 = Rx;
                    spi_prewrite(AmpSPI1);}
                else if(Cmand == 3)
                   spi_prewrite(0);
                else if(Cmand == 7)
                    {PWM2 = Rx;
                    spi_prewrite(AmpSPI2);}
                break;      
            
            
            case 4: 
                if(Cmand == 1)
                    spi_prewrite(0);
                else if(Cmand == 2)
                    {PWM3 = Rx;
                    spi_prewrite(AmpSPI3);}
                else if(Cmand == 7)
                    {PWM4 = Rx;
                    spi_prewrite(AmpSPI4);}
                break;
             
            }
    Byte_Count = 0;
}
/*
#int_timer1 level = 6

void timer1_isr()
{
    set_adc_channel(0);  
    Current1 = read_adc() * Alpha;// Alpha = adc/4096 * 3.3 * 10
    AmpSPI1  = (int8)Current1;
    
    set_adc_channel(2);  
    Current2 = read_adc() * Alpha;// Alpha = adc/4096 * 3.3 * 10
    AmpSPI2  = (int8)Current2;
    
    set_adc_channel(3);  
    Current3 = read_adc() * Alpha;// Alpha = adc/4096 * 3.3 * 10
    AmpSPI3  = (int8)Current3;
 
    set_adc_channel(4);  
    Current4 = read_adc() * Alpha;// Alpha = adc/4096 * 3.3 * 10
    AmpSPI4  = (int8)Current4;
}*/


void main()
{    
   output_float(PIN_G9); // SS as an input
  /* setup_adc_ports(sAN0 | sAN2 | sAN3| sAN4, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);

   
   // Sampling rate setup for TC1 at 100ms//
   setup_timer1(TMR_INTERNAL | TMR_DIV_BY_8, 50000);
   enable_interrupts(INT_TIMER1);*/
   
    //Frequency 100Khz set up for PWM 1,2,3,4   
   /* setup_timer2(TMR_INTERNAL | TMR_DIV_BY_1, 39);
    setup_compare(1, COMPARE_PWM | COMPARE_TIMER2);
    set_pwm_duty(1,0);
    setup_compare(2, COMPARE_PWM | COMPARE_TIMER2);
    set_pwm_duty(2,0);
    setup_compare(3, COMPARE_PWM | COMPARE_TIMER2);
    set_pwm_duty(3,0);
    setup_compare(4, COMPARE_PWM | COMPARE_TIMER2);
    set_pwm_duty(4,0);*/
    
   //enable_interrupts(INT_TIMER1);
   enable_interrupts(INT_SPI2);
   enable_interrupts(INTR_GLOBAL);
   

   while(1)
    {
       

      /* duty1 = PWM1 * 0.4;
       set_pwm_duty(1,duty1);
       
       duty2 = PWM2 * 0.4;
       set_pwm_duty(2,duty2);
       
       duty3 = PWM3 * 0.4;
       set_pwm_duty(3,duty3);
       
       duty4 = PWM4 * 0.4;
       set_pwm_duty(4,duty4);*/
       

      
    }
}   
