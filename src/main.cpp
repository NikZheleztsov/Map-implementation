#include "map.h"
#include <iostream>
#include <algorithm>

template <class Key, class T, class Compare, class Allocator>
void print (Map<Key,T,Compare,Allocator>& map)
{
    for (auto x : map)
        std::cout << '{' << x.first << ", " << x.second << '}' << "  ";
    std::cout << std::endl;
}

int main ()
{
    // constructors
    std::cout << "============= Constructors =============" << std::endl << std::endl;
    Map<int, std::string> map1;
    Map<int, std::string> map2({{10, "str10"}, {3, "str3"}, {5, "str5"}});
    std::cout << "Map2: ";
    print(map2);

    Map map_for_del(map2);
    std::cout << "Map_for_del:  ";
    print(map_for_del);

    Map mv_map(move(map_for_del));
    std::cout << "Map_for_del (moved):  ";
    print(map_for_del);
    std::cout << "Mv_map: ";
    print(mv_map);

    Map<int, std::string> it_map (map2.begin(), map2.end());
    std::cout << "It_map: ";
    print(it_map);

    std::cout << std::endl << "Map1(after =): ";
    map1 = mv_map;
    print(map1);

    std::cout << std::endl << "============= Element access =============" << std::endl << std::endl;
    std::cout << "Map1.at(10)): ";
    std::cout << map1.at(10) << std::endl;
    map1.at(10) = "str10_cp";
    std::cout << "Map1.at(10)='str10_cp': " << map1.at(10) << std::endl;

    std::cout << "Map1[3]: " << map1[3] << std::endl;
    map1[7] = "str7";
    std::cout << "Map1 (after map1[7]): ";
    print(map1);

    std::cout << std::endl << "============= Iterators =============" << std::endl << std::endl;
    for (auto i = mv_map.rbegin(); i != mv_map.rend(); i++)
    {
        i.second() = "mv_str";
        std::cout << '{' << i.first() << ", " << i.second() << '}' << "  ";
    }
    std::cout << std::endl;

    std::cout << std::endl << "============= Capacity =============" << std::endl << std::endl;
    if (!map1.empty())
    {
        std::cout << "Map1.size: " << map1.size() << std::endl;
        std::cout << "Map1.max_size: " << map1.max_size() << std::endl;
    }

    std::cout << std::endl << "============= Modifiers =============" << std::endl << std::endl;
    mv_map.clear();
    std::cout << "Mv.map.size() (after clear): " << mv_map.size() << std::endl;
    std::cout << std::endl;

    map1.insert(std::make_pair(0, "str0"));
    std::cout << "Map1.insert(0): ";
    print(map1);

    mv_map.insert(map1.begin(), map1.find(5));
    std::cout << "Mv_map(after insert): ";
    print(mv_map);

    mv_map.insert({{2, "str2"}, {10, "str10"}});
    print(mv_map);

    std::cout << std::endl;
    mv_map.erase(mv_map.begin());
    mv_map.erase(3);
    std::cout << "Mv_map(after erase): ";
    print(mv_map);
    map1.erase(map1.begin(), map1.find(7));
    std::cout << "Map1(after erase): ";
    print(map1);

    std::cout << std::endl;
    map1.swap(mv_map);
    std::cout << "Map1(after swap with mv_map): ";
    print(map1);
    std::cout << "Mv_map(after swap with map1): ";
    print(mv_map);

    auto n1 = it_map.extract(it_map.begin());
    auto n2 = it_map.extract(10);
    std::cout << "First node from it_map: " << n1.first << ' ' << n1.second << std::endl;
    std::cout << "10 node from it_map: " << n2.first << ' ' << n2.second << std::endl;
    std::cout << "It_map: ";
    print(it_map);

    std::cout << std::endl;
    map1.merge(mv_map);
    std::cout << "Mv_map(after merge): ";
    print(mv_map);
    std::cout << "Map1(after merge): ";
    print(map1);

    std::cout << std::endl;
    std::cout << "Count mv_map 10: " << mv_map.count(10) << std::endl;
    std::cout << "Count mv_map 7: " << mv_map.count(7) << std::endl;

    std::cout << "STL: \n";
    std::for_each(map1.begin(), map1.end(), [] (auto x) { std::cout << x.first; });
    std::cout << std::endl;
}
