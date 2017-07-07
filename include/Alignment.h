#ifndef ALIGNMENT
#define ALIGNMENT

#include <vector>
#include <queue>
#include <tuple>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "master.h"
#include "Distribution.h"

using namespace std;
using namespace cv;
using namespace Distribution;

const int VIS_PATCH_SIZE      = 8;
const int VIS_MAX_SPOT        = 48;
const double FACTOR_SIMILARIY = 0.25;

typedef tuple<double, int> distanceToIndex;

class compareDistance
{
public:
  // Ascending order
  inline bool operator()(distanceToIndex &a, distanceToIndex &b)
  { 
    return get<0>(a) < get<0>(b); 
  }
};

class compareScore
{
public:
  // Descending order
  inline bool operator()(distanceToIndex &a, distanceToIndex &b)
  { 
    return get<0>(a) > get<0>(b); 
  }
};


#define MAX_HISTORY_LENGTH    5

struct TrackablePoint
{
  Mat feature;
  deque<Point2f> history;
  int numAbsence; // Number of consecutive frames this point is absent
  int numPresent; // Number of consecutive frames this point is present

  static TrackablePoint create(const Point2f& p, const Mat& feat)
  {
    TrackablePoint t;
    t.feature = feat.clone();
    t.history.push_back(p);
    t.numAbsence = 0;
    t.numPresent = 1;
    return t;
  }

  // TAOTODO: write a velocity function which estimates with 3 consecutive frames

  bool updateNewPosition(const Point2f& p, double momentum, const Mat& newFeat)
  {
    // Update new position by momentum of history
    Point2f np = p;
    int L = history.size();
    if (L>0)
    {
      double sumMomentum = 0;
      for (int i=L-1, k=0; i>=0; i--, k++)
      {
        double c    = pow(momentum, k); // TAOTODO: Add some random noise?
        np          = np + c * history[i];
        sumMomentum += c;
      }
      np /= (1+sumMomentum);
    }
    this->history.push_back(np);
    this->numPresent++;
    this->numAbsence = 0;
    newFeat.copyTo(this->feature);
    if (this->history.size() > MAX_HISTORY_LENGTH)
    {
      this->history.pop_front();
      return false;
    }
    return true;
  }

  int markAbsent()
  {
    this->numAbsence += 1;
    this->numPresent = 0;
    return this->numAbsence;
  }

  Point2f get() const
  {
    return this->history.back();
  }
};


class Alignment
{
protected:
  bool isVisualisationOn;
  function<double (Point2f, Point2f)> measureDistFunction;
  double maxDistance;
public:
  Alignment(function<double (Point2f, Point2f)> measureDistance, double maxMoveDistance);
  void setVisualisation(bool on);
  unordered_map<int,int> align(vector<TrackablePoint> basepoints, vector<Point2f> newpoints, const Mat newFeatures);
};

#endif