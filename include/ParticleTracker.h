#ifndef PARTICLE_TRACKER
#define PARTICLE_TRACKER

#include <set>
#include <tuple>
#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"
#include "master.h"
#include "ITracker.h"
#include "DrawUtils.h"
#include "Alignment.h"
#include "Grid.h"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

struct TrackedPoint
{
  float x;
  float y;
  int numAbsence; // Number of consecutive frames this point is absent
  int numPresent; // Number of consecutive frames this point is present

  static create(float x, float y)
  {
    TrackedPoint t;
    t.x = x;
    t.y = y;
    t.numAbsence = 0;
    t.numPresent = 1;
    return t;
  }
};

class ParticleTracker : public ITracker
{
private:
  const int maxAbsenceAllowed = 3;
  const double maxDisplacement = 12;
  const double momentum = 0.25;

  Alignment* alignment;

  // TAOTODO:Point which is absent for one single frame should remain
  vector<Point2f> prevPoints;

  Mat prevFeatures;
  Grid* grid;

protected:
  Ptr<Feature2D> sift;
  void trackFeatures(Mat &im);
  void initialiseGrid(int w, int h);

public:
  ParticleTracker();
  ~ParticleTracker();
  tuple<vector<Point2f>,Mat> detectPoints(Mat &in);

  function<void (Mat)> track();
};



#endif