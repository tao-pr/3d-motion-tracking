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

  double maxEdgeLength   = im.size[0];
  double maxDisplacement = im.size[0]*0.833;

  // Split meshes
  vector<MeshObject> meshes = mesh.split(maxEdgeLength);
  cout << "... " << meshes.size() << " meshes splitted" << endl;
  for (MeshObject m : meshes) 
    m.drawMesh(canvas, Scalar(100,100,200), Scalar(0,0,240), maxEdgeLength);

  // TAOTODO: Align the newly detected meshes with the previous ones
  alignMeshes(meshes, maxDisplacement);

  namedWindow("tracked", CV_WINDOW_AUTOSIZE);
  imshow("tracked", canvas);
}

void MotionTracker::alignMeshes(vector<MeshObject> newMeshes, double maxDist)
{
  // Memorise the previously detected meshes
  vector<MeshObject> prevMeshes;
  while (!this->currMeshes.empty())
  {
    MeshObject m = this->currMeshes.back();
    prevMeshes.push_back(m);
    this->currMeshes.pop_back();
  }

  int N0 = prevMeshes.size();
  int N1 = this->currMeshes.size();
  int N  = max(N0,N1);

  // Take only centroids into account
  vector<Point2f> centroids0;
  vector<Point2f> centroids;

  // Fill lists of centroids
  for (auto m : prevMeshes)
    centroids0.push_back(m.centroid());
  for (auto m : this->currMeshes)
    centroids.push_back(m.centroid());

  // TAOTODO: Pad if N0 != N1

  // Map 1-to-N distrances
  // and create an assignment problem matrix M
  //     [row] => represents old mesh
  //     [col] => represents new mesh
  Mat m = Mat(N, N, CV_32F, Scalar(0.));
  int j = 0;
  for (auto c0 : centroids0)
  {
    int i = 0;
    for (auto c1 : centroids)
    {
      float d = (float)_dist(c0,c1);
      m.at<float>(j,i) = d;
      i++;
    }
    j++;
  }

  // TAOTODO: Use Hungarian algorithm to find best matches
  // of [currentMesh] and [prevMesh]



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
