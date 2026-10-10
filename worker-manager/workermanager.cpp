#include<iostream>
#include<fstream>
#include <cstdlib>
#include"workermanager.h"
#include"worker.h"
#include"employee.h"
#include"boss.h"
#include"manager.h"
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::ios;


workermanager::workermanager() {
    ifstream ifs;
    ifs.open(FILENAME,ios::in);

    //首次使用文件还未创建的处理方法
        if (!ifs.is_open())
        {
        cout << "文件不存在！" << endl;
        this->employee_num = 0;
        this->worker_arr = NULL;
        this->is_file_empty = 1;
        ifs.close();
        return;
        }
    //文件存在 员工数据为空
        char tem1;
        ifs >> tem1;
        if (ifs.eof() == 1) 
        {
        cout << "文件为空" << endl;
        this->employee_num = 0;
        this->worker_arr = NULL;
        this->is_file_empty = 1;
        ifs.close();
        return;
        }
    //文件存在 并且员工数据不为空
        int num = get_num();
        cout << "目前职工人数为：" << num << endl;
        
        this->employee_num = num;
        this->worker_arr = new worker * [this->employee_num];
        this->init_emp();

        /*测试初始化函数
        for (int i = 0; i < this->employee_num; i++)
        {
	  cout << this->worker_arr[i]->name << endl;
	  cout << this->worker_arr[i]->num << endl;
	  cout << this->worker_arr[i]->position << endl;
        }
        */

}

workermanager::~workermanager() {
    if (this->worker_arr != NULL)
    {
        delete[] this->worker_arr;
        this->worker_arr = NULL;
    }
}

//展示菜单
void workermanager::show_menu() {
    cout << "--------------------" << endl;
    cout << "欢迎使用职工管理系统" << endl;
    cout << "---0.退出管理程序---" << endl;
    cout << "---1.增加职工信息---" << endl;
    cout << "---2.显示职工信息---" << endl;
    cout << "---3.删除职工信息---" << endl;
    cout << "---4.修改职工信息---" << endl;
    cout << "---5.查找职工信息---" << endl;
    cout << "---6.按照编号排序---" << endl;
    cout << "---7.清空所有文档---" << endl;
    cout << "--------------------" << endl;
    cout << "请输入选项：" << endl;

    string a;
    cin >> a;
    if (a == "0") { cout << "已退出，欢迎下次使用" << endl;  return; }
    else if (a == "1") {
        add_worker(); show_menu();
    }
    else if (a == "2") {}
    else if (a == "3") {}
    else if (a == "4") {}
    else if (a == "5") {}
    else if (a == "6") {}
    else if (a == "7"
        ) {}
    else {
        cout << "请输入有效指令" << endl;  show_menu();
    }
}

//添加职工
void workermanager::add_worker() {
    int a = 0;
    cout << "请输入要添加的员工个数：" << endl;
    cin >> a;

    if (a > 0)
    {
        //更新空间大小
        int new_size = a + employee_num;
        //开辟新空间
        worker** new_space = new worker * [new_size];
        //将原空间内容引进/从零开始创建
        if (this->worker_arr != NULL)
        {
	  for (int i = 0; i < this->employee_num; ++i)
	  {
	      new_space[i] = this->worker_arr[i];
	  }
        }
        else
        {
	  for (int i = 0; i < a; ++i)
	  {
	      string id;
	      string name;
	      int decision;


	      cout << "请输入第" << i + 1 << "位职工编号：" << endl;
	      cin >> id;
	      cout << "请输入第" << i + 1 << "位职工姓名：" << endl;
	      cin >> name;

	      worker* worker = NULL;
	      int sign1 = 1;
	      while (sign1) {
		cout << "请选择第" << i + 1 << "位职工职位：" << endl;
		cout << "1.员工  2.经理  3.老板" << endl;
		cin >> decision;
		switch (decision) {
		case 1:
		    worker = new employee(id, name, "员工");
		    sign1 = 0;
		    break;
		case 2:
		    worker = new manager(id, name, "经理");
		    sign1 = 0;
		    break;
		case 3:
		    worker = new boss(id, name, "老板");
		    sign1 = 0;
		    break;
		default:
		    cout << "输入有误！请输入正确的数字选项！" << endl;
		    
		}
	      }
	      new_space[i + this->employee_num] = worker;
	  }
        }
        delete[] this->worker_arr;
        this->worker_arr = new_space;
        this->employee_num = new_size;
        cout << "成功添加" << a << "名员工" << endl;
        //更改文件不为空的标志
        this->is_file_empty = 0;

        this->save();
    }
    else {
        cout << "输入有误，请重新输入" << endl; 
    }
    system("pause");
    system("cls");
}

//保存文件
void workermanager::save() {
    ofstream ofs;
    ofs.open(FILENAME, ios::out);
    for (int i = 0; i < this->employee_num; i++){
        ofs << worker_arr[i]->num << " " 
	  << worker_arr[i]->name << " " 
	  << worker_arr[i]->position << endl;
        
    }
    ofs.close();
}

//统计人数
int workermanager::get_num() {
    ifstream ifs;
    ifs.open(FILENAME,ios::in);
    int num1 = 0;
    string id,name,dir;
    while (ifs >> id >> name >> dir)
    {
        num1++;
    }
    ifs.close();
    return num1;
}

//初始化员工
void workermanager::init_emp() {
    ifstream ifs;
    ifs.open(FILENAME, ios::in);

    int index = 0;
    string id, name, dir;
    while (ifs >> id && ifs >> name && ifs >> dir)
    {
        worker* worker = NULL;
        if(dir == "员工")
        {
	  worker = new employee(id, name, "员工");
        }
        else if (dir == "经理")
        {
	  worker = new manager(id, name, "经理");
        }
        else
        {
	  worker = new boss(id, name, "老板");
        }
        worker_arr[index] = worker;
        index++;
    }
    ifs.close();
}

int main(void) {

    workermanager test01;
    test01.show_menu();
    system("pause");
    return 0;
}



