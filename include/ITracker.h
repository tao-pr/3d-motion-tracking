#ifndef ITRACKER
#define ITRACKER

#include <vector>
#include <functional>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.hpp>

using namespace cv;
using namespace std;

class ITracker 
{
public:
  virtual inline ~ITracker(){};
  virtual function<void (Mat)> track() = 0;
};

#endif