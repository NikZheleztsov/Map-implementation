#include <map>
#include <iostream>

int main ()
{
    std::map<int, std::string> map { {1, "str"} };
    map.begin()->first = 2;
}
