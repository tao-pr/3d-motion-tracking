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

const int VIS_PATCH_SIZE = 16;
const int VIS_MAX_SPOT   = 20;

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
private:
  void drawVisCell(int i, int j, double score);
  void redrawVis(const Mat& matchScore);
protected:
  bool isVisualisationOn;
  function<double (Point2f, Point2f)> measureDistFunction;
  function<double (Mat, Mat)> measureSimilarityFunction;
  double maxDistance;
  Mat vis;
public:
  Alignment(function<double (Point2f, Point2f)> measureDistance, function<double (Mat, Mat)> measureFeatureSimilarity, double maxMoveDistance);
  void setVisualisation(bool on);
  void align(vector<Point2f> basepoints, vector<Point2f> newpoints, const Mat& baseFeatures, const Mat& newFeatures);
};

#endif