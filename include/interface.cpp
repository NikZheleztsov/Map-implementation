#include <iostream>
#include <string>
#include "tree.h"

int main (int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Missing operand\n";
        return 1;
    }

    std::string cmd = {""};

    if (static_cast<std::string>(argv[1]) == "-i") //int
    {
        Tree<int, int> first_tr;

        while (cmd != "q" && cmd != "quit")
        {
            std::cout << "\033[0;96mCommand (int): \033[0m";
            std::cin >> cmd;

            if (cmd == "push") // push "value"
            {
                int val, key;
                std::cin >> key >> val;
                first_tr.push(std::make_pair(key, val));
                std::cin.ignore(1000, '\n');

            } else if (cmd == "pop") // pop "value"
            {
                if (first_tr.get_root() != nullptr) 
                {
                    int key;
                    std::cin >> key;
                    std::cin.ignore(1000, '\n');
                    first_tr.del(key);

                } else 
                    std::cout << "Empty tree\n";

            } else if (cmd == "print")
            {
                // first_tr.print ();

            } else if (cmd == "destruct")
            {

            } else if (cmd == "q" || cmd == "quit")
                std::cout << "Quitting...\n";
        }

    } else if (static_cast<std::string>(argv[1]) == "-p") //pair
    {

    } else 
        std::cout << "Unknown operand\n";
}

