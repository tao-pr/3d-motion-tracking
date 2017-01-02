/**
 * Entry point for 3D motion track app
 * @author TaoPR (StarColon.com)
 * @copyRight 2016-now 
 */

#include <functional>
#include "IdentityTransformation.h"
#include "FilterTransformation.h"
#include "VideoCamera.h"
#include "CamShiftTracker.h"
#include "MotionTracker.h"

using namespace std;

bool debug = true;
float meshDisplace = 50;
float maxEdgeLength = 250;
int longestAbsence = 3;

int main(int argc, char** argv)
{
  // Prepare frame transformers
  IdentityTransformation nothing;
  FilterTransformation grayDownsampling = FilterTransformation(
    FilterType::Nothing, 0, 0, 0.43
    );

  // Prepare tracking models
  CamShiftTracker camTrack;
  // function<void (Mat)> tracker = camTrack.track();
  // function<void (Mat)> idle    = [](Mat){};

  
  MotionTracker motionTrack(meshDisplace, maxEdgeLength, longestAbsence, debug);
  function<void (Mat)> tracker = motionTrack.track();
  
  // Start capturing from video source
  VideoCamera cam("cam");
  cam.captureRealtimeWith(grayDownsampling, tracker);
  return 0;
}