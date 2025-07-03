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
  [[nodiscard]] int getIdPage() const;
  [[nodiscard]] int getSizePage() const;
  [[nodiscard]] int getOrigTerminalAddress() const;
  [[nodiscard]] int getDestTerminalAddress() const;
};

#endif // PAGE_HPP