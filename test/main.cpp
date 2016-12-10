/**
 * Simple test suite
 * @author Tao PR (github.com/starcolon)
 */

#include <functional>
#include <queue>
#include "IdentityTransformation.h"
#include "FilterTransformation.h"
#include "VideoCamera.h"
#include "CamShiftTracker.h"
#include "MotionTracker.h"

#include "TestHungarian.h"

using namespace std;


int main(int argc, char** argv)
{
  vector<TestScenario> tests = {
    TestHungarian()
  };

  // Prepare test cases
  cout << "[3d-motion-track]" << endl;

  
}
