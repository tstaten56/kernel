/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed:_____________________________________ Date:_____________        */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS kernel, Version 1.01, Spring 2015.                 */

#define word(hb,lb) (hb*256+lb)

void printString(char*);
void readString(char*);
void writeInt(int);
void readInt(int*);
void handleInterrupt21(int, int, int, int);

void main()
{
   char line[80];
   int x;
   makeInterrupt21();
   interrupt(33,0,"Hello world\r\n\0",0,0);
   interrupt(33,0,"\r\n\0",0,0);
   interrupt(33,0,"Enter a line: \0",0,0);
   interrupt(33,1,line,0,0);
   interrupt(33,0,"\r\n\0",0,0);
   interrupt(33,0,"You wrote: \0",0,0);
   interrupt(33,0,line,0,0);
   interrupt(33,0,"\r\n\0",0,0);
   interrupt(33,0,"Enter a number: \0",0,0);
   interrupt(33,15,&x,0,0);
   interrupt(33,0,"\r\n\0",0,0);
   interrupt(33,0,"You entered \0",0,0);
   interrupt(33,14,x,0,0);
   interrupt(33,0,"\r\n\0",0,0);
   while(1);
}

void printString(char* c)
{
   /* Print each character using interrupt 16 until it hits the end of string
      character '\0' */
   while (*c != '\0')
   {
       interrupt(16,14*256+*c,0,0,0);
       ++c;    
   }
   return;
}

void readString(char* c)
{
   /* All characters typed should be printed to the screen, after
    reading a character. The character should be printed to the screen using
     interrupt 16  (Like printScreen). *
    * BACKSPACE key: when a backspace (ASCII 0x8) is pressed, it should print
    the backspace to the screen but not store it in the array. Instead
    it should decrease the array index. (Make sure the array index does not
    go below 0)*/
    int cIndex = 0;
    char temp = interrupt(22,0,0,0,0);
    while (temp != 0xD) /* 0xD is the enter key*/
    {
        if (temp == 0x8) /*we have a backspace character*/
        {
        if (cIndex > 0) /*make sure the array index does not go below 0*/
            {
                c--; /*delete the last character from the array*/
                cIndex--;
            interrupt(16,14*256+temp,0,0,0); /*print backspace to screen*/
            interrupt(16,14*256+' ',0,0,0);
            interrupt(16,word(14,temp));     
            }
           
        }
        else
        {
            interrupt(16,14*256+temp,0,0,0);
            *c = temp;
            c++;                    /*increment array*/
            cIndex++;
        }
        temp = interrupt(22,0,0,0,0); /*add next character to temp*/

     }
     *c = 0x0;
   return;
}

int mod(int a, int b)
{
   int x = a;
   while (x >= b) x = x - b;
   return x;
}

int div(int a, int b)
{
   int q = 0;
   while (q * b <= a) q++;
   return (q - 1);
}

void writeInt(int x)
{
   char number[6], *d;
   int q = x, r;
   if (x < 1)
   {
      d = number; *d = 0x30; d++; *d = 0x0; d--;
   }
   else
   {
      d = number + 5;
      *d = 0x0; d--;
      while (q > 0)
      {
         r = mod(q,10); q = div(q,10);
         *d = r + 48; d--;
      }
      d++;
   }
   printString(d);
}

void readInt(int* number)
{
   int num = 0;
   char tempNum[7];
   int cIndex = 0;
   readString(tempNum);
   /*Converting the string to an integer */
   num += tempNum[cIndex] - '0'; /*ASCII  48 is equal to 0*/
   cIndex++;
   while (tempNum[cIndex] != 0x0)
   {
       num *= 10;
      /* num += *c - 48; ASCII  48 is equal to 0*/
       num += tempNum[cIndex] - '0';  
       cIndex++;
   }
   *number = num;
   return;
}

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
   if (ax == 0)
   {
      printString(bx);
   }
   else if (ax == 1)
   {
      readString(bx);
   }
   else if (ax == 14)
   {
      writeInt(bx);
   }
   else if (ax == 15)
   {
      readInt(bx);
   }
   else
   {
      printString("AX is an undefined value of: \r\n\0");
      writeInt(ax);
      printString("\r\n\0");
   }
}

