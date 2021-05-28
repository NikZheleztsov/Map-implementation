#include "map.h"
#include <iostream>

int main ()
{
    Map<int, std::string, std::less<int>> map;
    Map<int, std::string> map2;
    Map<int, std::string> map4 
        ({ {3, "str3"}, {5, "str"}, {1, "str1"}});
    Map<int, std::string> map3 (map4);
    std::cout << map4.at(3) << std::endl;
    std::cout << map4.begin().first()
        << ' ' << map4.begin().second() << std::endl;

    map2.insert(map4.begin(),map4.end());

    std::cout << std::endl;
    for (auto x : map2)
        std::cout << x.first << ' ' << x.second << std::endl;
    map2.insert(std::make_pair(-1, "str-1"));
    std::cout << map2.begin().second() << std::endl;

    map.insert({{23, "str23"}, {10, "str10"}});
    map.erase(map.begin());
    for (auto x : map)
        std::cout << x.first << ' ' << x.second << "  ";

    std::cout << std::endl << std::endl;
    for (auto x : map2)
        std::cout << x.first << ' ' << x.second << "  ";
    std::cout << std::endl;

    auto beg = map2.begin();
    auto end = map2.find(3);;
    map2.erase(beg, end);

    for (auto x : map2)
        std::cout << x.first << ' ' << x.second << "  ";

    map2.erase(5);
    std::cout << std::endl << "Map2: ";
    for (auto x : map2)
        std::cout << x.first << ' ' << x.second << "  ";

    std::cout << std::endl << "Map3: ";
    for (auto x : map3)
        std::cout << x.first << ' ' << x.second << "  ";

    map2.swap(map3);
    std::cout << std::endl << "Map3: ";
    for (auto x : map3)
        std::cout << x.first << ' ' << x.second << "  ";

    std::cout << std::endl << "Map2: ";
    for (auto x : map2)
        std::cout << x.first << ' ' << x.second << "  ";

    auto nh = map2.extract(1);
    nh.first = 2;

    std::cout << std::endl << "Map2: ";
    for (auto x : map2)
        std::cout << x.first << ' ' << x.second << "  ";

    map3.insert({10, "str10"});
    map2.merge(map3);

    std::cout << "Map2.merge(map3)";
    std::cout << std::endl << "Map3: ";
    for (auto x : map3)
        std::cout << x.first << ' ' << x.second << "  ";

    std::cout << std::endl << "Map2: ";
    for (auto x : map2)
        std::cout << x.first << ' ' << x.second << "  ";
}
