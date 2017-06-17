#include "Distribution.h"

template class Distribution::Bucket<double>;
template class Distribution::GenericDistribution<double>;

template<class T>
Distribution::Bucket<T>::Bucket(T interval, T lowerBound, T upperBound)
{
  assert(lowerBound < upperBound);
  this->interval = interval;
  this->lowerBound = lowerBound;
  this->upperBound = upperBound;
}

template<class T>
int Distribution::Bucket<T>::index(const T v)
{
  return (int)floor((v - this->lowerBound)/this->interval);
}

template<class T>
int Distribution::Bucket<T>::numBucketsWithinInterval()
{
  return (this->upperBound - this->lowerBound);
}

template<class T>
void Distribution::GenericDistribution<T>::bucketPlot(Bucket<T> bucketInterval, tuple<T,T> bounds, string wnd, int drawUnitSize)
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