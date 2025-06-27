#include "../include/Router.hpp"
#include "../include/Package.hpp"
#include "../include/Page.hpp"
#include "../include/NetworkConfig.hpp"
#include "../include/NetworkSimulator.hpp"
#include <iostream>
#include <boost/circular_buffer.hpp>
#include <cmath>
#include <stdexcept>

Router::Router(int routerAddress, std::map<int, int> routingTable, int queueSize, int packageSize, NetworkSimulator* networkSimulator)
{
    this->packageSize = packageSize;
    this->routerAddress = routerAddress;
    this->routingTable = routingTable;
    this->networkSimulator = networkSimulator;

    // Initialize package queues for each neighbor
    for (const auto &entry : routingTable)
    {
        int neighborAddress = entry.first;
        packageQueuesByNeighbor[neighborAddress] = boost::circular_buffer<Package*>(queueSize);
        this->amIEndNode = AddressUtils::isTerminal(neighborAddress);
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

void Router::receivePage(Page* page)
{
    if (hasQueueFreeSpaceForPage(page->getOrigTerminalAddress(), page->getSizePage()))
    {
        std::cout << "Router " << routerAddress << " received page with ID " << page->getIdPage()
                  << " from terminal " << page->getOrigTerminalAddress() << std::endl;
    }
    else
    {
        std::cout << "Router " << routerAddress << " has no free space for page with ID "
                  << page->getIdPage() << " from terminal " << page->getOrigTerminalAddress() << std::endl;
        return; // No space to process this page
    }

    // This method should receive a page and split it into packages
    std::list<Package*> packages = splitPage(page);

    // Store the packages in the packageQueuesByNeighbor map
    boost::circular_buffer queue = packageQueuesByNeighbor[page->getOrigTerminalAddress()];
    for (Package* pkg : packages)
    {
        queue.push_back(pkg);
    }
}

std::list<Package*> Router::splitPage(Page* page)
{
    // This method should split a page into packages
   int nPackages = std::ceil(static_cast<float>(page->getSizePage()) / packageSize);
   std::list<Package*> packages;
   for (int i = 0; i < nPackages; ++i)
   {
       int packageId = i; // Package IDs start from 1
       Package* package = new Package(packageId, page->getIdPage(), packageSize,
                                      page->getOrigTerminalAddress(), page->getDestTerminalAddress());
       package->getRouteTaken().push_back(routerAddress); // Add current router to the route
       packages.push_back(package);
   }
   return packages;
}

void Router::sendPackage(int destAddress, Package* package)
{
    // This method should send a package to the specified destination address
    Router* destRouter = networkSimulator->getRouterByAddress(destAddress);
    if (destRouter == NULL)
    {
        throw std::invalid_argument("Router with address " + std::to_string(destAddress) + " not found.");
    }

    package->getRouteTaken().push_back(destAddress);
    destRouter->receivePackage(package);
}

void Router::receivePackage(Package* package)
{
    // This method should receive a package and process it
    if (hasQueueFreeSpaceForPkg(this->routerAddress))
    {
        std::cout << "Router " << routerAddress << " received package with ID "
                  << package->getIdPackage() << " from Router " 
                  << package->getRouteTaken().back() << std::endl;
    }
    else
    {
        std::cout << "Router " << routerAddress << " has no free space for package with ID "
                  << package->getIdPackage() << " from Router "
                  << package->getRouteTaken().back() << std::endl;
        return; // No space to process this package
    }

    // Add the package to the queue of the corresponding neighbor
    boost::circular_buffer<Package*> queue = packageQueuesByNeighbor[package->getRouteTaken().back()];
    queue.push_back(package);
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
    int nPackages = std::ceil(static_cast<float>(pageSize) / packageSize);

    if (it != packageQueuesByNeighbor.end())
    {
        return (it->second.size() + nPackages) <= it->second.capacity();
    }
    return false;
}

Page* Router::rebuildPage()
{
    // This method should rebuild a page from the pending packages
    // for now it returns an empty Page
    Page* emptyPage = new Page(0, 0, 0, 0);
    return emptyPage;
}

void Router::sendPage(int destTerminalAddress)
{
    // This method should send a page to the specified terminal address
}