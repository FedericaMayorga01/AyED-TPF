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
    Page* generatePage(void);

  public:
    Terminal(int terminalAddress, int routerAddress, NetworkSimulator* networkSimulator);
    int getTerminalAddress(void);
    int getRouterAddress(void);
    void setTerminalNodes(std::vector<Terminal> terminalNodes);
    void sendPage(void);
    void receivePage(Page* page);
};

#endif // TERMINAL_HPP
