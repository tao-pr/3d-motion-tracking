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
  Mat velocity;
  GravityFunction gravity; // Function which maps [distance] => [gravity magnitude]
  double gravityThreshold; // Minimum gravity magnitude to draw effect

public:
  Grid(Size gridSize, GravityFunction gravity, double gravityThreshold = 0.0)
  {
    this->velocity = Mat::zeros(gridSize.height, gridSize.width, 3);
    this->GravityFunction = gravity;
    this->gravityThreshold = gravityThreshold;
  }
  ~Grid();
};



#endif