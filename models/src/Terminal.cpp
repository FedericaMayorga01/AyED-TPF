#include "../include/Terminal.hpp"
#include "../include/NetworkSimulator.hpp"
#include "../include/Page.hpp"
#include "../include/Router.hpp"
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <vector>

Terminal::Terminal(const int terminalAddress, const int routerAddress, NetworkSimulator* networkSimulator)
{
    this->terminalAddress = terminalAddress;
    this->routerAddress = routerAddress;
    this->networkSimulator = networkSimulator;
    this->terminalNodes = std::vector<Terminal>();
}

int Terminal::getTerminalAddress() const
{
    return terminalAddress;
}

int Terminal::getRouterAddress() const
{
    return routerAddress;
}

void Terminal::addTerminalNode(const Terminal& terminalNode)
{
    this->terminalNodes.emplace_back(terminalNode);
}

void Terminal::sendPage()
{
    const Page* page = generatePage();
    std::cout << "\033[1m\033[33mSend Page: \033[0m" <<"Terminal " << page->getOrigTerminalAddress() << " sending page with ID " << page->getIdPage() << " of size "
              << page->getSizePage() << " bytes to terminal " << page->getDestTerminalAddress() << std::endl;

    Router* router = networkSimulator->getRouterByAddress(routerAddress);

    router->receivePage(page);
}

Page* Terminal::generatePage()
{
    static int idPage = 1;
    const int sizePage = std::rand() % 10240 + 256;

    int randomIndex = std::rand() % terminalNodes.size();
    int destTerminalAddress = terminalNodes[randomIndex].getTerminalAddress();
    // Ensure the destination terminal is not the same as the source terminal
    if (destTerminalAddress == terminalAddress && terminalNodes.size() > 1)
    {
        randomIndex = std::rand() % terminalNodes.size();
        destTerminalAddress = terminalNodes[randomIndex].getTerminalAddress();
    }

    return new Page(idPage++, sizePage, terminalAddress, destTerminalAddress);
}

void Terminal::receivePage(const Page* page) const {
    // This method should handle the reception of a page
    std::cout << "Terminal " << terminalAddress << " received page with ID " << page->getIdPage() << " of size "
              << page->getSizePage() << " bytes from terminal " << page->getOrigTerminalAddress() << std::endl;
}
