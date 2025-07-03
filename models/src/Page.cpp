#include "../include/Page.hpp"

Page::Page(const int idPage, const int sizePage, const int origTerminalAddress,
           const int destTerminalAddress): idPage(0) {
    this->idPage = idPage;
    this->sizePage = sizePage;
    this->origTerminalAddress = origTerminalAddress;
    this->destTerminalAddress = destTerminalAddress;
}

int Page::getIdPage() const { return idPage; }

int Page::getSizePage() const { return sizePage; }

int Page::getOrigTerminalAddress() const { return origTerminalAddress; }

int Page::getDestTerminalAddress() const { return destTerminalAddress; }