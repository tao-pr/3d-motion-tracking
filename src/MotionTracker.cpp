#include "MotionTracker.h"

MotionTracker::MotionTracker()
{
}

function<void (Mat)> MotionTracker::track()
{
  auto pipe = [&](Mat im)
  {
    trackMotion(im);
    cout << "Tracking ..." << endl;
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

  // Detect corners
  int wndSize    = 5;
  int maxCorners = 32;
  double minDist = 5;
  vector<Point2f> cornersH = TrackUtils::detectFeaturePoints(h, wndSize, maxCorners, minDist*3);
  vector<Point2f> cornersV = TrackUtils::detectFeaturePoints(v, wndSize, maxCorners, minDist);

  // Canvas
  // Mat canvas = Mat::zeros(v.size(), CV_8UC3);
  // cvtColor(s, canvas, COLOR_GRAY2RGB);
  Mat canvas = im.clone();

  // Draw detected corners
  DrawUtils::drawMarks(canvas, cornersH, Scalar(0,0,240));
  DrawUtils::drawMarks(canvas, cornersV, Scalar(0,210,240));

  namedWindow("tracked", CV_WINDOW_AUTOSIZE);
  imshow("tracked", canvas);
}

