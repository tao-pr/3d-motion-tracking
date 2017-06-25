#ifndef SPACE
#define SPACE

#include <vector>
#include <opencv2/opencv.hpp>
#include "master.h"
#include "Transformation.h"

using namespace std;
using namespace cv;

/**
 * Space structure which builds up a network of 
 * vertices and their connected neighbours 
 */
class Space  // TAODEBUG: KD-Tree
{
private:
  vector<Point2f> vertices;
public:
  Space();
  ~Space();
  static Space& create(const vector<Point2f> ps);

  operator PointTransformation >>(const Space &another) = 0;
};

#endif