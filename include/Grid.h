#ifndef GRID
#define GRID

#include <vector>
#include <functional>
#include <opencv2/opencv.hpp>
#include "master.h"

using namespace std;
using namespace cv;

typedef function<double (double)> GravityFunction ;

class Grid
{
protected:
  Mat velocityX, velocityY;
  GravityFunction gravity; // Function which maps [distance] => [gravity magnitude]
  double gravityThreshold; // Minimum gravity magnitude to draw effect

public:
  Grid(Size gridSize, GravityFunction gravity, double gravityThreshold = 0.0)
  {
    this->velocityX = Mat::zeros(gridSize.height, gridSize.width, CV_64FC1);
    this->velocityY = Mat::zeros(gridSize.height, gridSize.width, CV_64FC1);
    this->gravity   = gravity;
    this->gravityThreshold = gravityThreshold;
  }
  
  virtual ~Grid()
  {
  }

  inline void setAnchor(int x, int y, const Point2f& velocity)
  {
    this->velocityX.at<double>(y, x) = velocity.x;
    this->velocityY.at<double>(y, x) = velocity.y;
  }
};



#endif