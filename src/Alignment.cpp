#include "Alignment.h"

Alignment::Alignment(function<double (Point2f, Point2f)> measureDistance, function<double (Mat, Mat)> measureFeatureSimilarity, double maxMoveDistance)
{
  cout << GREEN << "Initialising alignment engine..." << RESET << endl;
  this->measureDistFunction       = measureDistance;
  this->measureSimilarityFunction = measureFeatureSimilarity;
  this->maxDistance               = maxMoveDistance;
  this->isVisualisationOn         = false;
}

void Alignment::setVisualisation(bool on)
{
  this->isVisualisationOn = on;
}

void Alignment::align(vector<Point2f> basepoints, vector<Point2f> newpoints, const Mat baseFeatures, const Mat newFeatures)
{
  int i = 0;
  int M = VIS_MAX_SPOT * VIS_PATCH_SIZE + 1;
  Mat matchScore = Mat::zeros(basepoints.size(), newpoints.size(), CV_64FC1);
  Mat vis = Mat::zeros(M, M, CV_8UC3);
  for (auto bp : basepoints)
  {
    // List of Tuples of <distance, index of candidate>
    // Closest first
    priority_queue<distanceToIndex, vector<distanceToIndex>, compareDistance> candidates;

    // Measure all candidates within the perimeter
    int j = 0;
    for (auto np : newpoints)
    {
      double d = this->measureDistFunction(bp, np);
      if (d > this->maxDistance)
      {
        candidates.push(make_tuple(j, 0));
        matchScore.at<double>(i,j) = 0;
      }
      else
      {
        double d_ = 1/d;
        auto v0 = baseFeatures.row(i);
        auto v1 = newFeatures.row(j);
        double summag = sum(v0)[0] + sum(v1)[0];
        double similarity = v0.dot(v1)/summag;

        candidates.push(make_tuple(j, d_ * similarity));
        matchScore.at<double>(i,j) = d_;
        
        if (this->isVisualisationOn)
        {
          int v   = floor(d_*255);
          auto p0 = Point2f(i*VIS_PATCH_SIZE, j*VIS_PATCH_SIZE);
          auto p1 = Point2f((i+1)*VIS_PATCH_SIZE-1, (j+1)*VIS_PATCH_SIZE-1);
          auto c  = Scalar(0,0,int(v));
          rectangle(vis, p0, p1, c, -1);
        }
      }
      j++;
    }

    // TAOTODO:

    if (this->isVisualisationOn)
    {
      imshow("tracking score", vis); 
    }


    i++;
  }
}