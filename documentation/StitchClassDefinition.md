# Class Definition

This is the main class of this project, which has the following attributes and functions

## Attributes

1. **images**
   - It is a vector of images which are to be stitched together.
   - Type: `std::vector<cv::Mat> &images`

2. **imagesDescriptors**
   - It is a vector of descriptors corresponding to each image.
   - Type: `std::vector<cv::Mat> &imagesDescriptors`

3. **imagesKeypoints**
   - It is a vector of keypoints corresponding to each image.
   - Type: `std::vector<std::vector<cv::KeyPoint>> &imagesKeypoints`

4. **grayscaledImages**
   - It is a vector of grayscale versions of input images.
   - Type: `std::vector<cv::Mat> &grayscaledImages`

5. **currentStitchedImage**
   - It is the currently stitched image (result of stitching process).
   - Type: `cv::Mat &currentStitchedImage`

6. **currentImageDescriptor**
   - It is the descriptor of the currently processed image.
   - Type: `cv::Mat &currentImageDescriptor`

7. **currentImageKeypoints**
   - It is a vector of keypoints detected in the currently processed image.
   - Type: `std::vector<cv::KeyPoint> &currentImageKeypoints`

8. **orb**
   - It is the feature detector and descriptor extractor.
   - Type: `cv::Ptr<cv::Feature2D> &orb`

## Functions

1. **processImage()**
   - This function processes the images by stitching them together into a panorama.
     - It calculates the number of images and determines the middle index.
     - If the number of images are odd then it will not merge the middle pics, else it will merge the middle pics, this is to make sure that the number of pics at both side from the middle is same.
     - It merges the images from the middle iteratively with images from left and right, this is done until there are no images on both side.
     - Finally, it crops the stitched image to remove any black regions and writes the result to a file.

2. **addImage(const std::string &filename, bool flag)**
   - This function adds a new image to the list of images to be stitched.
     - It reads the image from the specified file path.
     - If the `flag` parameter is `true`, it resizes the image to one-fourth of its original size.
     - It converts the image to grayscale, detects keypoints and descriptors, and stores them along with the image in their respective containers.

3. **setCurrentImage(cv::Mat &image)**
   - This function updates the currently stitched image to the provided image.
     - It converts the image to grayscale, detects keypoints and descriptors, and updates the member variables accordingly.

4. **showMatches(std::vector<cv::KeyPoint> &keypoints1, std::vector<cv::KeyPoint> &keypoints2, std::vector<cv::DMatch> &goodmatches, cv::Mat &image1, cv::Mat &image2)**
   - This function displays the matches between two images.
     - It draws the matches between the keypoints of the two input images and displays the result.

5. **mergeMiddleImages(cv::Mat &result, std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, uint8_t middle)**
   - This function merges the middle images in the sequence.
     - It computes the homography between the keypoints of the middle images.
     - It warps the second middle image onto the first one and blends them together to create the stitched image.

6. **mergeMidRightImages(std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, int rightIndex)**
   - This function merges the right-side images with the currently stitched image.
     - It computes the homography between the keypoints of the current stitched image and the right-side image.
     - It warps the right-side image onto the current stitched image and blends them together to update the stitched image.

7. **mergeLeftMidImages(std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, int leftIndex)**
   - This function merges the left-side images with the currently stitched image.
     - It computes the homography between the keypoints of the left-side image and the current stitched image.
     - It warps the left-side image onto the current stitched image and blends them together to update the stitched image.

8. **mergeLeftMidRightImages(int leftIndex, int rightIndex)**
   - This function merges the left, middle, and right images together.
     - It calls `mergeMidRightImages` and `mergeLeftMidImages` to merge the left and right images with the current stitched image.

9. **getMatchingPoint(std::vector<cv::Point2f> &points1, std::vector<cv::Point2f> &points2, cv::Mat &descriptors1, cv::Mat &descriptors2, std::vector<cv::KeyPoint> &keypoints1, std::vector<cv::KeyPoint> &keypoints2, cv::Mat &image1, cv::Mat &image2)**
   - This function finds matching keypoints between two images.
     - It computes descriptors for the keypoints of both images and matches them using a brute-force matcher.
     - It selects only the good matches based on the distance ratio criterion and stores the matching keypoints.

10. **findWrapRect(cv::Size sz, cv::Mat &H)**
    - This function finds the bounding rectangle of the warped image after perspective transformation.
      - It calculates the transformed coordinates of the image corners

11. **checkInteriorExterior(const cv::Mat &mask, const cv::Rect &croppingMask, int &top, int &bottom, int &left, int &right)**

- This function adjusts the cropping mask by checking the exterior of the contour
  - It identifies if the top, right, bottom and left borders of the cropping rectangle has to be moved by comparing the exterior and interior of the contour

12. **crop(cv::Mat &image,cv::Mat &gray)**

- This function crops the stitched image by removing the black areas around the image
  - It finds the contours in the image and sorts them to identify the largest contour
  - It initializes a cropping mask for the identified contour and calls upon the checkInteriorExterior() function for adjustments
  - Adjusts the borders and returns the cropped image
