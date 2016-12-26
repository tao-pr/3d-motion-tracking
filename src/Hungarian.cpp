#include "Hungarian.h"

Hungarian::Hungarian(const Mat& cost, bool debug=false)
{
  this->debug = debug;
  this->costM = cost.clone();
}

vector<tuple<int, int>> Hungarian::optimiseMinima() const
{
  vector<tuple<int, int>> minima;

  int nRows = this->costM.rows;
  int nCols = this->costM.cols;

  Mat cost = this->costM.clone();

  // Row-wise minima subtraction
  for (int n=0; n<nRows; n++)
  {
    float mini = minOfRow(n);
    Mat minVec  = Mat(1, nCols, CV_32FC1, Scalar(mini));
    cost.row(n) = cost.row(n) - minVec;
  }

  // Col-wise minima subtraction
  for (int n=0; n<nCols; n++)
  {
    float mini = minOfCol(n);
    Mat minVec = Mat(nRows, 1, CV_32FC1, Scalar(mini));
    cost.col(n) = cost.col(n) - minVec;
  }

  if (this->debug)
  {
    cout << "...Col & Row subtracted" << endl;
    cout << cost << endl << endl;
  }

  // Cover all zeroes in the matrix 
  // The minimum number of lines to cover must equal to the dimension
  int iter = 0;
  while (true && iter<MAX_ITER)
  {
    tuple<set<int>, set<int>> zeroes = Hungarian::coverZeros(cost, this->debug);
    set<int> zeroRows = get<0>(zeroes);
    set<int> zeroCols = get<1>(zeroes);
    if (debug){
      cout << cost << endl << endl;
      cout << "...Cover zeroes : expected {" << nRows 
        << "} got {" << zeroRows.size() + zeroCols.size() << "}" << endl;
    }
    if (zeroRows.size() + zeroCols.size() == nRows)
      break;

    if (this->debug){
      cout << "...Creating additional zeroes" << endl;
    }
    Hungarian::createAdditionalZeros(cost, zeroes, this->debug);
    iter++;
  }

  // Locate minima
  for (int j=0; j<nRows; j++)
    for (int i=0; i<nCols; i++)
    {
      if (this->costM.at<float>(j,i) == 0)
        minima.push_back(make_tuple(j,i));
    }

  return minima;
}

float Hungarian::minOfRow(int i) const
{
  float m = numeric_limits<float>::max();
  int nCols = this->costM.cols;
  for (int k=0; k<nCols; k++)
  {
    float v = this->costM.at<float>(i,k);
    m = v < m ? v : m;
  }
  return m;
}

float Hungarian::minOfCol(int i) const
{
  float m = numeric_limits<float>::max();
  int nRows = this->costM.rows;
  for (int k=0; k<nRows; k++)
  {
    float v = this->costM.at<float>(k,i);
    m = v < m ? v : m;
  }
  return m;
}

void Hungarian::createAdditionalZeros(Mat& m, tuple<set<int>, set<int>> zeroes, bool debug=false)
{
  // Find minimum uncovered number in {m}
  float minVal = numeric_limits<float>::max();
  int nRows = m.rows;
  int nCols = m.cols;

  set<int> zeroRows = get<0>(zeroes);
  set<int> zeroCols = get<1>(zeroes);

  for (int j=0; j<nRows; j++)
    for (int i=0; i<nCols; i++)
    {
      if (zeroRows.find(j) != zeroRows.end() || 
        zeroCols.find(i) != zeroCols.end())
        continue;
      float u = m.at<float>(j,i);
      if (u<minVal) minVal = u;
    }

  if (debug)
  {
    cout << "Minimum uncovered : " << minVal << endl;
  }

  // Subtract all uncovered numbers with {min}
  // Add {min} to double-covered numbers
  for (int j=0; j<nRows; j++)
    for (int i=0; i<nCols; i++)
    {
      auto rowCovered = zeroRows.find(j) != zeroRows.end();
      auto colCovered = zeroCols.find(i) != zeroCols.end();

      // Double-covered
      if (rowCovered && colCovered)
        m.at<float>(j,i) += minVal;
      // Uncovered
      else if (!(rowCovered || colCovered))
        m.at<float>(j,i) -= minVal;
    }

  if (debug)
  {
    cout << "Additional Zeroes:" << endl;
    cout << m << endl << endl;
  }

}

tuple<set<int>, set<int>> Hungarian::coverZeros(Mat& m, bool debug=false)
{
  // Positions of all zeroes
  unordered_map<int, vector<int>> zeroRow; // {row id => indexes in master}
  unordered_map<int, vector<int>> zeroCol; // {col id => indexes in master}
  vector<tuple<int, int>> master;

  // C++ {set} is an implementation of Red-Black tree
  // so it's an efficient candidate for this job \\O//
  set<int> lineRows; // list of row ids to cover 
  set<int> lineCols; // list of col ids to cover

  // Find all zeroes
  int numRows = m.rows;
  int numCols = m.cols;
  for (int j=0; j<numRows; j++)
    for (int i=0; i<numCols; i++)
      if (m.at<float>(j,i) <= 1e-4) // Zero?
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

        // Add lines to cover, both horizontal and vertical
        if (lineRows.find(j)==lineRows.end())
          lineRows.insert(j);
        if (lineCols.find(i)==lineCols.end())
          lineCols.insert(i);
      }

  if (debug)
  {
    // Print out detected zeroes
    cout << "~zeros~" << endl;
    for (auto z : master)
    {
      cout << get<0>(z) << ", " << get<1>(z) << endl;
    }
  }

  // Pruning cover lines:
  // Iterate and remove the lines which won't lose any covered zeros
  for (auto zero : master)
  {
    int j = get<0>(zero);
    int i = get<1>(zero);
    int nRowNeighbors = zeroRow[j].size();
    int nColNeighbors = zeroCol[i].size();
    if (nRowNeighbors==1 && nColNeighbors>1) 
    {
      // If [row] can be removed, there still other lines cover it
      lineRows.erase(j);
    }
    else if (nRowNeighbors>1 && nColNeighbors==1)
    {
      // If [col] can be removed, there still other lines cover it
      lineCols.erase(i);
    }
    // Otherwise, neither of the lines could be removed.
    // leave them
  }
  
  // All cover lines pruned!
  return make_tuple(lineRows, lineCols);
};

