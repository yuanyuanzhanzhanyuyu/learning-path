#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "menu.h"
#include"bill.h"
#include"card.h"
#include "data.h"

void show_mainmenu() {
    while (1) {
        printf("\n------------------------------\n");
        printf("校园网费管理系统 v1.0\n");
        printf("-------------菜单-------------\n");
        printf("1.添加卡\n");
        printf("2.查询卡\n");
        printf("3.上机\n");
        printf("4.下机\n");
        printf("5.充值\n");
        printf("6.退费\n");
        printf("7.查询统计\n");
        printf("8.注销卡\n");
        printf("0.退出\n");
        printf("请选择菜单选项(0~8):\n");
        printf("------------------------------\n");

        char choice[10];
        scanf("%9s", choice);
        if (strlen(choice) != 1) {
            printf("输入选项错误，请重新输入\n\n");
            continue;
        }

        switch (choice[0]){
        case '1':
            add_card();
            break;
        case '2':
            //此处进行查找函数接口的调换
            seek_card_list_fuzzy();
            //seek_card_list_exact();
            break;
        case '3':
            on_computer();
            break;
        case '4':
            off_computer();
            break;
        case '5':
            recharge();
            break;
        case '6':
            refund();
            break;
        case '7':
            statistics();
            break;
        case '8':
            delete_card();
            break;
        case '0':
            return; 
        default:
            printf("输入选项错误，请重新选择\n\n");
            break;
        }
    }
}
