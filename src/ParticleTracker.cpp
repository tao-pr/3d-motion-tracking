#include "ParticleTracker.h"

ParticleTracker::ParticleTracker()
{
  cout << GREEN << "[Tracker]" << RESET << " Initialising SIFT particle tracker..." << endl;
  
  auto featureSimilarityMeasure = [](Mat m1, Mat m2){ return 0; }; //TAOTODO:
  const double maxDisplacement = 30;

  this->sift         = SIFT::create();
  this->alignment    = new Alignment(_dist, featureSimilarityMeasure, maxDisplacement);
  this->alignment->setVisualisation(true);
  this->prevFeatures = nullptr;
}

ParticleTracker::~ParticleTracker()
{
  cout << CYAN << "[Tracker]" << RESET << " Tearing down SIFT particle tracker..." << endl;
  delete this->sift;
  delete this->alignment;
  delete this->prevFeatures;
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
    trackFeatures(im);
  };

  return pipe;
}

void ParticleTracker::trackFeatures(Mat &im)
{
  auto points = detectPoints(im);
  Mat features = Mat(1,1,CV_32FC1, Scalar(0));
  DrawUtils::drawMarks(im, points, Scalar(0,50,255));
  imshow("sift", im);

  // TAOTODO: Aligh the detected points 
  if (!this->prevPoints.empty())
  {
    cout << "... " << points.size() << " feature points tracked" << endl;
    alignment->align(prevPoints, points, prevFeatures, &features);
  }
  
  // Store the points
  this->prevPoints.clear();
  for (auto p : points)
    this->prevPoints.push_back(p);
}