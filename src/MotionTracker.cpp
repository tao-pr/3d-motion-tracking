#include "MotionTracker.h"

MotionTracker::MotionTracker()
{
}

function<void (Mat)> MotionTracker::track()
{
  auto pipe = [&](Mat im)
  {
    trackMotion(im);
  };

  return pipe;
}

void MotionTracker::trackMotion(Mat &im)
{
  // Convert image to HSV
  Mat hsv;
  cvtColor(im, hsv, CV_BGR2HSV);

  // Blur the image for noise reduction
  Mat bhsv;
  GaussianBlur( hsv, bhsv, Size(3,3), 2.5, 2.5, BORDER_REFLECT_101);

  // Take only V component
  Mat arr[3];
  split(bhsv, arr);
  Mat h = arr[0];
  Mat s = arr[0];
  Mat v = arr[2];

  normalize(h, h, 0, 255, NORM_MINMAX, -1, Mat());
  medianBlur(h, h, 9);

  // Detect corners
  int wndSize    = 5;
  int maxCorners = 16;
  double minDist = 5;
  vector<Point2f> cornersS = TrackUtils::detectFeaturePoints(h, wndSize, maxCorners, minDist*3, 0.2);
  vector<Point2f> cornersV = TrackUtils::detectFeaturePoints(v, wndSize, maxCorners, minDist, 0.05);

  // Canvas
  // Mat canvas = Mat::zeros(v.size(), CV_8UC3);
  // cvtColor(h, canvas, COLOR_GRAY2RGB);
  Mat canvas = im.clone();

  // Create a trackable mesh
  vector<Point2f> points = cornersS;
  points.insert(points.end(), cornersV.begin(), cornersV.end());
  cout << "... " << points.size() << " vertices captured" << endl;
  MeshObject mesh(points);
  cout << "... Mesh allocated" << endl;

  double maxEdgeLength = im.size[0];
  mesh.drawMesh(canvas, Scalar(100,100,200), maxEdgeLength);

  // Draw detected corners
  DrawUtils::drawMarks(canvas, cornersS, Scalar(0,0,240));
  DrawUtils::drawMarks(canvas, cornersV, Scalar(210,100,0));

  namedWindow("tracked", CV_WINDOW_AUTOSIZE);
  imshow("tracked", canvas);
}


Mat MotionTracker::calcHistBackProjection(Mat& im)
{
  Mat hist, bproj;
  Mat mask = Mat();
  int nChannels[] = {0};
  int histBins[]  = {16};
  float rangeValues[] = {0, 180};
  const float* ranges[] = {rangeValues};
  calcHist(&im, 1, nChannels, mask, hist, 1, histBins, ranges, true, false);
  normalize(hist, hist, 0, 255, NORM_MINMAX, -1, mask);
  GaussianBlur( hist, hist, Size(3,3), 10.0, 10.0, BORDER_REFLECT_101);
  calcBackProject(&im, 1, nChannels, hist, bproj, ranges);
  return bproj;
}
