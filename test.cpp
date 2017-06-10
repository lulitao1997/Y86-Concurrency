//
// Created by nn on 2017/6/8.
//

#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include "map_io.h"
using namespace std;

string str = "{\"apple\":\"1\",\"people\":\"sss\"}";
map<string, string> var;
char buf[1023];
int main() {
    sprintf(buf, "aaa %d", 123);
    sprintf(buf, "123123 sdfsfs %d", 123);
    printf("%s", buf);
}