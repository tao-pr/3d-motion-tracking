#ifndef TEST_HUNGARIAN
#define TEST_HUNGARIAN

#include <queue>
#include <functional>
#include "Hungarian.h"
#include "common.h"
#include "coloriser.h"

using namespace std;
using namespace cv;


function<bool()> caseCoverZeros = []()
{
  float vecM[9] = {0, 1, 0, 1, 3, 4, 2, 5, 0};
  Mat m         = Mat(3, 3, CV_32F, vecM);

  cout << m << endl << endl;

  tuple<set<int>, set<int>> zeroes = Hungarian::coverZeroes(m, true);
  set<int> lineRows = get<0>(zeroes);
  set<int> lineCols = get<1>(zeroes);

  cout << "rows: " << endl;
  for (auto j : lineRows) 
    cout << j << endl;

  cout << "cols: " << endl;
  for (auto i : lineCols)
    cout << i << endl;

  // ASSERT
  set<int> expectedRows = {0};
  set<int> expectedCols = {2};
  return isEql(lineRows, expectedRows) && isEql(lineCols, expectedCols);
};

function<bool()> caseCoverNoZeros = []()
{
  float vecM[16] = {1, 2, 1, 3, 2, 2, 1, 1, 3, 1, 5, 1, 2, 1, 5, 3};
  Mat m          = Mat(4, 4, CV_32F, vecM);

  cout << m << endl << endl;

  tuple<set<int>, set<int>> zeroes = Hungarian::coverZeroes(m, true);
  set<int> lineRows = get<0>(zeroes);
  set<int> lineCols = get<1>(zeroes);

  cout << "rows: " << endl;
  for (auto j : lineRows) 
    cout << j << endl;

  cout << "cols: " << endl;
  for (auto i : lineCols)
    cout << i << endl;

  // ASSERT
  set<int> expectedRows;
  set<int> expectedCols;
  return isEql(lineRows, expectedRows) && isEql(lineCols, expectedCols);
};

function<bool()> caseCoverZerosLargeMat = []()
{
  float vecM[25] = {
    1, 2, 1, 0, 3,
    3, 0, 1, 5, 1,
    1, 0, 3, 1, 2,
    0, 3, 2, 2, 4,
    0, 0, 3, 0, 4
  };
  Mat m = Mat(5, 5, CV_32F, vecM);

  cout << m << endl << endl;

  tuple<set<int>, set<int>> zeroes = Hungarian::coverZeroes(m, true);
  set<int> lineRows = get<0>(zeroes);
  set<int> lineCols = get<1>(zeroes);

  cout << "rows: " << endl;
  for (auto j : lineRows) 
    cout << j << endl;

  cout << "cols: " << endl;
  for (auto i : lineCols)
    cout << i << endl;

  // ASSERT
  set<int> expectedRows = {4};
  set<int> expectedCols = {0,1,3};
  return isEql(lineRows, expectedRows) && isEql(lineCols, expectedCols);
};

function<bool()> caseCoverZerosSparse = []()
{
  float vec[25] = {
    0, 0, 1, 5, 3,
    0, 3, 1, 0, 4,
    1, 0, 0, 0, 1,
    0, 0, 0, 1, 3,
    0, 0, 8, 0, 0
  };
  Mat m = Mat(5, 5, CV_32F, vec);
  cout << m << endl << endl;

  tuple<set<int>, set<int>> zeroes = Hungarian::coverZeroes(m, true);
  set<int> lineRows = get<0>(zeroes);
  set<int> lineCols = get<1>(zeroes);

  // ASSERT
  set<int> expectedRows = {2,3,4};
  set<int> expectedCols = {0,1,3};
  return isEql(lineRows, expectedRows) && isEql(lineCols, expectedCols);
};

function<bool()> caseAdditionalZero = []()
{
  float vec[9] = {
    0, 3, 1,
    2, 4, 1,
    3, 1, 0 // This will need only 2 lines to cover
  };
  Mat m0 = Mat(3, 3, CV_32F, vec);
  cout << m0 << endl << endl;
  Mat m = m0.clone();

  tuple<set<int>, set<int>> zeroes = Hungarian::coverZeroes(m, false);

  cout << "...[Cover rows:]";
  for (int j : get<0>(zeroes))
    cout << j << ",";
  cout << endl << "...[Cover cols:]";
  for (int i : get<1>(zeroes))
    cout << i << ",";
  cout << endl;

  Hungarian::createAdditionalZeros(m, zeroes, true);



  // TAOTODO:
  return false;
};

function<bool()> caseOptimise = []()
{
  float vec[9] = {
    1, 10, 3,
    5, 7, 4,
    5, 10, 7
  };
  Mat m = Mat(3, 3, CV_32F, vec);
  cout << m << endl << endl;

  Hungarian hungarian(m, true);
  vector<tuple<int,int>> minima = hungarian.optimiseMinima();

  // TAOTODO:
  cout << YELLOW << "[Minima]" << endl;
  for (tuple<int,int> p : minima)
  {
    int j = get<0>(p);
    int i = get<1>(p);
    printf("...(%d, %d) : %0.0f\n", j, i, m.at<float>(j,i));
  }

  // ASSERT

  return false;
};

TestScenario testHungarian0 = TestScenario("Hungarian");
TestScenario testHungarian = testHungarian0
  >> TestCase("[case 1] - Cover zeroes", caseCoverZeros)
  >> TestCase("[case 2] - Cover zeroes (non-zeroes)", caseCoverNoZeros)
  >> TestCase("[case 3] - Cover zeroes (larger matrix)", caseCoverZerosLargeMat)
  >> TestCase("[case 4] - Cover zeroes (sparse matrix)", caseCoverZerosSparse)
  >> TestCase("[case 5] - Create additional zeroes", caseAdditionalZero)
  >> TestCase("[case _] - Minima", caseOptimise);

#endif