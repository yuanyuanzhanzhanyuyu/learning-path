#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "card.h"
#include "data.h"
#include "bill.h"
#include<time.h>

//全局卡文件头指针
CARD_LIST_P CARD_LIST_HEAD_P = NULL;
//卡链表初始化
void initCARD()
{
    freeCARD();
    CARD_LIST_HEAD_P = NULL;
}
//卡链表释放
void freeCARD() {
    CARD_LIST* p = CARD_LIST_HEAD_P;
    while (p != NULL) {
        CARD_LIST* temp = p;
        p = p->next;
        free(temp);
    }
    CARD_LIST_HEAD_P = NULL;
}

void file_to_card() {
    freeCARD();

    FILE* temf = fopen("./data/card_bin_file.dat", "rb");
    if (!temf) {return;}

    CARD temc;
    while (fread(&temc, sizeof(CARD), 1, temf) == 1)
    {
        CARD_LIST_P teml = (CARD_LIST*)malloc(sizeof(CARD_LIST));
        teml->card = temc;
        teml->next = NULL;

        if (!CARD_LIST_HEAD_P){CARD_LIST_HEAD_P = teml;}
        else {
	  CARD_LIST_P p = CARD_LIST_HEAD_P;
	  while (p->next){p = p->next;}
	  p->next = teml;
        }
    }
    fclose(temf);
}

void card_to_file(){
    FILE* bin_file = fopen("./data/card_bin_file.dat","wb");
    FILE* txt_file = fopen("./data/card_txt_file.txt","w");
    if (!bin_file || !txt_file)
    {
        printf("卡文件打开失败！");
        return;
    }

    CARD_LIST_P p = CARD_LIST_HEAD_P;
    while (p)
    {
	  fwrite(&(p->card), sizeof(CARD), 1, bin_file);

	  char stime[30], etime[30], ltime[30];
	  strftime(stime, sizeof(stime), "%Y-%m-%d %H:%M", localtime(&p->card.startt));
	  strftime(etime, sizeof(etime), "%Y-%m-%d %H:%M", localtime(&p->card.endt));
	  strftime(ltime, sizeof(ltime), "%Y-%m-%d %H:%M", localtime(&p->card.last_usage_time));

	  fprintf(txt_file, "%s##%s##%d##%s##%s##%.2f##%s##%d##%.2f##%d\n\n",
	      p->card.aname,
	      p->card.apwd,
	      p->card.nstatus,
	      stime, etime,
	      p->card.total_money,
	      ltime,
	      p->card.usage_count,
	      p->card.balance,
	      p->card.delete_sign);
	  
	  p = p->next;
    }
    fclose(bin_file);
    fclose(txt_file);
    }

void add_card() {

    file_to_card();

    CARD_LIST_P NEW_NODE = (CARD_LIST_P)malloc(sizeof(CARD_LIST));
    if (NEW_NODE == NULL) { printf("链表创建失败\n"); system("pause"); return; }
    memset(NEW_NODE, 0, sizeof(CARD_LIST));
    NEW_NODE->next = NULL;

    char temp_aname[CARD_NO_MAX_NUM];
    char temp_apwd[CARD_PWD_MAX_NUM];
    float temp_balance;

    printf("\n------------添加卡------------\n");
    printf("请输入卡号(长度为0~18)：");
    scanf("%19s", temp_aname);
    if (strlen(temp_aname) > 18 || strlen(temp_aname) < 1)
    {
        printf("卡号长度必须为1~18位，添加失败！\n"); free(NEW_NODE); system("pause"); return;
    }
    CARD_LIST* p = CARD_LIST_HEAD_P;
    int seeksign = 0;
    while (p != NULL)
    {
        if (p->card.delete_sign == 0 && strcmp(p->card.aname, temp_aname) == 0)
        {
	  seeksign = 1;
	  break;
        }
        p = p->next;
    }
    if (seeksign)
    {
        printf("卡号已存在，添加失败\n"); free(NEW_NODE); system("pause"); return;
    }
    printf("请输入密码(长度为1~8)：");
    scanf("%9s", temp_apwd);
    if (strlen(temp_apwd) > 8 || strlen(temp_apwd) < 1)
    {
        printf("密码长度必须为1~8位，添加失败！\n"); free(NEW_NODE); system("pause"); return;
    }
    printf("请输入开卡金额(元)：");
    scanf("%f", &temp_balance);
    if (temp_balance <= 0.0)
    {
        printf("开卡金额必须为正数，添加失败！\n"); free(NEW_NODE); system("pause"); return;
    }
    strcpy(NEW_NODE->card.aname, temp_aname);
    strcpy(NEW_NODE->card.apwd, temp_apwd);
    NEW_NODE->card.nstatus = 0;
    NEW_NODE->card.startt = time(NULL);
    NEW_NODE->card.endt = time(NULL) + 365 * 24 * 3600;
    NEW_NODE->card.total_money = temp_balance;
    NEW_NODE->card.last_usage_time = time(NULL);
    NEW_NODE->card.usage_count = 0;
    NEW_NODE->card.balance = temp_balance;
    NEW_NODE->card.delete_sign = 0;

    if (CARD_LIST_HEAD_P == NULL) { CARD_LIST_HEAD_P = NEW_NODE; }
    else {
        CARD_LIST* temp = CARD_LIST_HEAD_P;
        while (temp->next != NULL)
        {
	  temp = temp->next;
        }
        temp->next = NEW_NODE;
    }

    printf("卡添加成功！\n");
    printf("\n-------添加卡信息如下-------\n");
    printf("卡号\t密码\t\t状态\t余额\n");
    printf("%s\t%s\t\t%d\t%.2f\n",
        NEW_NODE->card.aname,
        NEW_NODE->card.apwd,
        NEW_NODE->card.nstatus,
        temp_balance);

        card_to_file();
}

//链表精准查找
void seek_card_list_exact() {

    file_to_card();

    if (CARD_LIST_HEAD_P == NULL) {
        printf("当前无卡信息！\n");
        system("pause");
        return;
    }

    printf("\n------------精确查询------------\n");
    printf("请输入要查询的卡的卡号(1~18位)：\n");
    char seek_aname[CARD_NO_MAX_NUM];
    scanf("%19s", seek_aname);
    if (strlen(seek_aname) < 1 || strlen(seek_aname) > 18)
    {
        printf("卡号长度必须为1~18位，查询失败！\n"); system("pause"); return;
    }

    CARD_LIST* p = CARD_LIST_HEAD_P;
    int seeksign = 0;
    printf("\n查询结果：\n");
    while (p != NULL)
    {
        if (p->card.delete_sign == 1) { p = p->next; continue; }

        if (strcmp(seek_aname, p->card.aname) == 0)
        {
	  seeksign = 1;

	  char start_time_str[30] = { 0 };
	  struct tm* start_tm = localtime(&p->card.startt);
	  strftime(start_time_str, sizeof(start_time_str), "%Y-%m-%d %H.%M.%S", start_tm);

	  char last_usage_str[30] = { 0 };
	  struct tm* last_tm = localtime(&p->card.last_usage_time);
	  strftime(last_usage_str, sizeof(last_usage_str), "%Y-%m-%d %H.%M.%S", last_tm);
	  printf("卡号\t状态\t余额\t累计金额\t使用次数\t开卡时间\t\t上次使用时间\n");
	  printf("%-8s%-8d%-8.2f%-16.2f%-16d%-24s%-20s\n\n",
	      p->card.aname,
	      p->card.nstatus,
	      p->card.balance,
	      p->card.total_money,
	      p->card.usage_count,
	      start_time_str,
	      last_usage_str);
        }
        p = p->next;
    }
    if (seeksign == 0) { printf("未找到该卡号信息！\n"); }
    system("pause");
 }

//链表模糊查找
void seek_card_list_fuzzy() {

    file_to_card();

    if (CARD_LIST_HEAD_P == NULL) {
        printf("当前无卡信息！\n");
        system("pause");
        return;
    }

    printf("\n------------模糊查询-------------\n");
    printf("请输入要查询的卡的卡号(1~18位)：\n");
    char seek_aname[CARD_NO_MAX_NUM];
    scanf("%19s", seek_aname);
    if (strlen(seek_aname) < 1 || strlen(seek_aname) > 18)
    {
        printf("卡号长度必须为1~18位，查询失败！\n"); system("pause"); return;
    }

    CARD_LIST* p = CARD_LIST_HEAD_P;
    int seeksign = 0;
    printf("\n查询结果：\n");
    while (p != NULL)
    {
        if (p->card.delete_sign == 1) { p = p->next; continue; }

        if (strstr(p->card.aname,seek_aname) != NULL)
        {
	  seeksign = 1;

	  char start_time_str[30] = { 0 };
	  struct tm* start_tm = localtime(&p->card.startt);
	  strftime(start_time_str, sizeof(start_time_str), "%Y-%m-%d %H.%M.%S", start_tm);

	  char last_usage_str[30] = { 0 };
	  struct tm* last_tm = localtime(&p->card.last_usage_time);
	  strftime(last_usage_str, sizeof(last_usage_str), "%Y-%m-%d %H.%M.%S", last_tm);
	  printf("卡号\t状态\t余额\t累计金额\t使用次数\t开卡时间\t\t上次使用时间\n");
	  printf("%-8s%-8d%-8.2f%-16.2f%-16d%-24s%-20s\n\n\n",
	      p->card.aname,
	      p->card.nstatus,
	      p->card.balance,
	      p->card.total_money,
	      p->card.usage_count,
	      start_time_str,
	      last_usage_str);
        }
        p = p->next;
    }
    if (seeksign == 0) { printf("未找到该卡号信息！\n"); }
    card_to_file();
    system("pause");
}

void delete_card() {
    file_to_card();

    if (CARD_LIST_HEAD_P == NULL) {
        printf("当前无卡信息！\n");
        system("pause");
        return;
    }

    printf("\n------------注销-------------\n");
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
        if (pc->card.delete_sign == 1) {
	  pc = pc->next;
	  continue;
        }
        else if (strcmp(pc->card.aname, tem_num) == 0 && strcmp(pc->card.apwd, tem_pwd) != 0)
        {
	  seeksign = 2;  break;
        }
        else if (strcmp(pc->card.aname, tem_num) == 0 && strcmp(pc->card.apwd, tem_pwd) == 0 && pc->card.nstatus == 0)
        {
	  pc->card.delete_sign = 1; seeksign = 1;  printf("注销成功！\n"); system("pause"); break;
        }
        else if (strcmp(pc->card.aname, tem_num) == 0 && strcmp(pc->card.apwd, tem_pwd) == 0 && pc->card.nstatus == 1)
        {
	  seeksign = 1; printf("此卡正在上机，请下机后再来操作！\n"); system("pause"); break;
        }
        else { pc = pc->next; }
    }
    if (seeksign == 0) { printf("此卡不存在或已注销，无法操作！\n"); system("pause"); }
    else if (seeksign == 2) { printf("密码错误，请重试！\n"); system("pause");}

    card_to_file();
}
