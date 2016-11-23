#include "MeshObject.h"

MeshObject::MeshObject(vector<Point2f> &ps)
{
  this->points = ps;
}

vector<TrackedPoint> MeshObject::toKeyPoint2fs() const
{
  vector<TrackedPoint> kps;
  for (Point2f p: this->points)
    kps.push_back(TrackedPoint(p));
  return kps;
}

MeshObject MeshObject::align(MeshObject &newMesh)
{
  // TAOTODO:
  MeshObject out;

  
  return out;
}

Point2f MeshObject::centroid() const
{
  if (this->isEmpty())
    throw "No elements";
  double sumx = 0;
  double sumy = 0;
  double N    = (double)this->points.size();

  for (Point2f p : this->points)
  {
    sumx += p.x;
    sumy += p.y;
  }

  return Point2f(sumx/N, sumy/N);
}

vector<MeshObject> MeshObject::split(double maxEdgeLength) const
{
  vector<MeshObject> ms;

  // TAOTODO:

  return ms;
}

bool MeshObject::isEmpty() const
{
  return this->points.empty();
}


void MeshObject::drawMesh(Mat &canvas, Scalar color, double maxDistance) const
{
  auto N = this->points.size();
  for (int i=0; i<N; i++)
    for (int j=i+1; j<N; j++)
    {
      Point2f p0 = this->points[i];
      Point2f p  = this->points[j];
      if (_dist(p0,p) <= maxDistance)
        line(canvas, p0, p, color, 1, CV_AA);
    }
}



