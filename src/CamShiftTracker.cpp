#include "CamShiftTracker.h"

function<void (Mat)> CamShiftTracker::track()
{
  auto pipe = [&](Mat im)
  {
    // TAOTODO: Track!

    cout << "Tracking ..." << endl;
  };

  return pipe;
}

