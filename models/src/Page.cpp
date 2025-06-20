#include "../include/Page.hpp"

Page::Page(int idPage, int sizePage, int origTerminalAddress,
           int destTerminalAddress)
{
    this->idPage = idPage;
    this->sizePage = sizePage;
    this->origTerminalAddress = origTerminalAddress;
    this->destTerminalAddress = destTerminalAddress;
}

int Page::getIdPage() const { return idPage; }

int Page::getSizePage() const { return sizePage; }

int Page::getOrigTerminalAddress() const { return origTerminalAddress; }

int Page::getDestTerminalAddress() const { return destTerminalAddress; }