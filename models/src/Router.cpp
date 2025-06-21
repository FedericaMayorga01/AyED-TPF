#include "../include/Router.hpp"
#include "../include/Package.hpp"
#include "../include/Page.hpp"
#include <iostream>
#include <boost/circular_buffer.hpp>

Router::Router(int routerAddress, std::map<int, int> routingTable, int queueSize, int packageQuantity)
{
    this->packageQuantity = packageQuantity;
    this->routerAddress = routerAddress;
    this->routingTable = routingTable;
    this->amIEndNode = false; // TODO: check if in routing table there is an terminal address

    // Initialize package queues for each neighbor
    for (const auto &entry : routingTable)
    {
        int neighborAddress = entry.first;
        packageQueuesByNeighbor[neighborAddress] = boost::circular_buffer<Package>(queueSize);
    }
}

int Router::getRouterAddress() const
{
    return routerAddress;
}

bool Router::getAmIEndNode() const
{
    return amIEndNode;
}

std::list<Package> Router::splitPage(const Page page)
{
    // This method should split a page into packages
    // for now it returns an empty queue
    std::list<Package> emptyPackageList;
    return emptyPackageList;
}

void Router::sendPackage(int destAddress, const Package package)
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

bool Router::hasQueueFreeSpaceForPkg(int neighborAddress) const
{
    auto it = packageQueuesByNeighbor.find(neighborAddress);
    if (it != packageQueuesByNeighbor.end())
    {
        return it->second.size() < it->second.capacity();
    }
    return false;
}

bool Router::hasQueueFreeSpaceForPage(int neighborAddress, int pageSize) const
{
    auto it = packageQueuesByNeighbor.find(neighborAddress);
    int nPackages = pageSize / packageQuantity; // Assuming pageSize is divisible by packageQuantity

    if (it != packageQueuesByNeighbor.end())
    {
        return (it->second.size() + nPackages) <= it->second.capacity();
    }
    return false;
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