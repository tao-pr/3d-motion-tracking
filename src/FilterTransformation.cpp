#include "FilterTransformation.h"

FilterTransformation::FilterTransformation(FilterType t, float level, float maxScale)
{
  this->m_type     = t;
  this->m_level    = level;
  this->m_maxScale = maxScale;
}

Mat FilterTransformation::transform(Mat& input)
{
  switch (m_type)
  {
    case FilterType::Gray: 
      return toGray(input);
    case FilterType::BinaryThreshold: 
      return toBinarised(input);
    case FilterType::TruncateThreshold:
      return toTruncated(input);
    case FilterType::ZeroThreshold:
      return toZeroCut(input);
    default:
      return input; // By default, analogous to IdentityTransformation
  }
}

Mat FilterTransformation::toGray(Mat &input)
{
  Mat buffer;
  cvtColor(input, buffer, CV_BGR2GRAY);
  return buffer;
}

Mat FilterTransformation::toBinarised(Mat &input)
{
  Mat buffer;
  threshold(input, buffer, m_level, m_maxScale, THRESH_BINARY);
  return buffer;
}

Mat FilterTransformation::toTruncated(Mat &input)
{
  Mat buffer;
  threshold(input, buffer, m_level, m_maxScale, THRESH_TRUNC);
  return buffer;
}

Mat FilterTransformation::toZeroCut(Mat &input)
{
  Mat buffer;
  threshold(input, buffer, m_level, m_maxScale, THRESH_TOZERO);
  return buffer;
}