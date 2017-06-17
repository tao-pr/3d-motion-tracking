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

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

class ParticleTracker : public ITracker
{
private:
  Alignment* alignment;
  vector<Point2f> prevPoints;
  Mat prevFeatures;

protected:
  Ptr<Feature2D> sift;
  void trackFeatures(Mat &im);

public:
  ParticleTracker();
  ~ParticleTracker();
  tuple<vector<Point2f>,Mat> detectPoints(Mat &in);

  function<void (Mat)> track();
};



#endif