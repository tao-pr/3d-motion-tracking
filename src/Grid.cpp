#include "Grid.h"

vector<tuple<Point2i, Point2d>> Grid::calculateVelocity(const vector<Point2i>& ps) const
{
  vector<tuple<Point2i, Point2d>> psOutput;
  for (auto p : ps)
  {
    double totalGravX = 0.0;
    double totalGravY = 0.0;
    for (auto c : this->anchors)
    {
      auto distance = _sqrt(_square(c.x-p.x) + _square(c.y-p.y));
      if (distance > this->maxDistance)
        continue;

      auto vx = this->velocityX.at<double>(c.y, c.x);
      auto vy = this->velocityY.at<double>(c.y, c.x);

      if (_sqrt(_square(vx) + _square(vy)) < gravityThreshold)
        continue;

      totalGravX += vx;
      totalGravY += vy;
    }

    // TAODEBUG:
    cout << "tot gx : " << totalGravX << endl;
    cout << "tot gy : " << totalGravY << endl;

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