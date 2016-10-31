#ifndef MOTIONTRACK
#define MOTIONTRACK

#include <vector>
#include <math.h>
#include <functional>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cvaux.hpp>
#include "DrawUtils.h"
#include "TrackUtils.h"
#include "ITracker.h"

using namespace cv;
using namespace std;


class MotionTracker : ITracker
{
public:
  MotionTracker();
  function<void (Mat)> track();

protected:
  void trackMotion(Mat& im);
  Mat calcHistBackProjection(Mat& im);
};

#endif