#ifndef TERMINAL_HPP
#define TERMINAL_HPP
#include <vector>

class Page;
class NetworkSimulator;
class Terminal
{
  private:
    int terminalAddress;
    int routerAddress;
    NetworkSimulator* networkSimulator;
    std::vector<Terminal> terminalNodes;
    Page* generatePage();

  public:
    Terminal(int terminalAddress, int routerAddress, NetworkSimulator* networkSimulator);

    [[nodiscard]] int getTerminalAddress() const;

    [[nodiscard]] int getRouterAddress() const;

    void addTerminalNode(const Terminal &terminalNode);
    void sendPage();
    void receivePage(const Page* page) const;
};

#endif // TERMINAL_HPP
