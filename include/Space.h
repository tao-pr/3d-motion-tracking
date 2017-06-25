#ifndef SPACE
#define SPACE

#include <vector>
#include <opencv2/opencv.hpp>
#include "master.h"

using namespace std;
using namespace cv;

class Space  // TAODEBUG: KD-Tree
{
private:
public:
  Space();
  ~Space();
  static Space& create(const vector<Point2f> ps);

  operator DistortionMapping >>(const Space &another);
};

#endif