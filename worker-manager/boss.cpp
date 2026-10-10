#include"boss.h"
#include<iostream>
#include<string>
using std::cin;
using std::cout;
using std::endl;

boss::boss(string a, string b, string c) {
    this->num = a;
    this->name = b;
    this->position = c;
}
void boss::show_information() {
    cout << "姓名：" << this->name << endl;
    cout << "职工编号：" << this->num << endl;
    cout << "岗位：" << this->get_position() << endl;
    cout << "岗位职责：管理公司所有事务" << endl;
}

string boss::get_position() {
    return string("老板");
}
