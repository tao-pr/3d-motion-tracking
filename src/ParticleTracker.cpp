#include "ParticleTracker.h"

ParticleTracker::ParticleTracker()
{
  cout << GREEN << "[Tracker]" << RESET << " Initialising SIFT particle tracker..." << endl;
  
  auto featureSimilarityMeasure = [](Mat m1, Mat m2){ return 0; }; //TAOTODO:
  const double maxDisplacement = 70;

  this->sift         = SIFT::create();
  this->alignment    = new Alignment(_dist, featureSimilarityMeasure, maxDisplacement);
  this->alignment->setVisualisation(true);
}

ParticleTracker::~ParticleTracker()
{
  cout << CYAN << "[Tracker]" << RESET << " Tearing down SIFT particle tracker..." << endl;
  delete this->sift;
  delete this->alignment;
}

tuple<vector<Point2f>,Mat> ParticleTracker::detectPoints(Mat &in)
{
  vector<Point2f> points;
  vector<KeyPoint> keypoints;
  Mat descriptors;
  this->sift->detectAndCompute(in, noArray(), keypoints, descriptors);
  KeyPoint::convert(keypoints, points);
  return make_tuple(points, descriptors);
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
  auto pointsAndFeatures = detectPoints(im);
  auto points = get<0>(pointsAndFeatures);
  Mat features = get<1>(pointsAndFeatures);
  // Draw all detected points as RED
  DrawUtils::drawMarks(im, points, Scalar(0,50,255));
  
  if (!this->prevPoints.empty())
  {
    auto pairs = alignment->align(prevPoints, points, prevFeatures, features);
    set<int> trackedPoints;

    for (auto pair : pairs)
    {
      // Draw connecting line between two aligned points
      int i = pair.first;
      int j = pair.second;
      trackedPoints.insert(j);
      line(im, prevPoints[i], points[j], Scalar(0,50,255), 1, CV_AA);
    }

    // Highlight new points with GREEN (without matched previous points)
    for (int j=0; j<points.size(); j++)
    {
      if (trackedPoints.find(j) == trackedPoints.end())
        DrawUtils::drawSpot(im, points[j], Scalar(0,255,50));
    }

    cout << "... " << points.size() << " feature points (" 
         << trackedPoints.size() << " tracked)" << endl;
  }

  imshow("sift", im);
  
  // Store the points
  this->prevPoints.clear();
  for (auto p : points)
    this->prevPoints.push_back(p);
  // Store the features
  features.copyTo(this->prevFeatures);
}