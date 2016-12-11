#ifndef TEST_HUNGARIAN
#define TEST_HUNGARIAN

#include <queue>
#include <functional>
#include "Hungarian.h"
#include "common.h"
#include "coloriser.h"

using namespace std;
using namespace cv;

// extern string RED;
// extern string GREEN;
// extern string YELLOW;
// extern string CYAN;
// extern string MAGENTA;
// extern string RESET;

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
  

  return true;
};

TestScenario testHungarian0 = TestScenario("Hungarian");
TestScenario testHungarian = testHungarian0
  >> TestCase("[case 1] - Cover zeroes", caseCoverZeros)
  >> TestCase("[case 2]", [](){ return true; });

#endif