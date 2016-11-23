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

#define EQ(a,b) ((a.x==b.x && a.y==b.y))

struct PointPair
{
  Point a, b;
  double distance;
};

struct ShorterDistance
{
  bool operator() (const PointPair& p1, const PointPair& p2) const 
  {
    return p1.distance < p2.distance;
  }
};

class MeshObject
{
public:
  MeshObject(){};
  MeshObject(vector<Point> &ps);

  vector<TrackableKeyPoint> toKeypoints() const;

  // Align new mesh onto the current mesh
  // This will also smoothen the displacement of points
  // with tracking technique
  MeshObject align(MeshObject &newMesh);

  bool isEmpty() const;

  void drawMesh(Mat &canvas, Scalar color) const;

protected:
  vector<Point> points; // TAOTOREVIEW: KD-Tree might be a better choice
};




#endif 