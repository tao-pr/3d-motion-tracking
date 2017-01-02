#include "CamShiftTracker.h"

function<void (Mat)> CamShiftTracker::track()
{
  auto pipe = [&](Mat im)
  {
    trackMotion(im);
    cout << "Tracking ..." << endl;
  };

  return pipe;
}

void CamShiftTracker::trackMotion(Mat &im)
{
  Rect entireRect   = Rect(0, 0, im.cols, im.rows);
  TermCriteria crit = TermCriteria(TermCriteria::MAX_ITER, 10, 1.0);

  // Convert image to HSV
  Mat hsv;
  cvtColor(im, hsv, CV_BGR2HSV);

  // Blur the image for noise reduction
  Mat bhsv;
  GaussianBlur( hsv, bhsv, Size(9,9), 8.0, 8.0, BORDER_REFLECT_101);

  // Compute image histogram
  cout << "...Computing histogram" << endl;
  Mat hist, backproj;
  Mat mask              = Mat(); // No masking
  int histBins[]        = {64, 64}; // bin sizes
  float range_hue[]     = {0, 180};
  float range_sat[]     = {0, 256};
  const float* ranges[] = {range_hue, range_sat};
  int nChannels[]       = {0, 2}; // Index of channels to take

  calcHist( &hsv, 1, nChannels, mask, hist, 2, histBins, ranges, true, false );
  normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());
  
  cout << "...Computing back projection" << endl;
  calcBackProject(&hsv, 1, nChannels, hist, backproj, ranges);

  // Track camera shift
  cout << "...Computing camera shift" << endl;
  RotatedRect tracked  = CamShift(backproj, entireRect, crit);

  // Draw the tracked area
  auto canvas = im.clone();
  ellipse( canvas, tracked, Scalar(0,100,255), 3, LINE_AA );

  // Draw the tracked movement direction
  arrowedLine(
    canvas, 
    tracked.center, 
    Point(
      tracked.center.x + tracked.size.width*cos(tracked.angle)/5,
      tracked.center.y - tracked.size.height*sin(tracked.angle)/5),
    Scalar(0,100,255),
    2, 8, 0, 0.08
    );


  namedWindow("tracked", CV_WINDOW_AUTOSIZE);
  imshow("tracked", canvas); 
}

