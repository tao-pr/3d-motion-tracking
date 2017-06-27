#ifndef SPACE
#define SPACE

#include <vector>
#include <opencv2/opencv.hpp>
#include "master.h"

using namespace std;
using namespace cv;

struct AnchorWithVelocity
{
  Point2f origin;
  Point2f speed;

  void setSpeed(double x, double y)
  {
    this->speed.x = x;
    this->speed.y = y;
  }

  static AnchorWithVelocity create(const Point2f& o)
  {
    AnchorWithVelocity a;
    a.origin = o;
    return a;
  }
};

/**
 * Space structure which builds up a network of 
 * vertices and their connected neighbours 
 */
template <class TStructure>
class Space
{
private:
  vector<AnchorWithVelocity> anchors;
protected:
  TStructure structure;
public:
  // Create a [Space] from list of anchor points
  inline Space(const vector<AnchorWithVelocity> vs)
  {
    this->anchors.assign(vs.begin(), vs.end());
  }

  virtual ~Space();

  inline const vector<AnchorWithVelocity> getAnchors() const 
  {
    return this->anchors;
  }

  inline const vector<Point2f> getAnchorsWithoutVelocity() const
  {
    vector<Point2f> ps;
    for (auto a : this->anchors)
    {
      ps.push_back(a.origin);
    }
    return ps;
  }

  inline const TStructure& getStructure() const
  {
    return this->structure;
  }
};


#endif