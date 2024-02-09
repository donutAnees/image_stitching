#include "../header/stitch.hpp"
#include <string>
#include <opencv2/core/mat.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>

void stitch::processImage()
{
    {
        uint8_t imageCount = images.size();
        uint8_t middle = images.size() / 2;
        if (imageCount % 2 == 0)
        {
            cv::Mat descriptors1, descriptors2;
            std::vector<cv::KeyPoint> keypoints1, keypoints2;
            cv::Ptr<cv::Feature2D> orb = cv::ORB::create(10000);
            std::vector<bool> states1;
            std::vector<bool> states2;
            orb->detectAndCompute(images[middle], states1, keypoints1, descriptors1);
            orb->detectAndCompute(images[middle + 1], states2, keypoints2, descriptors2);
            std::vector<std::vector<cv::DMatch>> matches;
            cv::BFMatcher matcher = cv::BFMatcher(cv::NORM_HAMMING); // Hamming is good for binary string based algorithms , cross checker can be true if ratio is not used
            matcher.knnMatch(descriptors1, descriptors2, matches, 2);
            std::vector<cv::DMatch> goodmatches;
            for (int i = 0; i < matches.size(); i++)
            {
                if (matches[i][0].distance < 0.75 * matches[i][1].distance)
                {
                    goodmatches.emplace_back(matches[i][0]);
                }
            }
            cv::Mat imMatches;
            cv::drawMatches(images[middle], keypoints1, images[middle+1], keypoints2, goodmatches, imMatches);
    

        }
        else
        {
        }
    }
}
void stitch::addImage(const std::string &filename)
{
    cv::Mat image = cv::imread(filename);
    images.emplace_back(image);
}