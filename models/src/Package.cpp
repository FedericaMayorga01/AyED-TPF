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
