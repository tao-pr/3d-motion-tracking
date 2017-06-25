#ifndef TRANSFORMATION
#define TRANSFORMATION

#include <vector>
#include <functional>
#include <opencv2/opencv.hpp>
#include "master.h"

using namespace std;
using namespace cv;

/**
 * Generic shape deformation of generic subject
 */
template<class TSubject> 
class Deformation
{
protected:
  double[] c; // Coefficients of the transformation
public:
  Deformation(double[] coeff);
  virtual TSubject apply(const TSubject& s) = 0;
};


#endif