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

      totalGravX += this->gravity(distance, vx);
      totalGravY += this->gravity(distance, vy);
    }
    
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
    auto vx = this->velocityX.at<double>(a.y, a.x);
    auto vy = this->velocityY.at<double>(a.y, a.x);
    auto a_ = Point2i(a.x + vx, a.y + vy);
    arrowedLine(this->canvas, a, a_, Scalar(0,0,245), 1.0, CV_AA);
  }

  // Draw the given points with velocity
  for (auto tup : ps)
  {
    auto p = get<0>(tup);
    auto v = get<1>(tup);
    auto p_ = Point2i(p.x + v.x, p.y + v.y);
    arrowedLine(this->canvas, p, p_, Scalar(200,0,0), 1.0, CV_AA);
  }

  imshow(wndName, this->canvas);
}