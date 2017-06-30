#include "Grid.h"

vector<tuple<Point2i, Point2f>> Grid::calculateVelocity(const vector<Point2i>& ps) const
{
  vector<tuple<Point2i, Point2f>> psOutput;
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

void Grid::renderVelocityMap(const string& wndName)
{
  this->canvas.setTo(Scalar::all(255));
  this->canvas64.setTo(Scalar::all(0.0));
  
  const int W = this->velocityX.cols;
  const int H = this->velocityX.rows;
  
  for (int i=0; i<W; i++)
    for (int j=0; j<H; j++)
    {
      auto magX = this->velocityX.at<double>(j,i);
      auto magY = this->velocityY.at<double>(j,i);
      if (magX + magY < this->gravityThreshold)
        continue;
      auto mag  = sqrt(magX*magX + magY*magY);
      auto c = Point2f(i+PATCH_MAP_SIZE/2, j+PATCH_MAP_SIZE/2);

      this->canvas64.at<double>(j,i) = mag;
    }

  normalize(this->canvas64, this->canvas64, 0.0, 255.0, NORM_MINMAX);

  for (int i=0; i<W; i++)
    for (int j=0; j<H; j++)
    {
      auto p0     = Point2i(i, j);
      auto p1     = Point2i(i+PATCH_MAP_SIZE, j+PATCH_MAP_SIZE);
      auto colour = (int)floor(this->canvas64.at<double>(j,i));
      rectangle(this->canvas, p0, p1, colour, -1);
    }

  // TAOTODO: Draw arrows

  imshow(wndName, this->canvas);
}