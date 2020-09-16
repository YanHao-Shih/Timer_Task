#include <ioavr.h>
#include "uart0.h"
#include "type.h"
#include "timer.h"
#include "timer_task.h"
//-----------------------------------------------------------------------------------------------------------------
#pragma vector = TIMER1_COMPA_vect
__interrupt void Timer1_IRQ(void){
    TIFR1&=~(1<<0);//clear Timer overflow flag
  
    Timer_ExecuteTask();
}
//-----------------------------------------------------------------------------------------------------------------
static void GPIOB_Init(void)
{
  DDRB|=((1<<4)|(1<<5));
}
//-----------------------------------------------------------------------------------------------------------------
static uint8_t GPIOB_Blink1(void){
  PORTB^=(1<<5);
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------
static uint8_t GPIOB_Blink2(void){
  PORTB^=(1<<4);
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------
void main( void )
{
  Timer1_Init();
  GPIOB_Init();
  
  askTimer(5000,ONCE,GPIOB_Blink1);
  askTimer(1000,REPEAT,GPIOB_Blink2);
  
  TIMSK1|=(1<<1);//Timer1 Output Compare A Match Interrupt Enable
  
  while(1)
  {
  }
}
