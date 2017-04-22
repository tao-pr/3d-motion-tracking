#ifndef PARTICLE_TRACKER
#define PARTICLE_TRACKER

#include <vector>
#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;

class ParticleTracker
{
protected:
  Ptr<Feature2D> sift;

public:
  ParticleTracker();
  vector<Point2f> detectPoints(Mat &in);
};



#endif