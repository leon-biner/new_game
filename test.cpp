#include <iostream>
#include <ncurses.h>
#include <chrono>
#include <vector>
#include "main_header.hpp"

int main()
{
    pair<int> paire(5, 5);
    pair<int> paire_2(4, 3);
    pair<int> paire_3 = paire - paire_2;
    std::cout << paire_3.x_ << paire_3.y_ << std::endl;
}