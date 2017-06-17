#ifndef DISTRIBUTION
#define DISTRIBUTION

#include <assert.h>
#include <tuple>
#include <vector>
#include <unordered_map>
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
    T lowerBound;
    T upperBound;
    T interval; // Resolution of the bucket along the line space
  public:
    Bucket(T interval, T lowerBound, T upperBound)
    {
      ASSERT(lowerBound < upperBound);
      this->interval = interval;
      this->lowerBound = lowerBound;
      this->upperBound = upperBound;
    }

    // Index of the bucket which the specified value belongs to
    virtual int index(const T v) = 0;
    virtual int numBucketsWithinInterval() = 0;
  };

  class DoubleBucket : public Bucket<double>
  {
  public:
    int index(const double v)
    {
      return (int)floor((v - this->lowerBound)/this->interval);
    }

    int numBucketsWithinInterval()
    {
      return (this->upperBound - this->lowerBound);
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

    void bucketPlot(Bucket<T> bucketInterval, tuple<T,T> bounds, string wnd, int drawUnitSize = 3)
    {
      int bucketCount = bucketInterval.numBucketsWithinInterval();
      Mat buckets = Mat::zeros(1, bucketCount, CV_32FC1);
      // Collect bucket population ticks
      for (auto p : this->population)
      {
        int index = bucketInterval.index(p);
        if (index < buckets.cols)
        {
          buckets.at<float>(1, index) = buckets.at<float>(1, index) + 1;
        }  
      }
      normalize(buckets, buckets, 0, 100, NORM_MINMAX);

      int M = drawUnitSize * 200;
      int N = drawUnitSize * bucketCount;
      Mat plot = Mat::zeros(M, N, CV_8UC3);

      for (int b=0; b<bucketCount; b++)
      {
        auto freq = (int)buckets.at<float>(1, b);
        auto xPos = (drawUnitSize * b) + (int)floor(drawUnitSize/2);
        auto toppoint    = Point2f(xPos, drawUnitSize * freq);
        auto bottompoint = Point2f(xPos, 0);
        line(plot, toppoint, bottompoint, Scalar(0,50,200), 1, CV_AA);
      }
      imshow(wnd, plot);
    }
  };
}

#endif