#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    string file1(argv[1]);
    string file2(argv[2]);
    Mat imRef1 = imread(file1, IMREAD_COLOR);
    Mat imRef2 = imread(file2, IMREAD_COLOR);

    Size size(imRef1.rows / 2 , imRef1.cols / 2);
    resize(imRef1,imRef1,size);
    resize(imRef2,imRef2,size);

    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    Ptr<Feature2D> orb = ORB::create(10000);

    orb->detectAndCompute(imRef1, Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(imRef2, Mat(), keypoints2, descriptors2);

    vector<vector<DMatch>> matches;
    BFMatcher matcher = BFMatcher(NORM_HAMMING); // Hamming is good for binary string based algorithms , cross checker can be true if ratio is not used

    matcher.knnMatch(descriptors1, descriptors2, matches, 2);

    vector<DMatch> goodmatches;

    for (int i = 0; i < matches.size(); i++)
    {
        if (matches[i][0].distance < 0.75 * matches[i][1].distance)
        {
            goodmatches.emplace_back(matches[i][0]);
        }
    }

    Mat imMatches;
    drawMatches(imRef1, keypoints1, imRef2, keypoints2, goodmatches, imMatches);

    std::vector<Point2f> points1, points2;
    for (size_t i = 0; i < goodmatches.size(); i++)
    {
        {
            points1.push_back(keypoints1[goodmatches[i].queryIdx].pt);
            points2.push_back(keypoints2[goodmatches[i].trainIdx].pt);
        }
    }

    Mat H = findHomography(points2, points1, RANSAC, 4);

    imshow("result", imMatches);
    waitKey(0);

    Mat result;

    warpPerspective(imRef2, result, H, Size(imRef1.cols + imRef2.cols, max(imRef1.rows, imRef2.rows)));
    imshow("result", result);
    waitKey(0);
    Mat half(result, Rect(0, 0, imRef1.cols, imRef1.rows));
    imRef1.copyTo(half);
    imwrite("matches.jpg", imMatches);
    imshow("result", result);

    waitKey(0);
    return 0;
}