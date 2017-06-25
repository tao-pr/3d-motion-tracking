#ifndef TRANSFORMATION
#define TRANSFORMATION

#include <vector>
#include <functional>
#include <opencv2/opencv.hpp>
#include "master.h"
#include "Transformation.h"

using namespace std;
using namespace cv;

#define TRANSFORM_FUNC  function<T (T, double[])>

template<class T> class Transformation
{
protected:
  TRANSFORM_FUNC t;
public:
  Transformation(TRANSFORM_FUNC _t);
};

class PointTransformation : public Transformation<Point2f>
{
  // TAOTODO:
};


#endif