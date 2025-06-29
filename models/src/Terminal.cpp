#include "../include/Terminal.hpp"
#include "../include/NetworkSimulator.hpp"
#include "../include/Page.hpp"
#include "../include/Router.hpp"
#include <iostream>
#include <math.h>
#include <random>
#include <unistd.h>
#include <vector>

Terminal::Terminal(int terminalAddress, int routerAddress, NetworkSimulator* networkSimulator)
{
    this->terminalAddress = terminalAddress;
    this->routerAddress = routerAddress;
    this->networkSimulator = networkSimulator;
}

int Terminal::getTerminalAddress()
{
    return terminalAddress;
}

int Terminal::getRouterAddress()
{
    return routerAddress;
}

void Terminal::setTerminalNodes(std::vector<Terminal> terminalNodes)
{
    this->terminalNodes = terminalNodes;
}

void Terminal::sendPage()
{
    Page* page = generatePage();
    std::cout << "Terminal " << terminalAddress << " sending page with ID " << page->getIdPage() << " of size "
              << page->getSizePage() << " bytes to terminal " << page->getDestTerminalAddress() << std::endl;

    Router* router = networkSimulator->getRouterByAddress(routerAddress);

    router->receivePage(page);
}

Page* Terminal::generatePage()
{
    static int idPage = 0;
    int sizePage = std::rand() % 10240 + 256;

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

void Terminal::receivePage(Page* page)
{
    // This method should handle the reception of a page
    std::cout << "Terminal " << terminalAddress << " received page with ID " << page->getIdPage() << " of size "
              << page->getSizePage() << " bytes from terminal " << page->getOrigTerminalAddress() << std::endl;
}
