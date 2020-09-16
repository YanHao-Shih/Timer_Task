#include "timer_task.h"
//-----------------------------------------------------------------------------------------------------------------
uint8_t Timer_Task=0;
volatile uint16_t u16Timer_Count[TASK_NUM]={0};

TimeTask MyTask[TASK_NUM];
//-----------------------------------------------------------------------------------------------------------------
static uint8_t Empty_Task(void)
{
  return 0;
}
//-----------------------------------------------------------------------------------------------------------------
void Timer_ExecuteTask(void)
{
  uint8_t loop=0;
  
  if(Timer_Task)//Have a task to execute
  {
    for(loop=0;loop<Timer_Task;loop++){
              
      u16Timer_Count[loop]++;
      
      if(u16Timer_Count[loop]>=MyTask[loop].time)
      {
        u16Timer_Count[loop]=0;
        
        MyTask[loop].fun();
        
        if(MyTask[loop].mode==ONCE)
        {
          if((Timer_Task>1) && (loop<(TASK_NUM-1))){//Over one task
            MyTask[loop].time=MyTask[loop+1].time;
            MyTask[loop].mode=MyTask[loop+1].mode;;
            MyTask[loop].fun=MyTask[loop+1].fun;
          }else{
            MyTask[loop].time=0;
            MyTask[loop].mode=0;
            MyTask[loop].fun=Empty_Task;
          }
          Timer_Task=Timer_Task-1;
        }	
      }
    }
  }
  else//no task to execute
  {
    u16Timer_Count[loop]=0;
  }
}
//-----------------------------------------------------------------------------------------------------------------
uint8_t askTimer(unsigned short u16Time,uint8_t u8Mode,uint8_t (*fun)(void))
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
