#ifndef HUNGARIAN
#define HUNGARIAN

#include <vector>
#include <tuple>
#include <unordered_map>
#include <opencv2/opencv.hpp>
#include <opencv/cvaux.hpp>

using namespace std;
using namespace cv;

/**
 * Hungarian algorithm implementation 
 * Compatible with [Mat<double>]
 */
class Hungarian
{
public:
  Hungarian(const Mat& cost);

  // <row, col>
  vector<tuple<int, int>> optimiseMinima() const;

private:
  Mat costM;

  // Find minimum of the i-th row
  double minOfRow(int i) const;
  // Find minimum of the i-th col
  double minOfCol(int i) const;

  static Mat coverZeros(Mat& m);
};


#endif