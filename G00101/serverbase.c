
#include "serverbase.h"
#include "tools.h"

int gen_send_info(char toSend[DIALOG_STAGE_NUM][MAX_SEND_LEN],dialog *dia)
{
   strcpy(toSend[0],"StuNo");
   strcpy(toSend[1],"pid");
   strcpy(toSend[2],"TIME");
   strcpy(toSend[3],"str");
   strcpy(toSend[4],"end");

   char s_rand[10];
   srand(getpid()+time(0));
   int i_rand=rangerand(32768,99999);
   myitoa(i_rand,s_rand);
   strcat(toSend[3],s_rand);

   dia->Randstr_len=i_rand;
   return 0;
}