/**********************************************************************
* Reading ADC with timer interrupt.
* © Harpreet Singh, 2017
* This version uses interrupts to free up the processor. 
* FileName:        main.c
* Dependencies:    Header (33FJ256GP710A.h) files
* Processor:       dsPIC33FJ256GP710A
* Compiler:        CCS v5.074 or Higher
*
************************************************************************/

#include <33FJ256GP710A.h>
#fuses HS,NOWDT,NOPROTECT
#use delay(crystal = 8Mhz)
#use spi(SLAVE, SPI2, BITS = 8, MODE = 1, ENABLE = PIN_G9, stream = SPI_2)

#define GREEN_LED PIN_F6

int8 SPI_Flag = 0, Byte_Count = 0, Rx, Tx, Cmand, ProbeID = 1, count = 0;
int8 Version = 0x77, TempLowDisplay, VoltageLowDisplay, TempSetPointLow, HVSetPointLow, TempHiDisplay;
int8  HVSetPointHi, TempSetPointHi;

#INT_SPI2

void spi2_slave_isr(void)
{
  Rx = spi_xfer_in(SPI_2, 8);
  Byte_Count++; 
  SPI_Flag = 1;

}




void main()
{    
    
    
    //Frequency set up for PWM    
    setup_timer2(TMR_INTERNAL | TMR_DIV_BY_1, 39);
    setup_compare(3, COMPARE_PWM | COMPARE_TIMER2);
    
    
   enable_interrupts(INT_SPI2);
   enable_interrupts(INTR_GLOBAL);

   TempLowDisplay = 0;
   VoltageLowDisplay = 0;
    
   
   set_pwm_duty(3,10);
   
   
   while(1)
    {
       if(SPI_Flag) 
       {
         SPI_Flag = 0;

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
                   spi_prewrite(SPI_Flag);
                else if(Cmand == 3)
                   spi_prewrite(SPI_Flag);
                break;      
            
            
            case 4: 
                if(Cmand == 1)
                   TempSetPointHi = Rx;
                else if(Cmand == 2)
                    HVSetPointHi = Rx;
                
                Byte_Count = 0;
                break;
             
            }

        }
    }
}   