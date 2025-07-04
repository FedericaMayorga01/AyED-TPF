#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include <list>
#include <string>

class Package
{
  private:
    int idPackage;
    int idPage;
    int sizePackage;
    int amountOfPackages;
    int origTerminalAddress;
    int destTerminalAddress;
    int cycleCounter{};
    std::list<int> routeTaken;

  public:
    Package(int idPackage, int idPage, int sizePackage, int amountOfPackages, int origTerminalAddress,
            int destTerminalAddress);

    [[nodiscard]] int getIdPackage() const;
    [[nodiscard]] int getIdPage() const;
    [[nodiscard]] int getSizePackage() const;
    [[nodiscard]] int getAmountOfPackages() const;
    [[nodiscard]] int getOrigTerminalAddress() const;
    [[nodiscard]] int getDestTerminalAddress() const;
    [[nodiscard]] int getCycleCounter() const;
    [[nodiscard]] std::list<int> getRouteTaken() const;
    void addToRouteTaken(int address);

    // override toString
    std::string toString() const;
};

#endif // PACKAGE_HPP
