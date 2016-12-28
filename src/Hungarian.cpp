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
    float mini = Hungarian::minOfRow(n,cost);
    Mat minVec  = Mat(1, nCols, CV_32FC1, Scalar(mini));
    cost.row(n) = cost.row(n) - minVec;
  }

  if (this->debug)
  {
    cout << "...Row subtracted" << endl;
    cout << cost << endl << endl;
  }

  // Col-wise minima subtraction
  for (int n=0; n<nCols; n++)
  {
    float mini = Hungarian::minOfCol(n,cost);
    Mat minVec = Mat(nRows, 1, CV_32FC1, Scalar(mini));
    cost.col(n) = cost.col(n) - minVec;
  }

  if (this->debug)
  {
    cout << "...Col subtracted" << endl;
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

float Hungarian::minOfRow(int i, const Mat& cost)
{
  float m = numeric_limits<float>::max();
  int nCols = cost.cols;
  for (int k=0; k<nCols; k++)
  {
    float v = cost.at<float>(i,k);
    m = v < m ? v : m;
  }
  return m;
}

float Hungarian::minOfCol(int i, const Mat& cost)
{
  float m = numeric_limits<float>::max();
  int nRows = cost.rows;
  for (int k=0; k<nRows; k++)
  {
    float v = cost.at<float>(k,i);
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
  // Initialisation
  set<int> coverRowLines;
  set<int> coverColLines;
  unordered_map<int, int> profileRow; // { j => num of zeroes in the row}
  unordered_map<int, int> profileCol; // { i => num of zeroes in the col}
  vector<Point> uncoveredZeroes;

  int numRows = m.rows;
  int numCols = m.cols;

  // Locate all zeroes
  for (int j=0; j<numRows; j++)
    for (int i=0; i<numCols; i++)
      if (m.at<float>(j,i) <= 1e-4) // Zero?
      {
        // Record the profiling lines
        if (profileRow.find(j) == profileRow.end())
          profileRow.insert(make_pair(j,1));
        else
          profileRow[j]++;

        if (profileCol.find(i) == profileCol.end())
          profileCol.insert(make_pair(i,1));
        else 
          profileCol[i]++;

        // Record uncovered zeroes
        uncoveredZeroes.push_back(Point(j,i));
      }

  if (debug)
  {
    cout << "...[Zero profiles] << endl";
    for (auto pj : profileRow)
      cout << "......Row #" << get<0>(pj) << " : " << get<1>(pj);
    cout << endl;
    for (auto pi : profileCol)
      cout << "......Col #" << get<0>(pi) << " : " << get<1>(pi);
  }

  // Convert profiles to priority queues
  auto comparer = [](tuple<int,int> a, tuple<int,int> b)
  {
    return get<1>(a) < get<1>(b);
  };
  priority_queue<tuple<int,int>,,> qRows;
  priority_queue<tuple<int,int>,,> qCols;
  for (auto pj : profileRow)
  {
    int j = get<0>(pj);
    int size = get<1>(pj);
    qRows.push(make_tuple(j,size));
  }
  for (auto pi : profileRow)
  {
    int i = get<0>(pi);
    int size = get<1>(pi);
    qRows.push(make_tuple(i,size));
  }

  // Repeatedly add more cover lines
  // Until there is no uncovered zeroes left
  while (uncoveredZeroes.size()>0)
  {
    
  }




  // TAOTODO:

  // All cover lines pruned!
  return make_tuple(coverRowLines, coverColLines);
};

