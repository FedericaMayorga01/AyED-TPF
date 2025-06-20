#ifndef PAGE_HPP
#define PAGE_HPP

class Page {
private:
  int idPage;
  int sizePage;
  int origTerminalAddress;
  int destTerminalAddress;

public:
  Page(int idPage, int sizePage, int origTerminalAddress,
       int destTerminalAddress);
  int getIdPage() const;
  int getSizePage() const;
  int getOrigTerminalAddress() const;
  int getDestTerminalAddress() const;
};

#endif // PAGE_HPP