#include <33FJ256GP710A.h>
#fuses XT,NOWDT,NOPROTECT
#device ADC = 12 
#use delay(crystal = 8Mhz)
#use spi(SLAVE, SPI2, BITS = 8, MODE = 1, ENABLE = PIN_G9, stream = SPI_2)


#define Heater1 PIN_F2
#define Heater2 PIN_F3
#define Heater3 PIN_F4
#define Heater4 PIN_F5

/******************************************************************************/
// Moving Average FIlter Parameters
// Alpha = 0.024169921875 = 3.3/4096 * 10 * 3 ; 10 is divider to send value on 8 bits; 3 is resister divider in circuit
#define Sample_Length  20
float Alpha = 0.024169921875;
float Cofficent = (1/(float)Sample_Length) * Alpha;
/******************************************************************************/

int8 SPI_Flag = 0, Byte_Count = 0, Rx, Tx, Cmand, ProbeID = 2,count = 0;
int8 Version = 7;
int8 PWM1 = 0, PWM2 = 0, PWM3 = 0, PWM4 = 0;
int8 AmpSPI1 = 15, AmpSPI2 = 25, AmpSPI3 = 35, AmpSPI4 = 45;
float Current1, Current2, Current3, Current4;  //
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
                Byte_Count = 0;            
                if(Cmand == 1)
                    spi_prewrite(0);
                else if(Cmand == 2)
                    {PWM3 = Rx;
                    spi_prewrite(AmpSPI3);}
                else if(Cmand == 7)
                    {PWM4 = Rx;
                    spi_prewrite(AmpSPI4);}
                break;
                
            default:
                Byte_Count = 0;
                break;
            }

}


float LP_Filter(int ch)
  {
    float Filter_Out;
    unsigned int16 i,sum, array[Sample_Length];

    set_adc_channel(ch);
    for(i = 0; i < Sample_Length; i++)
        {
            array[i] = read_adc();
            sum += array[i];
        }    
        Filter_Out = (float)sum * Cofficent;
        sum = 0;
    return(Filter_Out);    
  }
void main()
{    
   output_float(PIN_G9); // SS as an input
   setup_adc_ports(sAN0 | sAN2 | sAN3| sAN4, VSS_VDD);
   setup_adc(ADC_CLOCK_INTERNAL);

   
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
       

       AmpSPI1  = (int8)LP_Filter(0);
       AmpSPI2  = (int8)LP_Filter(2);
       AmpSPI3  = (int8)LP_Filter(3);
       AmpSPI4  = (int8)LP_Filter(4);
    }
}   