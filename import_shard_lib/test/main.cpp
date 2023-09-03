#include <iostream>
#include "calc.h"

int main()
{
    std::cout<<"this is test app"<<std::endl;
    add_func(7,999);
    reduce_func(34124,2);
    std::string command="";
    std::cin>>command;
    return 0;
}