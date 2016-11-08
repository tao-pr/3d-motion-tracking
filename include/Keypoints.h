#ifndef KEYPOINTS
#define KEYPOINTS

#include <vector>
#include <math.h>
#include <functional>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cvaux.hpp>
#include "DrawUtils.h"

// A single keypoint to track
class TrackableKeyPoint
{
public:
  TrackableKeyPoint(Point &p);
  // Update a new position
  void update(Point &p);
  Point predict();

private:
  KalmanFilter kf;
  Mat state;
  Mat noise;
  Mat measure;

  const int DIMENSION_OF_STATE  = 4; // [x,y,dx/dt,dy/dt]
  const int DIMENSION_OF_MEASUREMENT = 2; // [x,y]
  const int DIMENSION_OF_CONTROLVECTOR = 0;
};

// A set of sparse keypoints to track
class Mesh
{
public:
  Mesh(vector<Point> &ps);
  void update(vector<Point> &ps);

protected:
  vector<TrackableKeyPoint> points; // TAOTOREVIEW: Better use KD-Tree ?
  
};

#endif