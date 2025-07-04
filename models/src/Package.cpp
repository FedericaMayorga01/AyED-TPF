#include "../include/Package.hpp"
#include <list>

Package::Package(const int idPackage, const int idPage, const int sizePackage, const int amountOfPackages, const int origTerminalAddress,
                 const int destTerminalAddress)
{
    this->idPackage = idPackage;
    this->idPage = idPage;
    this->sizePackage = sizePackage;
    this->amountOfPackages = amountOfPackages;
    this->origTerminalAddress = origTerminalAddress;
    this->destTerminalAddress = destTerminalAddress;
}

int Package::getIdPackage() const
{
    return idPackage;
}

int Package::getIdPage() const
{
    return idPage;
}

int Package::getSizePackage() const
{
    return sizePackage;
}

int Package::getAmountOfPackages() const
{
    return amountOfPackages;
}

int Package::getOrigTerminalAddress() const
{
    return origTerminalAddress;
}

int Package::getDestTerminalAddress() const
{
    return destTerminalAddress;
}

int Package::getCycleCounter() const
{
    return cycleCounter;
}

std::list<int> Package::getRouteTaken() const
{
    return routeTaken;
}

void Package::addToRouteTaken(const int address)
{
    this->routeTaken.push_back(address);
}

std::string Package::toString() const {
    std::string routeStr;
    for (const auto& addr : routeTaken) {
        if (!routeStr.empty()) {
            routeStr += " -> ";
        }
        routeStr += std::to_string(addr);
    }
    return "Package ID: " + std::to_string(idPackage) +
           ", Page ID: " + std::to_string(idPage) +
           ", Size: " + std::to_string(sizePackage) +
           ", Amount: " + std::to_string(amountOfPackages) +
           ", Origin: " + std::to_string(origTerminalAddress) +
           ", Destination: " + std::to_string(destTerminalAddress) +
           ", Route Taken: [" + routeStr + "]";
}
