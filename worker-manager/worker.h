#pragma once
#include<iostream>
#include<string>

using std::string;

class worker {
public:
    virtual void show_information() = 0;
    virtual string get_position() = 0;
     string num;
     string name;
     string position;
};