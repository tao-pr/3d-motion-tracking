#ifndef PARTICLE_TRACKER
#define PARTICLE_TRACKER

#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"
#include "coloriser.h"
#include "ITracker.h"
#include "DrawUtils.h"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

class ParticleTracker : public ITracker
{
protected:
  Ptr<Feature2D> sift;

public:
  ParticleTracker();
  ~ParticleTracker();
  vector<Point2f> detectPoints(Mat &in);

  function<void (Mat)> track();
};



#endif