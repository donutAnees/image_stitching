#include <iostream>
#include <fstream>
#include <vector>
#include "../header/Mat.hpp"
#include "../header/Stitch.hpp"
#include "../header/Orb.hpp"
#include "../header/Feature.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    Mat mat = Mat("../img/img1.ppm");
    Mat gray = convertToGray(mat);
    // Write the grayscale image to a new PPM file
    //Stitcher stitch = Stitcher();
    Mat descriptor;
    std::vector<Keypoint> keypoints;
    ORB orb = ORB(gray,keypoints,descriptor);
    std::vector<std::pair<int,int>> commonKeypoint = orb.detectAndCompute();
    //stitch.drawKeypoints(gray,orb.keypoints);
    //Mat resized = resize(0.125,gray)s;
    //writePPM(resized,"resized.ppm");
    return 0;
}