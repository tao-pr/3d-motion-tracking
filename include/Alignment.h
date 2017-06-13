#ifndef ALIGNMENT
#define ALIGNMENT

#include <vector>
#include <queue>
#include <tuple>
#include <map>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "coloriser.h"

using namespace std;
using namespace cv;

typedef tuple<double, int> distanceToIndex;

class compareDistance
{
public:
  // TRUE if a<b
  inline bool operator()(distanceToIndex &a, distanceToIndex &b)
  { 
    return get<0>(a) < get<0>(b); 
  }
};

class Alignment
{
protected:
  function<double (Point2f, Point2f)> measureFunction;
  double maxDistance;
public:
  Alignment(function<double (Point2f, Point2f)> measure, double maxMoveDistance);
  void align(vector<Point2f> basepoints, vector<Point2f> newpoints, const Mat& baseFeatures, const Mat& newFeatures) const;
};

#endif