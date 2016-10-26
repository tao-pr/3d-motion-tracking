#include <vector>
#include <functional>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cvaux.hpp>
#include "ITracker.h"

using namespace cv;
using namespace std;

class CamShiftTracker
{
public:
  function<void (Mat)> track();
};