#ifndef TERMINAL_HPP
#define TERMINAL_HPP

class Terminal
{
private:
    int terminalAddress;
    int routerAddress;

public:
    Terminal(int terminalAddress, int routerAddress);
    int getTerminalAddress(void);
    int getRouterAddress(void);
    void sendPage(void);
};

#endif // TERMINAL_HPP