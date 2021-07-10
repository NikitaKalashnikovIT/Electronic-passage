#include "my_delay.h"

void delay (int a)
{
 volatile int i,j;
 for (i=0 ; i < a ; i++)
 {
  j++;
 }
 return;
}
