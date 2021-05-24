#include "map.h"
#include <iostream>

int main ()
{
    Map<int, std::string> m;
    m.insert(std::make_pair(5, "str5"));
    m.insert(std::make_pair(1, "str1"));
    m.insert(std::make_pair(8, "str8"));
    m.insert(std::make_pair(23, "str23"));
    m.insert(std::make_pair(0, "str0"));

    for (auto x : m)
        std::cout << x.second << ' ';
}
