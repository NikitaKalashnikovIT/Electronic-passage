#include "messages.h"

char Messages[MAX_MESSAGES];
//=========================================================================
void InitMessages(void)
{ char i;
	for(i=0; i<MAX_MESSAGES;i++)
	{
		Messages[i]=0;
	}
}
//=========================================================================
/* Ф-я устанавливает сообщение Msg в состояние установлено (1) но не активно (2)*/
void SendMessage(char Msg)
{
	Messages[Msg]=1;
	
}
//=========================================================================
/* Ф-я возвращает 1 если сообщение Msg в состоянии "активно"                 */
char GetMessage(char Msg)
{
	if(Messages[Msg]==2)		// если сообщение в состоянии "активно"
		return(1);
	return(0);
}
//==========================================================================
/* Ф-я переводит все установленные сообщения в состояние "активно", а так же 
уничтожает все активные сообщения                                            */
void ProcessMessages(void)
{char i;
	for(i=0; i<MAX_MESSAGES;i++)
	{
		if(Messages[i]==2) Messages[i]=0;
		if(Messages[i]==1) Messages[i]=2;
		
	}
}
