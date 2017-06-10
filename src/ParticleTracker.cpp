#include "ParticleTracker.h"

ParticleTracker::ParticleTracker()
{
  cout << GREEN << "[Tracker]" << RESET << " Initialising SIFT particle tracker..." << endl;
  this->sift = SIFT::create();
}

ParticleTracker::~ParticleTracker()
{
  delete this->sift;
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

function<void (Mat)> ParticleTracker::track()
{
  cout << "[Press Ctrl+c to escape]" << endl;
  auto pipe = [&](Mat im)
  {
    auto points = detectPoints(im);
    DrawUtils::drawMarks(im, points, Scalar(0,50,255));
    imshow("sift", im);

    // TAOTODO: Aligh the detected points 
  };

  return pipe;
}