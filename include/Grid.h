#ifndef GRID
#define GRID

#include <math.h>
#include <vector>
#include <tuple>
#include <functional>
#include <opencv2/opencv.hpp>
#include "master.h"

using namespace std;
using namespace cv;

class Grid
{
private:
  const int PATCH_MAP_SIZE = 2;

protected:
  Size size;
  vector<Point2i> anchors; // TAOTODO: Should use [TrackablePoint] so we can use velocity estmation
  Mat velocityX, velocityY;
  double gravityThreshold; // Minimum gravity magnitude to draw effect
  double maxDistance; // Maximum distance from the anchor which gravity can take effect

  Mat canvas;

public:
  Grid(Size gridSize, double maxInfluentialDistance, double gravityThreshold = 0.0)
  {
    this->size      = gridSize;
    this->maxDistance = maxInfluentialDistance;
    this->velocityX = Mat::zeros(gridSize.height, gridSize.width, CV_64FC1);
    this->velocityY = Mat::zeros(gridSize.height, gridSize.width, CV_64FC1);
    this->gravityThreshold = gravityThreshold;
    this->canvas    = Mat(gridSize.height * PATCH_MAP_SIZE, gridSize.width * PATCH_MAP_SIZE, CV_8UC3);
  }
  
  virtual ~Grid()
  {
  }

  inline void neutralise(const bool quick = true)
  {
    if (quick)
    {
      for (auto a : this->anchors)
      {
        this->velocityX.at<double>(a.y, a.x) = 0.0;
        this->velocityY.at<double>(a.y, a.x) = 0.0;
      }
    }
    else
    {
      this->velocityX.setTo(Scalar::all(0.0));
      this->velocityY.setTo(Scalar::all(0.0));
    }
    this->anchors.clear();
  }

  inline void setAnchor(const Point2i& anchor, const Point2d& velocity)
  {
    this->velocityX.at<double>(anchor.y, anchor.x) = velocity.x;
    this->velocityY.at<double>(anchor.y, anchor.x) = velocity.y;
    this->anchors.push_back(anchor);
  }

  // Calculate velocity with direction on the specified list of subjects
  virtual vector<tuple<Point2i, Point2d>> calculateVelocity(const vector<Point2i>& ps) const;

  virtual void renderVelocityMap(const string& wndName, const vector<tuple<Point2i, Point2d>>& ps);
};



#endif