#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#ifndef DATA_H
#define DATA_H
#define CARD_NO_MAX_NUM 100
#define CARD_PWD_MAX_NUM 100

typedef struct CARD
{
    char aname[19]; //卡号
    char apwd[8]; //密码
    int nstatus; //卡状态 1-上机 0-未上机
    time_t startt;//开始时间
    time_t endt;//结束时间
    float total_money;//累计金额
    time_t last_usage_time;//最后使用时间
    int usage_count;//使用次数
    float balance;//余额
    int delete_sign;//删除标志 0-未删 1-已删除
}CARD;

typedef struct BILL
{
    char aname[19]; // 卡号
    time_t startt;      // 上机时间
    time_t endt;        // 下机时间
    float comsume_amount;      // 消费金额
    int consume_status;        // 消费状态：0-未结算，1-已结算
    int delete_sign;           // 删除标识：0-未删除,1-已删除
}BILL;

typedef struct MONEY
{
    char aCardName[19];    // 卡号
    time_t in_out_time;      // 充值/退费的时间
    int in_out_status;  // 状态：0-表示充值，1-表示退费
    float in_out_amount;                  // 充值/退费的金额
    int delete_sign;                     // 删除标识：0-未删除,1-已删除
}MONEY;

typedef struct CARD_LIST
{
    CARD card;
    struct CARD_LIST *next;
}CARD_LIST,*CARD_LIST_P;
extern CARD_LIST_P CARD_LIST_HEAD_P;

typedef struct BILL_LIST
{
    BILL bill;
    struct BILL_LIST* next;
}BILL_LIST,*BILL_LIST_P;
extern BILL_LIST_P BILL_LIST_HEAD_P;

#endif
