#include "CamShiftTracker.h"

function<void (Mat)> CamShiftTracker::track()
{
  auto pipe = [&](Mat im)
  {
    // TAOTODO: Track!
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

  // Compute image histogram
  cout << "...Computing histogram" << endl;
  Mat hist, backproj;
  Mat mask              = Mat(); // No masking
  int histBins[]        = {64, 64}; // bin sizes
  float range_hue[]     = {0, 180};
  float range_sat[]     = {0, 256};
  const float* ranges[] = {range_hue, range_sat};
  int nChannels[]       = {0, 1}; // 0th and 1st channels

  calcHist( &hsv, 1, nChannels, mask, hist, 2, histBins, ranges, true, false );
  normalize(hist, hist, 0, 255, NORM_MINMAX);
  
  cout << "...Computing back projection" << endl;
  calcBackProject(&hsv, 1, nChannels, hist, backproj, ranges);

  // Track camera shift
  cout << "...Computing camera shift" << endl;
  RotatedRect tracked  = CamShift(backproj, entireRect, crit);

  // Draw the tracked area
  auto canvas = im.clone();
  ellipse( canvas, tracked, Scalar(0,100,255), 3, LINE_AA );
  namedWindow("tracked", CV_WINDOW_AUTOSIZE);
  imshow("tracked", canvas); 
}

