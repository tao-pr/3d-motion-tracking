#ifndef KEYPOINTS
#define KEYPOINTS

#include <queue>
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
  inline int getAbsence(){ return this->absenceLength; };

private:
  KalmanFilter kf;
  Mat state;
  Mat noise;
  Mat measure;

  // Current position
  Point p0; // Measured // TAOTODO: Nullable<>
  Point p_; // Predicted

  int absenceLength; // Number of consecutive absent frames

  const int DIMENSION_OF_STATE  = 4; // [x,y,dx/dt,dy/dt]
  const int DIMENSION_OF_MEASUREMENT = 2; // [x,y]
  const int DIMENSION_OF_CONTROLVECTOR = 0;
};

#define SQR(n)    n*n
#define DIST(a,b) sqrt(SQR(a.x - b.x) + SQR(a.y - b.y))



#endif