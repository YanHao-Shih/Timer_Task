
#include <ioavr.h>
#include "uart0.h"
#include "type.h"

#define TASK_NUM 5
// Mode
#define NONE	0
#define ONCE	1
#define REPEAT	2

volatile unsigned short u16Timer_Count[TASK_NUM]={0};
unsigned short u16Function_ExecuteTime=0;
unsigned char u8Function_Mode=0;
unsigned char (*u8Function)(void);
unsigned char Timer_Task=0;
volatile unsigned short u16Delay_Count=0;

typedef struct Task{
	unsigned char mode;
	unsigned short time;
	unsigned char (*fun)(void);
}TimeTask;

TimeTask MyTask[TASK_NUM];
//-----------------------------------------------------------------------------------------------------------------
//Timer用來檢查是否有函式需要被執行
void TimerTask(void)
{
  unsigned char loop=0;
  
  if(Timer_Task)//有任務要執行
  {
    for(loop=0;loop<Timer_Task;loop++){
              
      u16Timer_Count[loop]++;
      
      if(u16Timer_Count[loop]>=MyTask[loop].time)
      {
        u16Timer_Count[loop]=0;
        
        MyTask[loop].fun();
        
        if(MyTask[loop].mode==ONCE)
        {
          if(Timer_Task>1){//Over one task
            MyTask[loop].time=MyTask[loop+1].time;
            MyTask[loop].mode=MyTask[loop+1].mode;;
            MyTask[loop].fun=MyTask[loop+1].fun;
          }else{
            MyTask[loop].time=0;
            MyTask[loop].mode=0;
            MyTask[loop].fun=0;
          }
          Timer_Task=Timer_Task-1;
        }	
      }
    }
  }
  else//沒有任務要執行
  {
    u16Timer_Count[loop]=0;
  }
}
//-----------------------------------------------------------------------------------------------------------------
//設定Timer要執行函式
unsigned char askTimer(unsigned short u16Time,unsigned char u8Mode,unsigned char (*fun)(void))
{
  //Set time
  MyTask[Timer_Task].time=u16Time;
  //Set Mode
  MyTask[Timer_Task].mode=u8Mode;
  //Set Function 
  MyTask[Timer_Task].fun=fun;
  //Task count 
  if(Timer_Task<=TASK_NUM){Timer_Task++;};
  
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------
void Timer1_Init(void){
  SREG &=~(1<<7);//Global Interrupt disabled
  TCCR1A&=~((1<<0)|(1<<1));//CTC mode,Top Value:OCR1A
  TCCR1B&=~(1<<4)|(1<<1)|(1<<0);//CTC mode,Top Value:OCR1A,System Clock/1
  TCCR1B|=(1<<3)|(1<<0);//CTC mode,Top Value:OCR1A,System Clock/1
  //OCR1AH=(499/256);//System Clock:1MHz,Timer1:1ms
  //OCR1AL=(499%256);//System Clock:1MHz,Timer1:1ms
  OCR1AH=(3999/256);//System Clock:8MHz,Timer1:1ms
  OCR1AL=(3999%256);//System Clock:8MHz,Timer1:1ms
  SREG |=(1<<7);//Global Interrupt enabled
  TIMSK1|=(1<<1);//Timer1 Output Compare A Match Interrupt Enable
}
//-----------------------------------------------------------------------------------------------------------------
void Delay_1ms(unsigned short u16Count){
  u16Delay_Count=0;
  while(u16Delay_Count<u16Count);
}
//-----------------------------------------------------------------------------------------------------------------
#pragma vector = TIMER1_COMPA_vect
__interrupt void Timer1_IRQ(void){
    TIFR1&=~(1<<0);//clear Timer overflow flag
  
    TimerTask();
    
    if(u16Delay_Count<65535){u16Delay_Count++;}
}
//-----------------------------------------------------------------------------------------------------------------
unsigned char GPIOB_Blink1(void){
  PORTB^=(1<<5);
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------
unsigned char GPIOB_Blink2(void){
  PORTB^=(1<<4);
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------
void main( void )
{
  Timer1_Init();
  DDRB|=((1<<4)|(1<<5));
  
  askTimer(5000,ONCE,GPIOB_Blink1);
  askTimer(1000,REPEAT,GPIOB_Blink2);
  
  while(1)
  {
  }
}
