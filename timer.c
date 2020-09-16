#include "timer.h"
//-----------------------------------------------------------------------------------------------------------------
void Timer1_Init(void){
  SREG &=~(1<<7);//Global Interrupt disabled
  TCCR1A&=~((1<<0)|(1<<1));//CTC mode,Top Value:OCR1A
  TCCR1B&=~(1<<4)|(1<<1)|(1<<0);//CTC mode,Top Value:OCR1A,System Clock/1
  TCCR1B|=(1<<3)|(1<<0);//CTC mode,Top Value:OCR1A,System Clock/1
#ifdef _SYSTEM_CLOCK_1MHZ_  
  OCR1AH=(499/256);//System Clock:1MHz,Timer1:1ms
  OCR1AL=(499%256);//System Clock:1MHz,Timer1:1ms
#else  
  OCR1AH=(3999/256);//System Clock:8MHz,Timer1:1ms
  OCR1AL=(3999%256);//System Clock:8MHz,Timer1:1ms
#endif  
  SREG |=(1<<7);//Global Interrupt enabled
  //TIMSK1|=(1<<1);//Timer1 Output Compare A Match Interrupt Enable
}
