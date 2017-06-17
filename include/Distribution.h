#ifndef DISTRIBUTION
#define DISTRIBUTION

#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"

using namespace std;
using namespace cv;

namespace Distribution
{
  /**
   * Bucket defines an interval of continuous space of type <T>
   */
  template<class T> class Bucket
  {
  protected:
    T interval;
  public:
    Bucket(T interval)
    {
      this->interval = interval;
    }

    virtual bool valueInBucket(int bucketIndex, const T value) = 0;
  };

  class DoubleBucket : public Bucket<double>
  {
  public:
    bool valueInBucket(int bucketIndex, const double value)
    {
      return value < bucketIndex * this->interval;
    }
  };

  template<class T> class GenericDistribution 
  {
  private:
    vector<T> population;
  public:
    inline void clear()
    {
      this->population.clear();
    };

    inline void addPopulation(T e)
    {
      this->population.push_back(e);
    };

    void bucketPlot(Bucket<T> bucketInterval, string wnd)
    {
      Mat plot;
      // TAOTODO:
      imshow(wnd, plot);
    }
  };
}

#endif