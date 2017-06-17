#include "ParticleTracker.h"

ParticleTracker::ParticleTracker()
{
  cout << GREEN << "[Tracker]" << RESET << " Initialising SIFT particle tracker..." << endl;
  const double maxDisplacement = 70;

  this->sift         = SIFT::create();
  this->alignment    = new Alignment(_dist, maxDisplacement);
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
  #ifdef DRAW_ALL_POINTS
  DrawUtils::drawMarks(im, points, Scalar(0,50,255));
  #endif
  
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
      if (_dist(prevPoints[i], points[j]) >= MIN_DISTANCE_TO_DRAW_TRAIL)
        line(im, prevPoints[i], points[j], Scalar(0,50,255), 1, CV_AA);
      #ifndef DRAW_ALL_POINTS
      DrawUtils::drawSpot(im, points[j], Scalar(0,50,255));
      #endif
    }

    // Highlight new points with GREEN (without matched previous points)
    #ifdef DRAW_NEW_POINTS_GREEN
    for (int j=0; j<points.size(); j++)
    {
      if (trackedPoints.find(j) == trackedPoints.end())
        DrawUtils::drawSpot(im, points[j], Scalar(0,255,50));
    }
    #endif

    #ifdef DEBUG_ALIGNMENT
    cout << "... " << points.size() << " feature points (" 
         << trackedPoints.size() << " tracked)" << endl;
    #endif
  }

  imshow("sift", im);
  
  // Store the points
  this->prevPoints.clear();
  for (auto p : points)
    this->prevPoints.push_back(p);
  // Store the features
  features.copyTo(this->prevFeatures);
}