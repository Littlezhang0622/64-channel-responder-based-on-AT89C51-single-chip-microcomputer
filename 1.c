#include <reg52.h>
#include <absacc.h>

void  timer0(void);
void  display(void);
bit SEC;
sbit  FORBID=P1^7;
sbit  SOUND=P3^5;
sbit  DONE=P1^6;

unsigned char Xcnt,Scnt,Sset,Tcnt;
unsigned char keybuf,STbyte;

code unsigned char tab[]={0xC0,0xF9,0XA4,0XB0,0X99,0X92,0X82,0XF8,0X80,0x98};
//



/*void Delay()
{
	unsigned int i;
	for(i=0;i<200;i++);
}
*/
void timer0(void) interrupt 1 using 1
{
TH0=0x4C;
if (++Tcnt>=10)
    {
	Tcnt=0,SEC=!SEC;
	 if(!SOUND)
	 {
	 SOUND=1;
	 }
     if ((SEC)&&(STbyte==3))
	 {
	    Scnt--;
	    if(Scnt==0x00) 
		{STbyte=4,SOUND=0,FORBID=1;}
	
		
	 }
    }
 }



//
void key(void)
{
  switch(STbyte)
  {
     case 0:
	 if (keybuf==0x1e)
	    {
		   STbyte=1;
		   Scnt=Sset;
		 }
	  break;

//
  	 case 1:
	 if (keybuf==0x1b)
	 {
	 	   Scnt=Sset;
		   STbyte=2;
	 }
	 else if(keybuf==0x1d)
	 {
	 	   STbyte=3;
		   FORBID=0;
	 }
	 break;

 //
  	 case 2:
	 if (keybuf==0x17)
	 {
	       if(++Scnt>99)  Scnt=99;
	 }
	 else if(keybuf==0x0f)
	 {	 
	 	   if(--Scnt<5)  Scnt=5;	 
	 }
	 else if(keybuf==0x1b)
	 {
	       STbyte=1;
		   Sset=Scnt; 	 
	 }
     break;
    /*case 3:
	 if (keybuf==0x1d)
	 {
	    if ((SEC)&&(STbyte==3))
	 {
	    Scnt--;
		if(Scnt==0) STbyte=4,SOUND=0,FORBID=1;
	 }	  
	 }
 	 */
	 case 4:
	 if(keybuf==0x1e)
	    {
		   STbyte=1;
		   Scnt=Sset;		  
		 }
	  break;
   }
  }
 //ÏÔÊ¾ 
  void display(void)
  {
  switch(STbyte)
     {
	    case 0:
		{
		   XBYTE[0x7f00]=0x98;
		   XBYTE[0xbf00]=0xa1;
		   Xcnt=64;
		   XBYTE[0xdf00]=tab[Xcnt/10];
		   XBYTE[0xef00]=tab[Xcnt%10];	
		}
		break;

		case 1:
		{
		
		   XBYTE[0x7f00]=tab[Scnt/10];
		   XBYTE[0xbf00]=tab[Scnt%10];
		   XBYTE[0xdf00]=0xbf;
		   XBYTE[0xef00]=0xbf;
		}
		break;

		case 2:
		{		    
		   XBYTE[0x7f00]=tab[Scnt/10];
		   XBYTE[0xbf00]=tab[Scnt%10];
		   XBYTE[0xdf00]=0xc6;
		   XBYTE[0xef00]=0x89;		
		}
		break;

		case 3:
		{
		   if(DONE)
		   {
		    STbyte=4,SOUND=0,Tcnt=0,FORBID=1;		   
		   }
		   /*else	if(Scnt==0x00) 
		   {STbyte=4,SOUND=0,FORBID=1;}
		   */
		   else
		   { 
		   XBYTE[0x7f00]=tab[Scnt/10];
		   XBYTE[0xbf00]=tab[Scnt%10];		
		   }		    	  	
		}
		break;

		case 4:
		{
		   XBYTE[0x7f00]=tab[Scnt/10];
		   XBYTE[0xbf00]=tab[Scnt%10];
		   if(DONE)
		   {
		   Xcnt=P1&0x3F;
		   XBYTE[0xdf00]=tab[Xcnt/10];
		   XBYTE[0xef00]=tab[Xcnt%10];
		   }
		}	
		break; 
	 } 
  }
 void main(void)
 {
 unsigned char i,j;
 unsigned int k;
 STbyte=0;
   Tcnt=0;
   Sset=50;
   SEC=0;
   ET0=1;
   EA=1;
   TMOD=0x01;
   TH0=0x4c;
   TL0=0x00;
   TR0=1;
   while(1)
   {
     display();
     keybuf=(P3&0x1f);
     if(keybuf!=0x1f)
     {
       key();
	   display();
	   k=0;
	   do
	   {
	   k++;
	   if(k>330)
	   {
	   	k=300;
		key();
		display();
		}
		for (i=0,j=0;j<=200;j++)
		  {
		   if((P3&0x1f)==0x1f)  {i++;}
		  }
	    }
		while(i<150);
	  for(k=0;k<1000;k++)
	  {
	  	  if((P3&0x1f)!=0x1f){k=0;}
	  }
     } 
   }
 } 
 
