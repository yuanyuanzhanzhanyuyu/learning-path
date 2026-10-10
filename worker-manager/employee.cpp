#include"employee.h"
#include<iostream>
#include<string>
using std::cin;
using std::cout;
using std::endl;

employee::employee(string a, string b, string c) {
   this->num = a;
   this->name = b;
   this->position = c;
}
void employee::show_information() {
    cout << "姓名：" << this->name << endl;
    cout << "职工编号：" << this->num << endl;
    cout << "岗位：" << this->get_position() << endl;
    cout << "岗位职责：完成经理交给的任务" << endl;
}

string employee::get_position() {
    return string("员工");
}
