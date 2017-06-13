#include "Alignment.h"

Alignment::Alignment(function<double (Point2f, Point2f)> measureDistance, function<double (Mat, Mat)> measureFeatureSimilarity, double maxMoveDistance)
{
  this->measureDistFunction       = measureDistance;
  this->measureSimilarityFunction = measureFeatureSimilarity;
  this->maxDistance               = maxMoveDistance;
}

void Alignment::align(vector<Point2f> basepoints, vector<Point2f> newpoints, const Mat& baseFeatures, const Mat& newFeatures) const
{
  int i = 0;
  Mat matchScore = Mat(basepoints.size(), newpoints.size(), CV_32FC1, Scalar(1e32));
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
      candidates.push(make_tuple(j, d));
      j++;
    }

    // Take only top [N] closest new points
    const int N = 3;
    queue<distanceToIndex> q;
    while (!candidates.empty() && q.size()<N)
    {
      auto c = candidates.top();
      q.push(c);
      // TAOTODO: Measure similarity between [baseFeature[i]] vs [newFeature[j]]

      // Record the score (invert distance)
      int j = get<0>(c);
      matchScore.at<float>(i,j) = 1/get<1>(c);
    }

    // TAOTODO:

    i++;
  }
}