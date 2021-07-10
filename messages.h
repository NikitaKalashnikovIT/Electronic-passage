
#ifndef __STM32_MESSAGES
#define __STM32_MESSAGES

#define MAX_MESSAGES 10

#define MSG_KEY_PRESSED			1			// сообщение о нажатии клавиши
#define MSG_USART						2
#define MSG_SENSOR_DAMAGE		3
#define MSG_FACT_PASS				4

void InitMessages(void);
void SendMessage(char Msg);
char GetMessage(char Msg);
void ProcessMessages(void);
#endif
