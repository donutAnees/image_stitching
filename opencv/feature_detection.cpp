#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>


const float GOOD_MATCH_PERCENT = 0.15f;

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    string file1(argv[1]);
    string file2(argv[2]);
    Mat imRef1 = imread(file1, IMREAD_GRAYSCALE);
    Mat imRef2 = imread(file2, IMREAD_GRAYSCALE);
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    Ptr<Feature2D> orb = ORB::create();
    orb->detectAndCompute(imRef1, Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(imRef2, Mat(), keypoints2, descriptors2);
    vector<DMatch> matches;
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
    std::sort(matches.begin(), matches.end());
    matcher->match(descriptors1, descriptors2, matches, Mat());
    const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
    matches.erase(matches.begin()+numGoodMatches, matches.end());
    Mat imMatches;
    drawMatches(imRef1, keypoints1, imRef2, keypoints2, matches, imMatches);
    imshow("Matches", imMatches);
    waitKey(0);
    return 0;
}