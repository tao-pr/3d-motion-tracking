#include "ParticleTracker.h"

ParticleTracker::ParticleTracker()
{
  this->sift = SIFT::create();
}

vector<Point2f> ParticleTracker::detectPoints(Mat &in)
{
  vector<Point2f> points;
  vector<KeyPoint> keypoints;
  Mat descriptors;
  this->sift->detectAndCompute(in, noArray(), keypoints, descriptors);
  KeyPoint::convert(keypoints, points);
  return points;
}