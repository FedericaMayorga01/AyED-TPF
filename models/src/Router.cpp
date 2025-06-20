#include "../include/Router.hpp"
#include "../include/Package.hpp"
#include "../include/Page.hpp"
#include <iostream>

Router::Router(int routerAddress, std::map<int, int> routingTable)
{
    this->routerAddress = routerAddress;
    this->routingTable = routingTable;
    this->amIEndNode = false; // TODO: check if in routing table there is an terminal address
}

int Router::getRouterAddress() const
{
    return routerAddress;
}

bool Router::getAmIEndNode() const
{
    return amIEndNode;
}

std::queue<Package> Router::splitPage(const Page &page)
{
    // This method should split a page into packages
    // for now it returns an empty queue
    std::queue<Package> emptyPackageQueue;
    return emptyPackageQueue;
}

void Router::sendPackage(int destAddress, const Package &package)
{
    // This method should send a package to the specified destination address
}

void Router::processQueues()
{
    // This method should process the queues of packages
}

void Router::updateRoutingTable(std::map<int, int> newRoutingTable)
{
    // This method should update the routing table with the new routing information
}

Page Router::rebuildPage()
{
    // This method should rebuild a page from the pending packages
    // for now it returns an empty Page
    Page emptyPage(0, 0, 0, 0);
    return emptyPage;
}

void Router::sendPage(int destTerminalAddress)
{
    // This method should send a page to the specified terminal address
}