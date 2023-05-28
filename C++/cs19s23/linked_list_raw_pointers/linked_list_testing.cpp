#include "cs19_linked_list_raw_pointers.h"
#include <iostream>

int main() {
    cs19::LinkedList<char> list;
    for (auto c : std::string("Hello"))
        list.push_back(c);
    std::cout << list << '\n';
    cs19::LinkedList<char> another;
    another = list;
    std::cout << another << '\n';
    another.remove('l');
    std::cout << another << '\n';
    another.pop_back();
    std::cout << another << '\n';
    another.pop_front();
    std::cout << another << '\n';
    std::cout << another.front() << '\n';
}