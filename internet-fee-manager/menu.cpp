#include "menu.h"
#include "data.h"
#include "card.h"
#include<stdio.h>

FILE* tfile = NULL;
FILE* bfile = NULL;

int main(){
    initCARD();
    show_mainmenu();
    freeCARD();
    getchar();
return 0;}