#include "MeshObject.h"

MeshObject::MeshObject(vector<Point> &ps)
{
  this->points = ps;
}

vector<TrackableKeyPoint> MeshObject::toKeypoints() const
{
  vector<TrackableKeyPoint> kps;
  for (Point p: this->points)
    kps.push_back(TrackableKeyPoint(p));
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
  return this->points.empty();
}


void MeshObject::drawMesh(Mat &canvas, Scalar color) const
{
  auto N = this->points.size();
  for (int i=0; i<N; i++)
    for (int j=i+1; j<N; j++)
    {
      Point p0 = this->points[i];
      Point p  = this->points[j];
      if (!EQ(p0,p))
        line(canvas, p0, p, color, 1, CV_AA);
    }
}



