#include "tools.h"

int rangerand(int low,int hi)
{
   return rand()%(hi-low+1)+low;
}

int myitoa(int n,char*s)
{
   char temp[10];
   int i,j = 0;
   do
   {
      temp[j++] = n % 10 + '0';  
   }while((n /= 10) > 0);
   for(i=j-1;i>=0;i--)
      s[j-1-i]=temp[i];
   s[j]='\0';
}

void error_exit(char *name)
{
   perror(name);
   exit(-1);
}

int get_datatime(char * psDateTime)
{
    long nSeconds;
    struct tm * pTM;
    
    time(&nSeconds);
    pTM = localtime(&nSeconds);

    /* 系统日期和时间,格式: yyyymmddHHMMSS */
    sprintf(psDateTime, "%04d-%02d-%02d %02d:%02d:%02d",
            pTM->tm_year + 1900, pTM->tm_mon + 1, pTM->tm_mday,
            pTM->tm_hour, pTM->tm_min, pTM->tm_sec);
            
    return 0;
}