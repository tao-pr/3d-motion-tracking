/**
 * Entry point for 3D motion track app
 * @author TaoPR (StarColon.com)
 * @copyRight 2016-now 
 */

#include <functional>
#include "IdentityTransformation.h"
#include "FilterTransformation.h"
#include "VideoCamera.h"
#include "ParticleTracker.h"
#include "ITracker.h"

using namespace std;

bool debug              = true;
ITracker* trackerModel  = nullptr;

void mouseEvent(int event, int x, int y, int n, void* p)
{
  if (event == EVENT_LBUTTONDOWN)
  {
    // Point selection
    // TAOTODO:
    cout << "(" << x << ", " << y << ")" << endl;
  }
}

function<void (Mat)> createTracker()
{
  trackerModel = new ParticleTracker("sift");
  return trackerModel->track();
}

int main(int argc, char** argv)
{
  // Prepare frame transformers
  IdentityTransformation nothing;
  FilterTransformation grayDownsampling = FilterTransformation(
    FilterType::Nothing, 0, 0, 0.43);
  
  auto tracker = createTracker();
  ITracker::bindMouseEvent(trackerModel->wndName, mouseEvent);

  // Start capturing from video source
  VideoCamera cam("cam");
  cam.captureRealtimeWith(grayDownsampling, tracker);
  return 0;
}