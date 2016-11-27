#include "Hungarian.h"

Hungarian::Hungarian(const Mat& cost)
{
  this->costM = cost.clone();
}

vector<tuple<int, int>> Hungarian::optimiseMinima() const
{
  vector<tuple<int, int>> minima;

  int nRows = this->costM.rows;
  int nCols = this->costM.cols;

  Mat cost = this->costM.clone();

  // Row-wise minima subtraction
  cout << "...Row subtraction" << endl;
  for (int n=0; n<nRows; n++)
  {
    double mini = minOfRow(n);
    Mat minVec  = Mat(1, nCols, CV_32FC1, Scalar(mini));
    cost.row(n) = cost.row(n) - minVec;
  }

  // Col-wise minima subtraction
  cout << "...Col subtraction" << endl;
  for (int n=0; n<nCols; n++)
  {
    double mini = minOfCol(n);
    Mat minVec = Mat(nRows, 1, CV_32FC1, Scalar(mini));
    cost.col(n) = cost.col(n) - minVec;
  }

  // TAOTODO:


  return minima;
}

double Hungarian::minOfRow(int i) const
{
  double m = numeric_limits<double>::max();
  int nCols = this->costM.cols;
  for (int k=0; k<nCols; k++)
  {
    double v = this->costM.at<double>(i,k);
    m = v < m ? v : m;
  }
  return m;
}

double Hungarian::minOfCol(int i) const
{
  double m = numeric_limits<double>::max();
  int nRows = this->costM.rows;
  for (int k=0; k<nRows; k++)
  {
    double v = this->costM.at<double>(k,i);
    m = v < m ? v : m;
  }
  return m;
}

