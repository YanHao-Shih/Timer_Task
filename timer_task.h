#include "type.h"

// Task Number
#define TASK_NUM 5
// Mode
#define NONE	0
#define ONCE	1
#define REPEAT	2

typedef struct Task{
	uint8_t mode;
	uint16_t time;
	uint8_t (*fun)(void);
}TimeTask;

static uint8_t Empty_Task(void);
void Timer_ExecuteTask(void);
uint8_t askTimer(unsigned short u16Time,uint8_t u8Mode,uint8_t (*fun)(void));
