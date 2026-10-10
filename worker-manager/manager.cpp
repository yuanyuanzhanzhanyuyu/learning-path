#include"manager.h"
#include<iostream>
#include<string>
using std::cin;
using std::cout;
using std::endl;

manager::manager(string a, string b, string c) {
    this->num = a;
    this->name = b;
    this->position = c;
}
void manager::show_information() {
    cout << "姓名：" << this->name << endl;
    cout << "职工编号：" << this->num << endl;
    cout << "岗位：" << this->get_position() << endl;
    cout << "岗位职责：给员工布置任务，完成老板交给的任务" << endl;
}

string manager::get_position() {
    return string("经理");
}