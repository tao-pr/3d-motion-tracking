/**
 * Entry point for 3D motion track app
 * @author TaoPR (StarColon.com)
 * @copyRight 2016-now 
 */

#include <functional>
#include "IdentityTransformation.h"
#include "VideoCamera.h"
#include "CamShiftTracker.h"

using namespace std;

int main(int argc, char** argv)
{
  // Prepare frame transformers
  IdentityTransformation nothing;

  // Prepare tracking models
  CamShiftTracker camTrack;
  function<void (Mat)> tracker = camTrack.track();
  function<void (Mat)> idle    = [](Mat){};
  
  // Start capturing from video source
  VideoCamera cam("cam");
  cam.captureRealtimeWith(nothing, tracker);
  return 0;
}