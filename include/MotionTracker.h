#ifndef MOTIONTRACK
#define MOTIONTRACK

#include <vector>
#include <math.h>
#include <functional>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cvaux.hpp>
#include "DrawUtils.h"
#include "CalcUtils.h"
#include "TrackUtils.h"
#include "ITracker.h"
#include "MeshObject.h"
#include "Hungarian.h"

using namespace cv;
using namespace std;


class MotionTracker : ITracker
{
public:
  MotionTracker(float maxMeshDistance, float maxMeshEdgeLength, int longestAbsence, bool debug=false);
  function<void (Mat)> track();

  float maxMeshEdgeLength; // Maximum edge length of a mesh (longer edge will be cut off as another mesh)
  float maxMeshDistance; // Maximum distance allowed for coupling two meshes
  int longestAbsence; // Maximum number of frames allowed for a mesh to be absent

protected:
  bool debug;
  void trackMotion(Mat& im);
  void alignMeshes(vector<MeshObject> newMeshes, double maxDist);
  Mat calcHistBackProjection(Mat& im);

private:
  vector<MeshObject> currMeshes;
};

#endif