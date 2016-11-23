#include "MeshObject.h"

MeshObject::MeshObject(vector<Point2f> &ps)
{
  this->Point2fs = ps;
}

vector<TrackedPoint> MeshObject::toKeyPoint2fs() const
{
  vector<TrackedPoint> kps;
  for (Point2f p: this->Point2fs)
    kps.push_back(TrackedPoint(p));
  return kps;
}

MeshObject MeshObject::align(MeshObject &newMesh)
{
  // TAOTODO:
  MeshObject out;

  
  return out;
}

bool MeshObject::isEmpty() const
{
  return this->Point2fs.empty();
}


void MeshObject::drawMesh(Mat &canvas, Scalar color, double maxDistance) const
{
  auto N = this->Point2fs.size();
  for (int i=0; i<N; i++)
    for (int j=i+1; j<N; j++)
    {
      Point2f p0 = this->Point2fs[i];
      Point2f p  = this->Point2fs[j];
      if (_dist(p0,p) <= maxDistance)
        line(canvas, p0, p, color, 1, CV_AA);
    }
}



