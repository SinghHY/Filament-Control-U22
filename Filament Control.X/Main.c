#include <33FJ256GP710A.h>
#fuses XT,NOWDT,NOPROTECT
#use delay(crystal = 8Mhz, clock = 100Mhz)
#use spi(SLAVE, SPI2, BITS = 8, MODE = 1, ENABLE = PIN_G9, stream = SPI_2)

#define LED PIN_A3
#define LED2 PIN_A0

int8 SPI_Flag = 0, Byte_Count = 0, Rx, Tx, Cmand, ProbeID = 2, count = 0;
int8 Version = 7;


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
                   count = Rx;
                else if(Cmand == 2)
                   count = Rx;
                break;

            case 2: 
                Cmand = Rx;

                if(Cmand == 1)
                   spi_prewrite(25);
                else if(Cmand == 2)
                        spi_prewrite(35);
                else if(Cmand == 3)
                        spi_prewrite(ProbeID);
                break;
            
            case 3: 
                if(Cmand == 1)
                   spi_prewrite(45);
                else if(Cmand == 2)
                   spi_prewrite(SPI_Flag);
                else if(Cmand == 3)
                   spi_prewrite(SPI_Flag);
                break;      
                
            case 4: 
                Byte_Count = 0;
                if(Cmand == 1)
                   count = Rx;
                else if(Cmand == 2)
                    count = Rx;
                

                break;                
            default:
                Byte_Count = 0;
                break;        
        }

}




void main()
{    
   output_float(PIN_G9); // SS as an input
   
   enable_interrupts(INT_SPI2);
   enable_interrupts(INTR_GLOBAL);
   
   
   while(1)
    {
       output_high(LED);
       delay_ms(1000);
       output_low(LED);
       delay_ms(1000);

    }
}   