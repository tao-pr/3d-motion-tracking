#include "Hungarian.h"

bool CompareProfile::operator()(Profile &a, Profile &b )
{
  return get<1>(a) < get<1>(b);
}

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
    tuple<set<int>, set<int>> zeroes = Hungarian::coverZeroes(cost, this->debug);
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

tuple<set<int>, set<int>> Hungarian::coverZeroes(Mat& m, bool debug=false)
{
  // Initialisation
  set<int> coverRowLines;
  set<int> coverColLines;
  unordered_map<int, int> profile;
  vector<Point> uncoveredZeroes;

  int numRows = m.rows;
  int numCols = m.cols;

  // Locate all zeroes
  for (int j=0; j<numRows; j++)
    for (int i=0; i<numCols; i++)
      if (m.at<float>(j,i) <= 1e-4) // Zero?
      {
        // Record the profiling lines
        if (profile.find(j) == profile.end())
          profile.insert(make_pair(j,1));
        else
          profile[j]++;

        if (profile.find(-i) == profile.end())
          profile.insert(make_pair(-i,1));
        else 
          profile[-i]++;

        // Record uncovered zeroes
        uncoveredZeroes.push_back(Point(i,j));
      }

  // Convert profiles to priority queue
  priority_queue<Profile,vector<Profile>,CompareProfile> q0;
  priority_queue<Profile,vector<Profile>,CompareProfile> q;
  for (auto p : profile)
  {
    int k = get<0>(p);
    int mag = get<1>(p);
    q.push(make_tuple(k,mag));
    q0.push(make_tuple(k,mag));
  }

  if (debug)
  {
    cout << "...[Zero profiles] << endl";
    while (!q0.empty())
    {
      auto p = q0.top();
      if (get<0>(p) < 0)
        cout << "......Col #" << -get<0>(p) << " : " << get<1>(p);
      else
        cout << "......Row #" << get<0>(p) << " : " << get<1>(p);
      q0.pop();
    }
  }


  // Repeatedly add more cover lines (greedy-algo-based)
  // Until there is no uncovered zeroes left
  int nAttempt = 1;
  while (!uncoveredZeroes.empty())
  {
    if (debug) printf("...[Attempt #%d] %zu uncovered remaining", nAttempt, uncoveredZeroes.size());

    // Find the next profile line with most zeroes aligned within
    if (q.size()>0)
    {
      // Take the next
      Profile p = q.top();

    }

    nAttempt++;
  }




  // TAOTODO:

  // All cover lines pruned!
  return make_tuple(coverRowLines, coverColLines);
};

