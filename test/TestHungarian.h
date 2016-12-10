#ifndef TEST_HUNGARIAN
#define TEST_HUNGARIAN

#include <queue>
#include "Hungarian.h"
#include "common.h"

using namespace std;
using namespace cv;

TestScenario testHungarian0 = TestScenario("Hungarian");
TestScenario testHungarian = testHungarian0
  >> TestCase("[case 1]", [](){ return true; })
  >> TestCase("[case 2]", [](){ return true; });

#endif