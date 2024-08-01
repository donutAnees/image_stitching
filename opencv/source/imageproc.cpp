#include "../header/imageproc.hpp"

cv::Point shift(cv::Mat &img, cv::Rect rect)
{
  int top = 0, bottom = 0, left = 0, right = 0;
  cv::Point tl = rect.tl();
  cv::Point br = rect.br();
  if (tl.y < 0)
    top = -tl.y;
  if (br.y > img.rows)
    bottom = br.y - img.rows;
  if (tl.x < 0)
    left = -tl.x;
  if (br.x > img.cols)
    right = br.x - img.cols;
  cv::copyMakeBorder(img, img, top, bottom, left, right, cv::BORDER_CONSTANT);
  return cv::Point(left, top);
}