#include "Alignment.h"

Alignment::Alignment(function<double (Point2f, Point2f)> measureDistance, function<double (Mat, Mat)> measureFeatureSimilarity, double maxMoveDistance)
{
  cout << GREEN << "Initialising alignment engine..." << RESET << endl;
  const int M = VIS_PATCH_SIZE * VIS_MAX_SPOT;
  this->measureDistFunction       = measureDistance;
  this->measureSimilarityFunction = measureFeatureSimilarity;
  this->maxDistance               = maxMoveDistance;
  this->isVisualisationOn         = false;
  this->vis                       = Mat(M, M, CV_8UC3, Scalar(0,0,0));
}

void Alignment::setVisualisation(bool on)
{
  this->isVisualisationOn = on;
}

void Alignment::drawVisCell(int i, int j, double score)
{
  auto color = Scalar(0,0,int(floor(255*score)));
  rectangle(
    this->vis,
    Point2f(i*VIS_PATCH_SIZE, j*VIS_PATCH_SIZE),
    Point2f((i+1)*VIS_PATCH_SIZE-1, (j-1)*VIS_PATCH_SIZE-1),
    color, 
    -1
  );
}

void Alignment::redrawVis(const Mat& matchScore)
{
  for (int i=0; i<VIS_MAX_SPOT && i<matchScore.rows; i++)
    for (int j=0; j<VIS_MAX_SPOT && j<matchScore.cols; j++)
    {
      drawVisCell(i, j, matchScore.at<double>(i,j));
    }

  imshow("tracking scores", matchScore);
}

void Alignment::align(vector<Point2f> basepoints, vector<Point2f> newpoints, const Mat* baseFeatures, const Mat* newFeatures)
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
      int d = get<1>(c);
      if (d > this->maxDistance)
        matchScore.at<float>(i,j) = 0;
      else 
        matchScore.at<float>(i,j) = 1/d;
    }

    if (this->isVisualisationOn)
    {
      Mat forVis = Mat(matchScore.rows, matchScore.cols, CV_32FC1, Scalar(0));
      normalize(matchScore, forVis, 255, 0);
      this->redrawVis(forVis);
    }

    // TAOTODO:

    i++;
  }
}