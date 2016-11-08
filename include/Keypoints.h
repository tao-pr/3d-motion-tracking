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
  Point update(Point &p);
  Point setAbsence();
  Point predict();

  inline Point get(){ return this->p0; };
  inline Point getPredicted(){ return this->p_; };

private:
  KalmanFilter kf;
  Mat state;
  Mat noise;
  Mat measure;

  // Current position
  Point p0; // Measured
  Point p_; // Predicted

  int absenceLength;

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
  void drawMarkers(Mat& canvas, Scalar colorMeasure, Scalar colorPredict);

protected:
  vector<TrackableKeyPoint> points; // TAOTOREVIEW: Better use KD-Tree ?

private:
  const float MAX_DISTANCE = 25.0; // Max distance to track
  const int LONGEST_ABSENCE = 8; // Longest absence of points before removal
};

#endif