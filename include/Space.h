#ifndef SPACE
#define SPACE

#include <vector>
#include <opencv2/opencv.hpp>
#include "master.h"
#include "Transformation.h"

using namespace std;
using namespace cv;

/**
 * Space structure which builds up a network of 
 * vertices and their connected neighbours 
 */
template <class TStructure>
class Space
{
private:
  vector<Point2f> vertices;
protected:
  TStructure structure;
public:
  inline Space(const vector<Point2f> vs)
  {
    this->vertices.assign(vs.begin(), vs.end());
  }

  virtual ~Space();
  static Space& create(const vector<Point2f> ps);

  inline const TStructure& getStructure()
  {
    return this->structure;
  };

  virtual Deformation<Space<TStructure>> estimateDeformationTo(const Space<TStructure> &another) = 0;
};


#endif