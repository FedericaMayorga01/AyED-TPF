#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <list>

class Package {
private:
  int idPackage;
  int idPage;
  int sizePackage;
  int origTerminalAddress;
  int destTerminalAddress;
  int cycleCounter;
  std::list<int> routeTaken;

public:
  Package(int idPackage, int idPage, int sizePackage, int origTerminalAddress,
          int destTerminalAddress);

  int getIdPackage() const;
  int getIdPage() const;
  int getSizePackage() const;
  int getOrigTerminalAddress() const;
  int getDestTerminalAddress() const;
  int getCycleCounter() const;
  std::list<int> getRouteTaken() const;
};

#endif // PACKAGE_HPP