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

  tuple<set<int>, set<int>> zeroes = Hungarian::coverZeros(m, true);
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

  tuple<set<int>, set<int>> zeroes = Hungarian::coverZeros(m, true);
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

  tuple<set<int>, set<int>> zeroes = Hungarian::coverZeros(m, true);
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

TestScenario testHungarian0 = TestScenario("Hungarian");
TestScenario testHungarian = testHungarian0
  >> TestCase("[case 1] - Cover zeroes", caseCoverZeros)
  >> TestCase("[case 2] - Cover zeroes (non-zeroes)", caseCoverNoZeros)
  >> TestCase("[case 3] - Cover zeroes (larger matrix)", caseCoverZerosLargeMat);

#endif