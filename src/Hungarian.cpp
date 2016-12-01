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

Mat Hungarian::coverZeros(Mat& m)
{
  unordered_map<int, vector<int>> zeroRow; // {row id => indexes in master}
  unordered_map<int, vector<int>> zeroCol; // {col id => indexes in master}
  vector<tuple<int, int>> master;


  // Find all zeroes
  int numRows = m.rows;
  int numCols = m.cols;
  for (int j=0; j<numRows; j++)
    for (int i=0; i<numCols; i++)
      if (m.at<double>(j,i) == 0)
      {
        int idx = master.size();
        master.push_back({j, i});

        vector<int> idxVec = {idx};

        if (zeroRow.find(j) == zeroRow.end())
          zeroRow.insert(make_pair(j, idxVec));
        else
          zeroRow[j].push_back(idx);

        if (zeroCol.find(i) == zeroCol.end())
          zeroCol.insert(make_pair(i, idxVec));
        else
          zeroCol[i].push_back(idx);
      }

  // TAOTODO:
  // Find minimum number of lines to cover all those zeroes
  vector<int> lineRows;
  vector<int> lineCols;

  // Row with most zeroes

  // Col with most zeroes

}

