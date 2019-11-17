#ifndef __SAVEDIALOG_H
#define __SAVEDIALOG_H
#include<stdio.h>
#include<string.h>
#include<stdint.h>
#endif

#ifndef __dialog_defined
typedef struct dialog
{
//    char* toSend[4];

    int StuNo;
    uint32_t pid;
    int Randstr_len;
    char TIME[20];
    char Randstr[100000];
}dialog;
#define __dialog_defined
#endif

#define TIME_LEN 19
#define MAX_SEND_LEN 10
#define DIALOG_STAGE_NUM 5
/*
把dialog的信息存到文件中
path：文件路劲 
dia：ptr->要保存的dialog
*/
int savedialog(dialog *dia);