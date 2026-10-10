#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "bill.h"
#include "data.h"
#include"card.h"

BILL_LIST_P BILL_LIST_HEAD_P = NULL;

void initBILL()
{
    freeBILL();
    BILL_LIST_HEAD_P = NULL;
}

void freeBILL() {
    BILL_LIST* p = BILL_LIST_HEAD_P;
    while (p != NULL) {
        BILL_LIST* temp = p;
        p = p->next;
        free(temp);
    }
    BILL_LIST_HEAD_P = NULL;
}

void bill_to_file() {
    FILE* bin_file = fopen("./data/bill_bin_file.dat", "wb");
    FILE* txt_file = fopen("./data/bill_txt_file.txt", "w");
    if (!bin_file || !txt_file) {
        printf("账单文件打开失败！\n");
        return;
    }

    BILL_LIST_P p = BILL_LIST_HEAD_P;
    while (p) {
        if (p->bill.delete_sign == 0) {
            fwrite(&(p->bill), sizeof(BILL), 1, bin_file);

            char startt_str[30], endt_str[30];
            strftime(startt_str, sizeof(startt_str), "%Y-%m-%d %H:%M", localtime(&p->bill.startt));
            strftime(endt_str, sizeof(endt_str), "%Y-%m-%d %H:%M", localtime(&p->bill.endt));
            fprintf(txt_file, "%s##%s##%s##%.2f##%d##%d\n\n",
                p->bill.aname,
                startt_str,
                endt_str,
                p->bill.comsume_amount,
                p->bill.consume_status,
                p->bill.delete_sign
            );
        }
        p = p->next;
    }
    fclose(bin_file);
    fclose(txt_file);
}

void file_to_bill() {
    freeBILL();

    FILE* temf = fopen("./data/bill_bin_file.dat", "rb");
    if (!temf) { return; }

    BILL temb;
    while (fread(&temb, sizeof(BILL), 1, temf) == 1)
    {
        BILL_LIST_P teml = (BILL_LIST*)malloc(sizeof(BILL_LIST));
        teml->bill = temb;
        teml->next = NULL;

        if (!BILL_LIST_HEAD_P) { BILL_LIST_HEAD_P = teml; }
        else {
            BILL_LIST_P p = BILL_LIST_HEAD_P;
            while (p->next) { p = p->next; }
            p->next = teml;
        }
    }
    fclose(temf);
}

void on_computer() {
    file_to_card();
    file_to_bill();
    if (CARD_LIST_HEAD_P == NULL) {
        printf("当前无卡信息！\n");
        system("pause");
        return;
    }

    printf("\n------------上机-------------\n");
    printf("请输入卡号(1~18位)：\n");
    char tem_num[CARD_NO_MAX_NUM];
    scanf("%19s", tem_num);
    if (strlen(tem_num) < 1 || strlen(tem_num) > 18)
    {
        printf("卡号长度必须为1~18位，请重试！\n"); system("pause"); return;
    }
    printf("请输入密码(1~8位)：\n");
    char tem_pwd[CARD_PWD_MAX_NUM];
    scanf("%9s", tem_pwd);
    if (strlen(tem_pwd) > 8 || strlen(tem_pwd) < 1)
    {
        printf("密码长度必须为1~8位，请重试！\n");system("pause"); return;
    }

    printf("上机信息如下：\n");
    CARD_LIST_P p = CARD_LIST_HEAD_P;
    int seeksign = 0;
    while (p != NULL)
    {
        if (p->card.delete_sign == 1) { p = p->next; continue; }
        if (strcmp(p->card.aname, tem_num) == 0 && strcmp(p->card.apwd, tem_pwd) != 0)
        {
            seeksign = 2;  break;
        }
        if (strcmp(p->card.aname, tem_num) == 0 && strcmp(p->card.apwd, tem_pwd) == 0)
        {
            if (p->card.nstatus == 1) { printf("该卡正在上机!\n"); system("pause"); return; }
            if (p->card.balance <= 0) { printf("该卡余额为空，请充值!\n"); system("pause"); return; }
            seeksign = 1;
            p->card.nstatus = 1;
            p->card.last_usage_time = time(NULL);

            BILL_LIST_P NEW_NODE = (BILL_LIST_P)malloc(sizeof(BILL_LIST));
            if (NEW_NODE == NULL) { printf("内存分配失败\n"); system("pause"); return; }
            memset(NEW_NODE, 0, sizeof(BILL_LIST));
            strcpy(NEW_NODE->bill.aname, p->card.aname);
            NEW_NODE->bill.startt = p->card.last_usage_time;
            NEW_NODE->bill.endt = 0;
            NEW_NODE->bill.comsume_amount = 0;
            NEW_NODE->bill.consume_status = 0;
            NEW_NODE->bill.delete_sign = 0;
            NEW_NODE->next = NULL;
            if (BILL_LIST_HEAD_P == NULL)
            {
                BILL_LIST_HEAD_P = NEW_NODE;
            }
            else
            {
                BILL_LIST_P temp = BILL_LIST_HEAD_P;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = NEW_NODE;
            }

            printf("上机成功！");
            printf("\n--------上机卡信息如下：-------\n");
            char tem_time_str[30];
            struct tm* tem_time = localtime(&p->card.last_usage_time);
            strftime(tem_time_str, sizeof(tem_time_str), "%Y-%m-%d %H.%M.%S", tem_time);
            printf("卡号\t\t余额\t上机时间\n");
            printf("%s\t\t%.2f\t%s\n",
                p->card.aname,
                p->card.balance,
                tem_time_str
            );
            break;
        }
        else { p = p->next; }
    }
    if (seeksign == 0) { printf("该卡不存在或已注销，无法操作！\n"); system("pause"); }
    else if (seeksign == 2) { printf("密码错误，请重试！\n"); system("pause"); }

    bill_to_file();
    card_to_file();
}

void off_computer() {
    file_to_card();
    file_to_bill();
    if (CARD_LIST_HEAD_P == NULL) {
        printf("当前无卡信息！\n");
        system("pause");
        return;
    }

    printf("\n------------下机-------------\n");
    printf("请输入卡号(1~18位)：\n");
    char tem_num[CARD_NO_MAX_NUM];
    scanf("%19s", tem_num);
    if (strlen(tem_num) < 1 || strlen(tem_num) > 18)
    {
        printf("卡号长度必须为1~18位，请重试！\n"); system("pause"); return;
    }
    printf("请输入密码(1~8位)：\n");
    char tem_pwd[CARD_PWD_MAX_NUM];
    scanf("%9s", tem_pwd);
    if (strlen(tem_pwd) > 8 || strlen(tem_pwd) < 1)
    {
        printf("密码长度必须为1~8位，请重试！\n"); system("pause"); return;
    }

    printf("下机信息如下：\n");
    CARD_LIST_P pc = CARD_LIST_HEAD_P;
    int seeksign = 0;

    while (pc != NULL)
    {
        if (pc->card.delete_sign == 1) { pc = pc->next; continue; }
        if (strcmp(pc->card.aname, tem_num) == 0 && strcmp(pc->card.apwd, tem_pwd) != 0)
        {
            seeksign = 2;  break;
        }
        if (strcmp(pc->card.aname, tem_num) == 0 && strcmp(pc->card.apwd, tem_pwd) == 0)
        {
            BILL_LIST_P pb = BILL_LIST_HEAD_P;
            while (pb != NULL)
            {
                if (strcmp(pc->card.aname, pb->bill.aname) == 0 && pb->bill.consume_status == 0) { break; }
                else { pb = pb->next; }
            }
            if (pb == NULL) {
                printf("未找到该卡的上机账单！\n");
                system("pause");
                return;
            }
            if (pc->card.nstatus == 0) { printf("该卡尚未上机，请重试！\n"); system("pause"); return; }
            seeksign = 1;
            time_t startt = pc->card.last_usage_time;
            pb->bill.endt = time(NULL);
            pb->bill.comsume_amount = difftime(pb->bill.endt, startt) * 0.1;
            pb->bill.consume_status = 1;
            pc->card.balance = pc->card.balance - pb->bill.comsume_amount;
            if (pc->card.balance <= 0)
            {
                float tem_money;
                printf("当前欠费%.2f元，请输入充值金额：", pc->card.balance);
                scanf("%f", &tem_money);
                pc->card.balance = pc->card.balance + tem_money;
                pc->card.total_money = pc->card.total_money + tem_money;
                printf("充值成功！\n");
                system("pause");

            }
            pc->card.nstatus = 0;
            pc->card.last_usage_time = time(NULL);
            ++pc->card.usage_count;

            char start_time_str[30] = { 0 };
            struct tm* start_tm = localtime(&pb->bill.startt);
            strftime(start_time_str, sizeof(start_time_str), "%Y-%m-%d %H.%M.%S", start_tm);

            char end_time_str[30] = { 0 };
            struct tm* end_tm = localtime(&pb->bill.endt);
            strftime(end_time_str, sizeof(end_time_str), "%Y-%m-%d %H.%M.%S", end_tm);
            printf("\n-------下机卡信息如下：-------\n");
            printf("%-8s%-8s%-8s%-24s%s\n", "卡号", "消费", "余额", "上机时间", "下机时间");
            printf("%-8s%-8.1f%-8.1f%-24s%s\n",
                pb->bill.aname,
                pb->bill.comsume_amount,
                pc->card.balance,
                start_time_str,
                end_time_str);
            system("pause");
            break;
        }
        else { pc = pc->next; }
    }
    if (seeksign == 0) { printf("该卡不存在或已注销，无法操作！\n"); }
    else if (seeksign == 2) { printf("密码错误，请重试！\n"); }

    bill_to_file();
    card_to_file();
}

void recharge() {
    file_to_card();

    if (CARD_LIST_HEAD_P == NULL) {
        printf("当前无卡信息！\n");
        system("pause");
        return;
    }

    printf("\n------------充值-------------\n");
    printf("请输入卡号(1~18位)：\n");
    char tem_num[CARD_NO_MAX_NUM];
    scanf("%19s", tem_num);
    if (strlen(tem_num) < 1 || strlen(tem_num) > 18)
    {
        printf("卡号长度必须为1~18位，请重试！\n"); system("pause"); return;
    }
    printf("请输入密码(1~8位)：\n");
    char tem_pwd[CARD_PWD_MAX_NUM];
    scanf("%9s", tem_pwd);
    if (strlen(tem_pwd) > 8 || strlen(tem_pwd) < 1)
    {
        printf("密码长度必须为1~8位，请重试！\n"); system("pause"); return;
    }

    CARD_LIST_P pc = CARD_LIST_HEAD_P;
    int seeksign = 0;

    while (pc != NULL)
    {
        if (pc->card.delete_sign == 1) { pc = pc->next; continue; }
        else if (strcmp(pc->card.aname, tem_num) == 0 && strcmp(pc->card.apwd, tem_pwd) != 0)
        {
            seeksign = 2;  break;
        }
        else if (strcmp(pc->card.aname, tem_num) == 0 && strcmp(pc->card.apwd, tem_pwd) == 0)
        {
            float tem_money = 0;
            printf("请输入要充值的金额:\n");
            scanf("%f", &tem_money);
            pc->card.balance = pc->card.balance + tem_money;
            pc->card.total_money = pc->card.total_money + tem_money;
            printf("充值成功!\n");
            system("pause");
            break;
        }
        else { pc = pc->next; }
    }
    if (seeksign == 0) { printf("该卡不存在或已注销，无法操作！\n"); }
    else if (seeksign == 2) { printf("密码错误，请重试！\n"); }

    card_to_file();
}

void refund() {
    file_to_card();

    if (CARD_LIST_HEAD_P == NULL) {
        printf("当前无卡信息！\n");
        system("pause");
        return;
    }

    printf("\n------------退费-------------\n");
    printf("请输入卡号(1~18位)：\n");
    char tem_num[CARD_NO_MAX_NUM];
    scanf("%19s", tem_num);
    if (strlen(tem_num) < 1 || strlen(tem_num) > 18)
    {
        printf("卡号长度必须为1~18位，请重试！\n"); system("pause"); return;
    }
    printf("请输入密码(1~8位)：\n");
    char tem_pwd[CARD_PWD_MAX_NUM];
    scanf("%9s", tem_pwd);
    if (strlen(tem_pwd) > 8 || strlen(tem_pwd) < 1)
    {
        printf("密码长度必须为1~8位，请重试！\n"); system("pause"); return;
    }

    CARD_LIST_P pc = CARD_LIST_HEAD_P;
    int seeksign = 0;

    while (pc != NULL)
    {
        if (pc->card.delete_sign == 1) { pc = pc->next; continue; }
        else if (strcmp(pc->card.aname, tem_num) == 0 && strcmp(pc->card.apwd, tem_pwd) != 0)
        {
            seeksign = 2;  break;
        }
        else if (strcmp(pc->card.aname, tem_num) == 0 && strcmp(pc->card.apwd, tem_pwd) == 0 && pc->card.nstatus == 0)
        {
            float tem_money;
            if (pc->card.balance <= 0) { printf("卡内已经欠费，无法退费，请充值！\n"); break; }
            printf("当前可退余额为:%.2f\n", pc->card.balance);
            printf("请输入要退费的金额:");
            scanf("%f", &tem_money);
            if (pc->card.balance < tem_money) { seeksign = 1; printf("卡内余额不足，请重试！\n"); system("pause"); }
            else { pc->card.balance = pc->card.balance - tem_money; seeksign = 1; printf("退费完成！\n"); system("pause"); }
            break;
        }
        else if (strcmp(pc->card.aname, tem_num) == 0 && strcmp(pc->card.apwd, tem_pwd) == 0 && pc->card.nstatus == 1)
        {
            seeksign = 1;  printf("该卡正在上机，请先下机后再退费！\n"); break;
        }
        else { pc = pc->next; }
    }
    if (seeksign == 0) { printf("该卡不存在或已注销，无法操作！\n"); }
    else if (seeksign == 2) { printf("密码错误，请重试！\n"); }


    card_to_file();
}

void statistics() {
    printf("查询统计\n");
}
