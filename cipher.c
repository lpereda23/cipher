/**
 * Leonardo Pereda
 * November 01, 2017
 * cipher.c
 * This program uses lcg.c to make structs for generating the "random" values
 * There are three helper functions and the main function
 */
 
#include <stdio.h>
#include "lcg.h"
#define FALSE 0
#define TRUE 1

int ecrypt = FALSE;
int dcrypt = FALSE;
int ch = 0;
int lineNum = 1;
unsigned long m = 0;
unsigned long c = 0;
int mCnt = 0;
int cCnt = 0;
int validCh(int in);
struct LinearCongruentialGenerator key(void);
void encrypt(struct LinearCongruentialGenerator* n);
void decrypt(struct LinearCongruentialGenerator* n);

/**
 * If DEBUG is TRUE it will print out some printf statements throughout the 
 * program. Used to debug the program 
 * */
int DEBUG = FALSE;

/** 
 * This function is used to make sure the ch's that are being read from the standar
 * in are valid. This fcuntion is used throughout the entire program 
 * */
int validCh(int in)
{
    /** debug tool */
    if(DEBUG) printf("In validCh function this was passed here [%d]\n", in);
    if((in >= 32) && (in <= 126))
    {
      return TRUE;
    }
    else
    {
      return TRUE;
    }
}

/** 
 * Here I make the key which is struct I will be using to create the randValue
 * in order to encrypt. This uses the lcg.c to create the struct and generate the 
 * next random values everytime it is needed. When the struct is done, there is error checking 
 * and checks for the correct numbers according to the lab instructions.
*/
struct LinearCongruentialGenerator key(void)
{
  struct LinearCongruentialGenerator test;
  ch = getchar();
  if(ch == '\n' || ch == EOF)
  {
    mCnt = cCnt = 0;
    return makeLCG(0, 1);
  }
  if(ch != 'd' && ch != 'e')
  {
    mCnt = cCnt = 0;
    while((ch = getchar()) != '\n');
    return makeLCG(5, 10);
  }
  if(ch == 'd') dcrypt = 1;
  if(ch == 'e') ecrypt = 1;
  while((ch = getchar()) != EOF)
  {
    if(ch >= '0' && ch <= '9')
    {
      m = m*10 + (unsigned long)ch - '0';
      /** Debug tool */
      if(DEBUG) printf("This is m --> %lu\n", m);
      mCnt++;
      if(mCnt > 21)
      {
        mCnt = 0;
        while((ch = getchar()) != '\n');
        return makeLCG(5, 10);
      }
    }
    else if(ch == ',' && mCnt > 0)
    {
      mCnt = 0;
      break;
    }
    else
    {
      mCnt = 0;
      while((ch = getchar()) != '\n');
      return makeLCG(5, 10);
    }
  }
  mCnt = 0;
  while((ch = getchar()) != EOF)
  {
    if(ch == ',' || ch == '\n')
    {
      if(ch == '\n')
      {
        putchar(ch);
        cCnt = 0;
        break;
      }
      cCnt = 0;
      break;
    }
    if(ch >= '0' && ch <= '9')
    {
      c = c*10 + (unsigned long)ch - '0';
      /** Debug tool */
      if(DEBUG) printf("This is c --> %lu\n", c);
      cCnt++;
      if(cCnt > 21)
      {
        cCnt = 0;
        return makeLCG(5, 10);
      }
    }
    else
    {
      cCnt = 0;
      while((ch = getchar()) != '\n');
      return makeLCG(5, 10);
    }
  }
  if(c == 0 || m == 0)
  {
    mCnt = cCnt = 0;
    while((ch = getchar()) != '\n');
    return makeLCG(5,10);
  }
  test = makeLCG(m, c);
  if(test.a > test.m)
  {
    mCnt = cCnt = 0;
    while((ch = getchar()) != '\n');
    return makeLCG(5, 10);
  }
  cCnt = 0;
  return makeLCG(m, c);
}

/** 
 * In this function I am encrypting from the standard in. When in the while loop new randoms are generate
 * on every loop of the loop. This function deals with the special chars as well. And when a char is encrypted 
 * it is echoed out and not saved. 
*/
void encrypt(struct LinearCongruentialGenerator* n)
{
  unsigned long rand;
  unsigned long new;
  while((ch = getchar()) != EOF)
  {
    if(ch == '\n')
    {
      putchar(ch);
      break;
    }
    rand = getNextRandomValue(n);
    new = ch ^ (rand % 128);
    if(new < 32)
    {
      putchar('*');
      putchar('?' + new);
    }
    else if(new == 127)
    {
      putchar('*');
      putchar('!');
    }
    else if(new == '*')
    {
      putchar('*');
      putchar('*');
    }
    else putchar(new);
  }
}

/** 
 * In this function I am decrypting the ch which is reading from standard in
 * If there is a new line it exists the while loop. Else it would generate a new
 * random value. Also deals with the special chars. Checks for the * and goes to the next
 * char in order to know what was encrypted.
*/
void decrypt(struct LinearCongruentialGenerator* n)
{
  unsigned long rand; 
  unsigned long new;
  unsigned long realCh = 0;
  
  while((ch = getchar()) != EOF)
  {
    if(ch == '\n')
    {
      putchar(ch);
      break;
    }
    rand = getNextRandomValue(n);
    if(ch == '*')
    {
      ch = getchar();
      if(ch == '*')
      { 
        new = '*' ^ (rand % 128);
        if(validCh(new) == FALSE)
        {
          printf("Error\n");
          /** debug tool */
          if(DEBUG) printf("Error, this is not valid ascii **[%lu]\n", new);
          while((ch = getchar()) != '\n');
          break;
        }
        else if(validCh(new) == TRUE)
        {
          putchar(new);
        }
      }
      else if(ch == '!')
      {
        new = '!' ^ (rand % 128);
        if(validCh(new) == FALSE)
        {
          printf("Error\n");
          /** debug tool */
          if(DEBUG) printf("Error, this is not valid ascii *![%lu]\n", new);
          while((ch = getchar()) != '\n');
          break;
        }
        else if(validCh(new) == TRUE)
        {
          putchar(new);
        }
      }
      else
      {
        realCh = ch - '?';
        new = realCh ^ (rand % 128);
        /** debug tool */
        if(DEBUG) printf("new in else [%lu]\n", new);
        if(validCh(new) == FALSE) 
        {
          printf("Error\n");
          /** debug tool */
          if(DEBUG) printf("Error, this is not valid ascii else[%lu]\n", new);
          while((ch = getchar()) != '\n');
          break;
        }
        else if(validCh(new) == TRUE)
        {
          putchar(new);
        }
        /** debug tool */
        if(DEBUG) printf("\n");
      }
    }
    else if(ch != '*')
    {
      new = (ch ^ (rand % 128));
      /** debug tool */
      if(DEBUG) printf("new in else [%lu]\n", new);
      if(validCh(new) == FALSE) 
      {
        printf("Error\n");
        /** debug tool */
        if(DEBUG) printf("Error, this is not valid ascii else[%lu]\n", new);
        while((ch = getchar()) != '\n');
        break;
      }
      else if(validCh(new) == TRUE)
      {
        putchar(new);
      }
      /** debug tool */
      if(DEBUG) printf("\n");
    }
  }
}

/** 
 * Here in the main function I am first creating a struct with the m and c in order to 
 * use to make my randValue in order to encrypt. Hace error checking as well.This is 
 * where the line numbers are printed and if there are errors it will print error. 
*/
int main(void)
{
 struct LinearCongruentialGenerator n;
 while(1)
 {
   if(ch == EOF)
   {
     break;
   }
   n = key();
   /** debug tool */
   if(DEBUG) printf("These are the values of the struct x=%lu, a=%lu, m=%lu, c=%lu\n", n.x, n.a, n.m, n.c);

   if(n.x == 0 && n.a == 0 && n.m == 0 && n.c == 0)
   {
     printf("%5d) Error",lineNum);
     lineNum++;
     printf("\n");
   }
   else if(n.x == 0 && n.c == 1 && n.m == 0 && n.a == 0) continue;
   else
   {
     printf("%5d) " ,lineNum);
     lineNum++;
     if(dcrypt == 1) decrypt(&n);
     else encrypt(&n);
   }
   dcrypt = ecrypt = 0;
   m -= m;
   c -= c;
 }
 return 0;
}
