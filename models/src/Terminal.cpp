#include "../include/Terminal.hpp"

Terminal::Terminal(int terminalAddress, int routerAddress)
{
    this->terminalAddress = terminalAddress;
    this->routerAddress = routerAddress;
}

int Terminal::getTerminalAddress() { return terminalAddress; }

int Terminal::getRouterAddress() { return routerAddress; }

void Terminal::sendPage() {}