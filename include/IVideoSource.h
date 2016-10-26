#include <vector>
#include <functional>
#include <opencv2/opencv.hpp>
#include <opencv/cvaux.hpp>
#include "ITransformation.h"

#pragma once

using namespace std;
using namespace cv;

/**
 * Video source capture interface
 */

class IVideoSource
{
public:
  virtual Mat captureWith(ITransformation &t) = 0;
  virtual void captureRealtimeWith(ITransformation &t, function<void (Mat)> pipe) = 0;
};