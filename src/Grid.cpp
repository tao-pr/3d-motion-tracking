#include "Grid.h"

vector<tuple<Point2i, Point2d>> Grid::calculateVelocity(const vector<Point2i>& ps) const
{
  vector<tuple<Point2i, Point2d>> psOutput;
  for (auto p : ps)
  {
    // Identify N closest neighbours
    priority_queue<DistanceToPoint, vector<DistanceToPoint>, CompareNeighbourDistance> closest;
    for (auto c : this->anchors)
    {
      auto distance = _sqrt(_square(c.x-p.x) + _square(c.y-p.y));
      if (distance > this->maxDistance)
        continue;
      closest.push(make_tuple(distance, p));
    }

    // Take only top N nearest neighbours
    int n = 0;
    double factorRatio = 1.0;
    double totalGravX = 0.0;
    double totalGravY = 0.0;
    double sumFactor = 1.0;
    while (!closest.empty() && n<numNeighbours)
    {
      auto next   = closest.top();
      auto anchor = get<1>(next);
      closest.pop();

      // TAOTOREVIEW: Consider influence from distance to the anchor
      
      totalGravX += factorRatio * this->velocityX.at<double>(anchor.y, anchor.x);
      totalGravY += factorRatio * this->velocityY.at<double>(anchor.y, anchor.x);

      n++;
      factorRatio *= 0.9;
      sumFactor += factorRatio;
    }

    totalGravX /= sumFactor;
    totalGravY /= sumFactor;

    psOutput.push_back(make_tuple(p, Point2f(totalGravX, totalGravY)));
  }
  return psOutput;
}

void Grid::renderVelocityMap(const string& wndName, const vector<tuple<Point2i, Point2d>>& ps)
{
  this->canvas.setTo(Scalar::all(255));
  
  const int W = this->velocityX.cols;
  const int H = this->velocityX.rows;

  // Draw anchors first
  for (auto a : this->anchors)
  {
    auto vx = this->velocityX.at<double>(a.y, a.x) * PATCH_MAP_SIZE;
    auto vy = this->velocityY.at<double>(a.y, a.x) * PATCH_MAP_SIZE;
    auto aScaled = Point2i(a.x*PATCH_MAP_SIZE, a.y*PATCH_MAP_SIZE);
    auto bScaled = Point2i((a.x + vx)*PATCH_MAP_SIZE, 
                           (a.y + vy)*PATCH_MAP_SIZE);
    if (_sqrt(_square(vx) + _square(vy)) > MIN_VELOCITY_TO_DRAW)
      arrowedLine(this->canvas, aScaled, bScaled, Scalar(0,0,245), 1.0, CV_AA, 0, 0.3);

    rectangle(this->canvas, 
              Point2i((a.x-2)*PATCH_MAP_SIZE, (a.y-2)*PATCH_MAP_SIZE), 
              Point2i((a.x+2)*PATCH_MAP_SIZE, (a.y+2)*PATCH_MAP_SIZE),
              Scalar(0,0,245),
              1.0, CV_AA);
  }

  // Draw the given points with velocity
  for (auto tup : ps)
  {
    auto p = get<0>(tup);
    auto v = get<1>(tup);
    auto pScaled = Point2i(p.x*PATCH_MAP_SIZE, p.y*PATCH_MAP_SIZE);
    auto qScaled = Point2i((p.x + v.x)*PATCH_MAP_SIZE, 
                           (p.y + v.y)*PATCH_MAP_SIZE);
    arrowedLine(this->canvas, pScaled, qScaled, Scalar(200,0,0), 1.0, CV_AA);
    rectangle(this->canvas, 
              Point2i((p.x-2)*PATCH_MAP_SIZE, (p.y-2)*PATCH_MAP_SIZE), 
              Point2i((p.x+2)*PATCH_MAP_SIZE, (p.y+2)*PATCH_MAP_SIZE),
              Scalar(0,245,0),
              2.0, CV_AA);
  }

  imshow(wndName, this->canvas);
}