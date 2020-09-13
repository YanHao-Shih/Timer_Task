#include "uart0.h"

/*9600,n,8,1*/
void Init_UART0_Func(void)
{
    UCSR0B &= ~((1<<4)|(1<<3));   //Turn off TX and RX
    //9600, U2Xn=0, UBRRn=103
    UCSR0A &= ~(1<<1);            //U2Xn=0
    UBRR0 = 103;                  //UBRRn(16-bit)
    UCSR0C &= ~((1<<7)|(1<<6));   //Asynchronous USART
    UCSR0C &= ~((1<<5)|(1<<4));   //None Parity
    UCSR0C &= ~(1<<3);            //1-bit Stop bit
    UCSR0C |= ((1<<2)|(1<<1));    //8-bit Data UCSR0C的Bit2:1-UCSZ01:0設為1:1
    UCSR0B &= (1<<2);             //& UCSR0B的Bit2-UCSZ02設為0
    UCSR0B |= ((1<<4)|(1<<3));    //Turn on TX and RX
}
void Send_UART0Byte(uint8_t iData)
{  
    while(!(UCSR0A & (1<<5)));    //UCSR0A的Bit5為1時代表transmit buffer為空的, 可以寫入新的要被傳出的資料, 不等於1時判斷式為成立則處於while迴圈內
    UDR0 = iData;                 //UDR0出現在等號右邊, 代表把值寫入
}
void Send_UART0String(uint8_t* myString)
{
    uint8_t loop = 0;
  
    while(myString[loop] != '\0')
    {
        Send_UART0Byte(myString[loop]);
        loop++;
    }
}
void Send_UART0HEXTOASCII(uint8_t number)
{
    Send_UART0Byte((number/100) + '0');       
    Send_UART0Byte(((number%100)/10) + '0');     
    Send_UART0Byte((number%10) + '0');          
}
void Read_One_Byte_from_UDR0(uint8_t* ptrData)
{
    while(!(UCSR0A & (1<<7)));   //UCSR0A的Bit7-RXC0為1時代表沒有unread data in the receive buffer
    *ptrData = UDR0;             //UDR0在等號左邊, 代表把值讀出
}
void Send_UART0ByteTOASCII(uint8_t iData)
{
    if(iData/100)
    {
        Send_UART0Byte((iData/100)+ '0');          //顯示百位數字(ASCII格式)
    }
    
    iData %= 100;
    
    if(iData/10)
    {
        Send_UART0Byte((iData/10)+ '0');           //顯示十位數字(ASCII格式)
    }
    
    Send_UART0Byte((iData%10)+ '0');               //顯示個位數字(ASCII格式)
}

















