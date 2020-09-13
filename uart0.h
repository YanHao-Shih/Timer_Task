#ifndef _UART0_H
#define _UART0_H

#include "ioavr.h"
#include "type.h"

void Init_UART0_Func(void);
void Send_UART0Byte(uint8_t);
void Send_UART0String(uint8_t*);
void Send_UART0HEXTOASCII(uint8_t);
void Read_One_Byte_from_UDR0(uint8_t*);
void Send_UART0ByteTOASCII(uint8_t);
#endif