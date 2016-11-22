#ifndef MESH_OBJECT
#define MESH_OBJECT

#include <queue>
#include <vector>
#include <math.h>
#include <functional>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv/cvaux.hpp>
#include "DrawUtils.h"
#include "Keypoint.h"

class MeshObject
{
public:
  MeshObject(){};
  MeshObject(vector<Point> &ps);

  // Align two meshes and track corresponding points
  MeshObject align(MeshObject &another);

protected:
  vector<Point> points; // TAOTOREVIEW: KD-Tree might be a better choice
};




#endif 