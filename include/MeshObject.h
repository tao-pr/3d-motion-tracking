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
#include "KeyPoint.h"
#include "CalcUtils.h"

struct Point2fPair
{
  Point2f a, b;
  double distance;
};

struct ShorterDistance
{
  bool operator() (const Point2fPair& p1, const Point2fPair& p2) const 
  {
    return p1.distance < p2.distance;
  }
};

class MeshObject
{
public:
  MeshObject(){};
  MeshObject(vector<Point2f> &ps);

  vector<TrackedPoint> toKeyPoint2fs() const;

  // Align new mesh onto the current mesh
  // This will also smoothen the displacement of Point2fs
  // with tracking technique
  MeshObject align(MeshObject &newMesh);

  Point2f centroid() const; 

  // Split the mesh into one or more sub-meshes
  vector<MeshObject> split(double maxEdgeLength=100) const;

  bool isEmpty() const;

  void drawMesh(Mat &canvas, Scalar color, double maxDistance=100) const;

protected:
  vector<Point2f> points; // TAOTOREVIEW: KD-Tree might be a better choice
};




#endif 