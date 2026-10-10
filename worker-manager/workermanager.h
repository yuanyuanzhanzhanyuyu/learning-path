#pragma once //防止头文件重复包含
#include<iostream> //包含输入输出流头文件
#include<fstream>
#include"worker.h"
#include"employee.h"
#include"boss.h"
#include"manager.h"
#define FILENAME "employee_file.txt"

 
class workermanager {
  
public:
    //构造函数
    workermanager();
    //员工数量
    int employee_num; 
    //员工链表指针
    worker** worker_arr;
    //判断文件是否为空
    bool is_file_empty;
    

    
    
    //展示菜单
    void show_menu();
    //添加职工
    void add_worker();
    //保存文件
    void save();
    //统计员工数量
    int get_num();
    //初始化员工
    void init_emp();
    //析构函数
    ~workermanager();
};

