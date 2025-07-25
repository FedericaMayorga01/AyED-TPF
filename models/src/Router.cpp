#include "../include/Router.hpp"
#include "../../application/include/NetworkConfig.hpp"
#include "../../application/include/NetworkSimulator.hpp"
#include "../include/Package.hpp"
#include "../include/Page.hpp"
#include <algorithm>
#include <boost/circular_buffer.hpp>
#include <cmath>
#include <iostream>
#include <set>
#include <stdexcept>
#include <utility>

Router::Router(int routerAddress, int packageSize, NetworkSimulator *networkSimulator) {
    this->packageSize = packageSize;
    this->routerAddress = routerAddress;
    this->networkSimulator = networkSimulator;
    this->routingTable = std::map<int, Link>();
}

int Router::getRouterAddress() const {
    return routerAddress;
}

bool Router::getAmIEndNode() const {
    return amIEndNode;
}

std::map<int, boost::circular_buffer<Package *> > Router::getPackageQueuesByNeighbor() const {
    return packageQueuesByNeighbor;
}

void Router::receivePage(const Page *page) {
    if (hasQueueFreeSpaceForPage(page->getOrigTerminalAddress(), page->getSizePage())) {
        std::cout << "\033[1m\033[32mReceive Page: \033[0m" << "Router " << routerAddress << " received page with ID " << page->getIdPage() << " from terminal "
                << page->getOrigTerminalAddress() << std::endl;
    } else {
        std::cout << "\033[1m\033[31mNo Space: \033[0m" << "Router " << routerAddress << " has no free space for page with ID " << page->getIdPage()
                << " from terminal " << page->getOrigTerminalAddress() << std::endl;
        return; // No space to process this page
    }

    // This method should receive a page and split it into packages
    std::list<Package *> packages = splitPage(page);

    // Store the packages in the packageQueuesByNeighbor map
    for (Package *pkg: packages) {
        packageQueuesByNeighbor[page->getOrigTerminalAddress()].push_back(pkg);
        std::cout << "Router " << routerAddress << " added package with ID "
                << packageQueuesByNeighbor[page->getOrigTerminalAddress()].back()->getIdPackage()
                << " to queue for neighbor (terminal) " << page->getOrigTerminalAddress() << std::endl;
    }
}

std::list<Package *> Router::splitPage(const Page *page) const {
    // This method should split a page into packages
    const int numberOfPackages = std::ceil(static_cast<float>(page->getSizePage()) / packageSize);

    std::list<Package *> packages;
    for (int packageId = 1; packageId <= numberOfPackages; packageId++) {
        auto* package = new Package(packageId, page->getIdPage(), packageSize, numberOfPackages,
                                      page->getOrigTerminalAddress(), page->getDestTerminalAddress(), networkSimulator-> getCurrentCycle());
        // Add current router to the route
        package->addToRouteTaken(page->getOrigTerminalAddress());
        package->addToRouteTaken(routerAddress);
        packages.push_back(package);
    }
    return packages;
}

void Router::sendPackage(int destAddress, Package *package) {
    std::cout << "Package with ID " << package->getIdPackage() << " is being sent from Router " << routerAddress
            << " to Router " << destAddress << std::endl;

    if (AddressUtils::isTerminal(destAddress)) {
        std::cout << "This address belong to a terminal node, adding to pendingPkgsByPageId" << std::endl;

        pendingPackagesByPageId[package->getIdPage()].push_back(package);

        std::cout << "Router " << routerAddress << " added package with ID " << package->getIdPackage()
                << " to pending packages for page ID " << package->getIdPage() << std::endl;

        return;
    }

    // This method should send a package to the specified destination address
    Router *destRouter = networkSimulator->getRouterByAddress(destAddress);
    if (destRouter == nullptr) {
        throw std::invalid_argument("Router with address " + std::to_string(destAddress) + " not found.");
    }
    // Add destination router to the route taken by the package
    destRouter->receivePackage(routerAddress, package);
}

void Router::receivePackage(int senderAddress, Package *package) {
    // This method should receive a package and process it
    package->addToRouteTaken(routerAddress);

    if (hasQueueFreeSpaceForPkg(senderAddress)) {
        package->incrementCycleCounter();
        std::cout << "Router " << routerAddress << " received package with ID " << package->getIdPackage()
                << " from Router " << senderAddress << std::endl;
        // Add the package to the queue of the corresponding neighbor
        packageQueuesByNeighbor[senderAddress].push_back(package);
    } else {
        std::cout << "\033[1mNo Space: \033[0m" << "Router " << routerAddress << " has no free space for package with ID " << package->getIdPackage()
                << " from Router " << senderAddress << std::endl;
    }
}

void Router::processQueues() {
    // This method should process the queues of packages
    for (auto &entry: packageQueuesByNeighbor) {
        boost::circular_buffer<Package *> &queue = entry.second;
        AddressUtils::shuffleQueue(queue); // Shuffle the queue to randomize processing order

        if (queue.empty()) {
            continue;
        }

        Package *package = queue.front();
        Link link = routingTable[package->getDestTerminalAddress()];
        int bandwidth = link.getBandwidth();

        int i = 0;
        do {
            // Update the routing table
            std::cout << "Package with ID " << package->getIdPackage() << " is being processed by Router "
                 << routerAddress << " is on cycle " << package->getCycleCounter() << " and networkSimulator is on "<< networkSimulator->getCurrentCycle() << std::endl;
            if (package->getCycleCounter() < networkSimulator -> getCurrentCycle()){
                sendPackage(link.getNeighbor(), package);

                Package lastProcessedPackage = *package;
                queue.pop_front();
                package = queue.front();
                if (queue.empty())
                    break; // Exit if the queue is empty

                if (lastProcessedPackage.getDestTerminalAddress() != package->getDestTerminalAddress()) {
                    link = routingTable[package->getDestTerminalAddress()];
                    bandwidth = link.getBandwidth();
                }
            } else {
                std::cout << "Package with ID " << package->getIdPackage() << " is still in cycle " << package->getCycleCounter()
                      << ", moving to the end of the queue." << std::endl;
                Package* lastProcessedPackage = queue.front();
                queue.pop_front();
                queue.push_back(lastProcessedPackage);

                package = queue.front();
                if (queue.back()->getDestTerminalAddress() != package->getDestTerminalAddress()) {
                    link = routingTable[package->getDestTerminalAddress()];
                    bandwidth = link.getBandwidth();
                }
            }

            i++;
        } while (bandwidth != 0 && i < bandwidth);

        std::cout << "Router " << routerAddress << " amIEndNode: " << (amIEndNode ? "true" : "false") << std::endl;

        if (!amIEndNode) {
            return;
        }

        for (auto &entry: pendingPackagesByPageId) {
            int pageId = entry.first;
            std::list<Package *> pendingPackages = entry.second;

            // Check if we have all packages for this page
            if (pendingPackages.empty() || !checkPagesById(pageId)) {
                continue;
            }

            std::cout << "Router " << routerAddress << " has all packages for page ID " << pageId << std::endl;

            // Rebuild the page from the pending packages
            Page *rebuiltPage = rebuildPage(pageId);


            if (rebuiltPage != nullptr) {
                std::cout << "Router " << routerAddress << " has rebuilt page with ID " << rebuiltPage->getIdPage() <<
                        std::endl;
                sendPage(rebuiltPage);
            }
        }
    }
}

void Router::updateRoutingTable(bool initialize, int queueSize, std::map<int, Link> newRoutingTable) {
    routingTable = std::move(newRoutingTable);

    // print new routing table complete with details
    //std::cout << "Router " << routerAddress << " routing table: " << std::endl;
    for (auto &[fst, snd]: routingTable) {
        //std::cout << "Terminal Dest: " << fst << ", Next Hop: " << snd.getNeighbor()
        //        << ", Bandwidth: " << snd.getBandwidth() << std::endl;
    }

    std::set<int> uniqueNeighbors;
    for (const auto &entry: routingTable) {
        uniqueNeighbors.insert(entry.second.getNeighbor());
    }

    if (initialize) {
        packageQueuesByNeighbor.clear();

        for (int neighbor: uniqueNeighbors) {
            amIEndNode = amIEndNode || AddressUtils::isTerminal(neighbor);
            packageQueuesByNeighbor.emplace(neighbor, boost::circular_buffer<Package *>(queueSize));
            //std::cout << "Router " << routerAddress << " has created queue for neighbor " << neighbor
            //        << " (capacity: " << packageQueuesByNeighbor[neighbor].capacity() << ")" << std::endl;
        }
    }
}

bool Router::hasQueueFreeSpaceForPkg(int neighborAddress) {
    std::cout << "Router " << routerAddress << " checking free space for neighbor " << neighborAddress
            << ": Current size: " << packageQueuesByNeighbor[neighborAddress].size()
            << ", Capacity: " << packageQueuesByNeighbor[neighborAddress].capacity() << std::endl;

    return packageQueuesByNeighbor[neighborAddress].size() < packageQueuesByNeighbor[neighborAddress].capacity();
}

bool Router::hasQueueFreeSpaceForPage(int neighborAddress, int pageSize) const {
    auto it = packageQueuesByNeighbor.find(neighborAddress);
    int nPackages = std::ceil(static_cast<float>(pageSize) / packageSize);

    if (it != packageQueuesByNeighbor.end()) {
        return (it->second.size() + nPackages) <= it->second.capacity();
    }
    return false;
}

bool Router::checkPagesById(int pageId) {
    std::list<Package *> pendingPackages;
    // This method should check if the required number of packages to assemble a page by pageId are in the list
    try {
        pendingPackages = pendingPackagesByPageId[pageId];
    } catch (const std::out_of_range &e) {
        std::cerr << "Error: Page ID " << pageId << " not found in pending packages." << std::endl;
        return false; // Page ID not found
    }

    // Get the expected total number of packages from any package in the list
    int expectedPackageCount = pendingPackages.front()->getAmountOfPackages();

    // Check if we have all the required packages
    return pendingPackages.size() == expectedPackageCount;
}

void Router::printRouteTakenByPackage(const Package *package) {
    // This method should print the route taken by each package in the pending packages
    std::cout << "\033[1m\033[35m  Package ID: \033[0m" << package->getIdPackage() << "\033[1m\033[35m | Route taken: \033[0m";

    std::cout << std::endl;
    // Print the route taken by the package
    const std::list<int> &route = package->getRouteTaken();
    if (route.empty()) {
        std::cout << "[No route recorded]";
    } else {
        bool first = true;
        for (int routerAddr: route) {
            if (!first)
                std::cout << " -> ";
            std::cout << routerAddr;
            first = false;
        }
    }
    std::cout << std::endl;
}

Page *Router::rebuildPage(const int pageId) {
    std::cout << "\033[1m\033[35mRebuild Page: \033[0m" <<"Router " << routerAddress << ": Rebuilding page with ID " << pageId << std::endl;
    // This method should rebuild a page from the pending packages
    std::list<Package *> pendingPackages = pendingPackagesByPageId[pageId];
    if (pendingPackages.empty()) {
        std::cerr << "No pending packages for page ID " << pageId << std::endl;
        return nullptr; // No packages to rebuild the page
    }

    // Sort packages by package ID using merge sort
    pendingPackages.sort([](const Package *a, const Package *b) { return a->getIdPackage() < b->getIdPackage(); });

    int totalSize = 0;

    for (const auto &pkg: pendingPackages) {
        totalSize += pkg->getSizePackage();
        pkg->addToRouteTaken(pkg->getDestTerminalAddress()); // Add destination terminal to the route
        printRouteTakenByPackage(pkg); // Show the route taken by the package
    }

    // Create the rebuilt page
    Page *rebuiltPage = new Page(pageId, totalSize, pendingPackages.front()->getOrigTerminalAddress(),
                                 pendingPackages.front()->getDestTerminalAddress());

    std::cout << "Router " << routerAddress << ": Successfully rebuilt page " << pageId << std::endl;

    // Clear the pending packages for this page ID
    pendingPackagesByPageId[pageId].clear();

    return rebuiltPage;
}

void Router::sendPage(Page *page) const {
    // This method should send a page to the destination terminal address
    const int terminalAddress = page->getDestTerminalAddress();

    const Terminal *destTerminal = networkSimulator->getTerminalByAddress(terminalAddress);
    if (destTerminal == nullptr) {
        throw std::invalid_argument("Terminal with address " + std::to_string(terminalAddress) + " not found.");
    }

    destTerminal->receivePage(page);
}
