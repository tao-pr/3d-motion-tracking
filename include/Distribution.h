#ifndef DISTRIBUTION
#define DISTRIBUTION

#include <vector>

using namespace std;

template<class T> class Distribution 
{
private:
  vector<T> population;
public:
  inline void clear()
  {
    this->population.clear();
  }

  inline void addPopulation(T e)
  {
    this->population.push_back(e);
  }
};

#endif