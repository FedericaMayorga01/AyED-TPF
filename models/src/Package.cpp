#include "../include/Package.hpp"
#include <list>

Package::Package(int idPackage, int idPage, int sizePackage,
                 int origTerminalAddress, int destTerminalAddress)
{
    this->idPackage = idPackage;
    this->idPage = idPage;
    this->sizePackage = sizePackage;
    this->origTerminalAddress = origTerminalAddress;
    this->destTerminalAddress = destTerminalAddress;
}

int Package::getIdPackage() const { return idPackage; }

int Package::getIdPage() const { return idPage; }

int Package::getSizePackage() const { return sizePackage; }

int Package::getOrigTerminalAddress() const { return origTerminalAddress; }

int Package::getDestTerminalAddress() const { return destTerminalAddress; }

int Package::getCycleCounter() const { return cycleCounter; }

std::list<int> Package::getRouteTaken() const { return routeTaken; }