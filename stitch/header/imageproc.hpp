#ifndef IMAGEPROC_HPP
#define IMAGEPROC_HPP

#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

cv::Point shift(cv::Mat &img, cv::Rect rect);

#endif