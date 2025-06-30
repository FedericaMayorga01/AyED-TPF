#include "../include/Router.hpp"
#include "../include/NetworkConfig.hpp"
#include "../include/NetworkSimulator.hpp"
#include "../include/Package.hpp"
#include "../include/Page.hpp"
#include <algorithm>
#include <boost/circular_buffer.hpp>
#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>

Router::Router(int routerAddress, int queueSize, int packageSize, NetworkSimulator* networkSimulator)
{
    this->packageSize = packageSize;
    this->routerAddress = routerAddress;
    this->networkSimulator = networkSimulator;
    this->routingTable = std::map<int, Link>();

    // Initialize package queues for each neighbor
    for (const auto& entry : routingTable)
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
        std::cout << "Router " << routerAddress << " received page with ID " << page->getIdPage() << " from terminal "
                  << page->getOrigTerminalAddress() << std::endl;
    }
    else
    {
        std::cout << "Router " << routerAddress << " has no free space for page with ID " << page->getIdPage()
                  << " from terminal " << page->getOrigTerminalAddress() << std::endl;
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
    int numberOfPackages = std::ceil(static_cast<float>(page->getSizePage()) / packageSize);
    std::list<Package*> packages;
    for (int i = 0; i < numberOfPackages; ++i)
    {
        int packageId = i + 1; // Package IDs start from 1
        Package* package = new Package(packageId, page->getIdPage(), packageSize, numberOfPackages,
                                       page->getOrigTerminalAddress(), page->getDestTerminalAddress());
        // Add current router to the route
        package->addToRouteTaken(routerAddress);
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
    // Add destination router to the route taken by the package
    package->addToRouteTaken(destAddress);
    destRouter->receivePackage(package);
}

void Router::receivePackage(Package* package)
{
    // This method should receive a package and process it
    if (hasQueueFreeSpaceForPkg(this->routerAddress))
    {
        std::cout << "Router " << routerAddress << " received package with ID " << package->getIdPackage()
                  << " from Router " << package->getRouteTaken().back() << std::endl;
    }
    else
    {
        std::cout << "Router " << routerAddress << " has no free space for package with ID " << package->getIdPackage()
                  << " from Router " << package->getRouteTaken().back() << std::endl;
        return; // No space to process this package
    }

    // Add the package to the queue of the corresponding neighbor
    boost::circular_buffer<Package*> queue = packageQueuesByNeighbor[package->getRouteTaken().back()];
    queue.push_back(package);
}

void Router::processQueues()
{
    // This method should process the queues of packages
    // std::pair<const int, boost::circular_buffer<Package *>>
    for (auto& entry : packageQueuesByNeighbor)
    {
        Package* package = entry.second.front();
        entry.second.pop_front(); // Remove the package from the queue
        Link link = routingTable[package->getDestTerminalAddress()];
        int bandwidth = link.getBandwidth();

        sendPackage(link.getNeighbor(), package);
        std::cout << "Router " << routerAddress << " processed package with ID " << package->getIdPackage()
                  << " from neighbor " << entry.first << std::endl;

        for (int i = 0; i < bandwidth - 1; i++)
        {
            if (entry.second.size() <= 0)
            {
                break;
            }
            Package* pkg = entry.second.front();
            entry.second.pop_front(); // Remove the package from the queue
            sendPackage(link.getNeighbor(), pkg);
            std::cout << "Router " << routerAddress << " processed package with ID " << pkg->getIdPackage()
                      << " from neighbor " << entry.first << std::endl;
        }
    }
    
    if (!amIEndNode) {
        return;
    }

    for (auto& entry : pendingPackagesByPageId)
    {
        int pageId = entry.first;
        std::list<Package*> pendingPackages = entry.second;

        // Check if we have all packages for this page
        if (!checkPagesById(pageId))
        {
            continue;
        }
        // Rebuild the page from the pending packages
        Page* rebuiltPage = rebuildPage(pageId);
        if (rebuiltPage != nullptr)
        {
            sendPage(rebuiltPage);
        }
    }
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

bool Router::checkPagesById(int pageId)
{
    std::list<Package*> pendingPackages;
    // This method should check if the required number of packages to assemble a page by pageId are in the list
    try
    {
        pendingPackages = pendingPackagesByPageId[pageId];
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << "Error: Page ID " << pageId << " not found in pending packages." << std::endl;
        return false; // Page ID not found
    }

    // Get the expected total number of packages from any package in the list
    int expectedPackageCount = pendingPackages.front()->getAmountOfPackages();

    // Check if we have all the required packages
    return pendingPackages.size() != expectedPackageCount;
}

void Router::printRouteTakenByPackage(Package* package)
{
    // This method should print the route taken by each package in the pending packages
    std::cout << "  Package ID: " << package->getIdPackage() << " | Size: " << package->getSizePackage() << " bytes"
              << " | Route taken: ";

    // Print the route taken by the package
    const std::list<int>& route = package->getRouteTaken();
    if (route.empty())
    {
        std::cout << "[No route recorded]";
    }
    else
    {
        bool first = true;
        for (int routerAddr : route)
        {
            if (!first)
                std::cout << " -> ";
            std::cout << routerAddr;
            first = false;
        }
    }
    std::cout << std::endl;
}

Page* Router::rebuildPage(int pageId)
{
    // This method should rebuild a page from the pending packages
    std::list<Package*> pendingPackages = pendingPackagesByPageId[pageId];
    if (pendingPackages.empty())
    {
        std::cerr << "No pending packages for page ID " << pageId << std::endl;
        return nullptr; // No packages to rebuild the page
    }

    // Sort packages by package ID using merge sort
    pendingPackages.sort([](const Package* a, const Package* b) { return a->getIdPackage() < b->getIdPackage(); });

    int totalSize = 0;

    for (Package* pkg : pendingPackages)
    {
        totalSize += pkg->getSizePackage();
        pkg->addToRouteTaken(pkg->getDestTerminalAddress()); // Add destination terminal to the route
        printRouteTakenByPackage(pkg);                       // Show the route taken by the package
    }

    // Create the rebuilt page
    Page* rebuiltPage = new Page(pageId, totalSize, pendingPackages.front()->getOrigTerminalAddress(),
                                 pendingPackages.front()->getDestTerminalAddress());

    std::cout << "Router " << routerAddress << ": Successfully rebuilt page " << pageId
              << " with total size: " << totalSize << " bytes" << std::endl;

    // Clear the pending packages for this page ID
    pendingPackagesByPageId[pageId].clear();

    return rebuiltPage;
}

void Router::sendPage(Page* page)
{
    // This method should send a page to the destination terminal address
    int terminalAddress = page->getDestTerminalAddress();

    Terminal* destTerminal = networkSimulator->getTerminalByAddress(terminalAddress);
    if (destTerminal == NULL)
    {
        throw std::invalid_argument("Terminal with address " + std::to_string(terminalAddress) + " not found.");
    }

    destTerminal->receivePage(page);
}
