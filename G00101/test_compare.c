#include "compare.h"

int main()
{
        if (compare_dirs("./","./")==CMP_IDENTICAL)
                printf("identical directory\n");
        else
                printf("different directory\n");
       
        return 0;
}
