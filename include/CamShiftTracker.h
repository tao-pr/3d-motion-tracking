#ifndef CAMSHIFT
#define CAMSHIFT

#include <vector>
#include <math.h>
#include <functional>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cvaux.hpp>
#include "ITracker.h"

using namespace cv;
using namespace std;

class CamShiftTracker : public ITracker
{
public:
  function<void (Mat)> track();
  ~CamShiftTracker();

protected:
  void trackMotion(Mat& im);
};

#endif