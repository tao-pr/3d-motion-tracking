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
#include "SimpleFeaturePointTracker.h"
#include "ParticleTracker.h"
#include "ITracker.h"

using namespace std;

bool debug              = true;
string trackerModelName = "particle";


ITracker* trackerModel = nullptr;

function<void (Mat)> createTracker(string modelName)
{
  if (modelName == "camshift")
  {
    trackerModel = new CamShiftTracker();
    return trackerModel->track();
  }
  else if (modelName == "simple")// Good feature to track -based
  {
    float meshDisplace  = 50;
    float maxEdgeLength = 420;
    int longestAbsence  = 3;
    trackerModel        = new SimpleFeaturePointTracker(meshDisplace, maxEdgeLength, longestAbsence, debug);
    return trackerModel->track();
  }
  else if (modelName == "particle") // SIFT based
  {
    trackerModel = new ParticleTracker();
    return trackerModel->track();
  }
  else
  {
    cout << YELLOW << "[WARNING] Unsupported tracker model is specified." << RESET << endl;
    cout << YELLOW << "[WARNING] No tracker will be created." << RESET << endl;
    return [](Mat){}; // Do nothing
  }
}

int main(int argc, char** argv)
{
  // Prepare frame transformers
  IdentityTransformation nothing;
  FilterTransformation grayDownsampling = FilterTransformation(
    FilterType::Nothing, 0, 0, 0.43
    );
  
  auto tracker = createTracker(trackerModelName);

  // Start capturing from video source
  VideoCamera cam("cam");
  cam.captureRealtimeWith(grayDownsampling, tracker);
  return 0;
}