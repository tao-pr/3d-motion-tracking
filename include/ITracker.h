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
  virtual function<void (Mat)> track(Mat& input) = 0;
};