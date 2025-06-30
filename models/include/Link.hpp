#ifndef LINK_HPP
#define LINK_HPP

class Link
{
  private:
    int sourceAddress;
    int neighbor;
    int bandwidth;

  public:
    // Constructors
    Link(); // Default constructor
    Link(int sourceAddress, int neighbor, int bandwidth);

    // Destructor
    ~Link();

    // Getters
    int getSourceAddress() const;
    int getNeighbor() const;
    int getBandwidth() const;

    // Setters
    void setBandwidth(int bandwidth);
};

#endif // LINK_HPP
