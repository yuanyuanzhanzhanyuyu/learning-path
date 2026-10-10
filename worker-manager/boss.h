#pragma once
#include<iostream>
#include<string>
#include"worker.h"
using std::string;

class boss : public worker {
public:
    boss(string a, string b, string c);
    void show_information();
    string get_position();
};
