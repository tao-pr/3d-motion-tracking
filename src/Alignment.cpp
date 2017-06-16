#include "Alignment.h"

Alignment::Alignment(function<double (Point2f, Point2f)> measureDistance, function<double (Mat, Mat)> measureFeatureSimilarity, double maxMoveDistance)
{
  cout << GREEN << "Initialising alignment engine..." << RESET << endl;
  const int M = VIS_PATCH_SIZE * VIS_MAX_SPOT + 1;
  this->measureDistFunction       = measureDistance;
  this->measureSimilarityFunction = measureFeatureSimilarity;
  this->maxDistance               = maxMoveDistance;
  this->isVisualisationOn         = false;
  this->vis                       = Mat::zeros(M, M, CV_8UC3);
}

void Alignment::setVisualisation(bool on)
{
  this->isVisualisationOn = on;
}

void Alignment::drawVisCell(int i, int j, double score)
{
  auto color = Scalar(0,0,int(score));
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
  Mat matchScore = Mat::zeros(basepoints.size(), newpoints.size(), CV_64FC1);
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
        candidates.push(make_tuple(j, 1/d));
        matchScore.at<double>(i,j) = 1/d;
      }
      j++;
    }

    if (this->isVisualisationOn)
    {
      Mat forVis = Mat::zeros(matchScore.rows, matchScore.cols, CV_64FC1);
      normalize(matchScore, forVis, double(255.0), double(0.0));
      this->redrawVis(forVis);
    }

    // TAOTODO:

    i++;
  }
}