# Algorithms

## Stitching Overview

In order to perform image stitching of various images from different viewpoints, we must first identify the various keypoints in the different images. Keypoints are points which are interesting in the image, these are corners, edges or blobs which are different from the the pixels surrounding them making them distinct. Once the keypoints are detected, the next step involves identifying the various descriptors. Descriptors are compact representations of the local neighborhood around each keypoint in an image. While keypoints provide the location of distinctive points, descriptors encode information about the appearance or gradient distribution of pixels in the vicinity of each keypoint. Once we do this for each image, the next step involves matching the features which are similar in the different images. Not every matches are accurate, we use RANSAC algorithm to remove any outliers. Once the matching of keypoints between images is completed, the next step in image stitching is to compute the homography matrix. The homography matrix is a transformation matrix that describes the mapping between two perspective views of the same scene. This matrix is determined by the matching we did earlier and tells us what transformation must be done to convert one image's descriptor and keypoint into the another image. Using this homology matrix we warp one image and superimpose the other image on this to get the stitched image. Finally we blend the overlaps to remove the seam.

## ORB Overview

We have used ORB(Oriented FAST and Rotated BRIEF) for the detection of keypoints and descriptors. ORB effectively combines FAST and BRIEF algorithm and provides results twice as fast and accurate as SIFT and SURF. FAST (Features from Accelerated Segment Test) works as follows:

1. The algorithm iterates for all the pixels in the array.
2. The algorithm considers a circle of radius 3 around it and every pixel the circle touches is stored in an array.
3. If there are 12 contigous pixels in the circle (array) which are either greater than I + threshold or less than I - threshold, then that point is considered a keypoint. (I is the intensity of the current pixel and the threshold is normally 0.2 * I).
FAST is scale invariant, therefore to overcome this limitation, we apply FAST to the scaled down images of the current image. Once we find the keypoints from the various resolutions, we can then keep the keypoints which are common in all the scales and decide the direction of the keypoint using moments of that feature.
BRIEF (Binary Robust Independent Elementary Features) works as follows:

1. The algorithm smooths the image patch around the detected keypoints using a Gaussian filter, to reduce the sensitivity to noise.
2. For each keypoint, a random pair of pixels are selected within the patch, and an intensity comparison is done for each pair. If the first pixel in the pair is brighter, 1 is assigned to the corresponding bit, otherwise, 0 is assigned. This is repeated a number of times (n_d = 128, 256, 512; k = n_d/8 gives the number of bits required to store the descriptor), until a long binary string descriptor is created for the keypoint.
3. The descriptors generated from different images are compared using the Hamming distance, which counts the number of different bits between the bit strings. Smaller Hamming distances imply a closer and more likely match.
BRIEF is not inherently rotation invariant, so to overcome this limitation, different orientation estimation techniques are used.

## Feature Matching Overview

There are two types of algorithms for feature matching, FLANN and BruteForce. We tested both these algorithms and decided to choose BruteForce as it gives the best matches. Brute force matching is more robust to variations in the dataset and descriptors. FLANN's approximation techniques may not handle all types of descriptorsvery well, particularly with binary descriptors like those generated by ORB. BruteForce works by works by exhaustively comparing each feature descriptor in one set with all feature descriptors in another set to find the closest matches. For BruteForce we use NORM_HAMMING, it calculates the Hamming distance between two binary strings, which represents the number of bits that differ between the two strings. This is best suited for ORB since BRIEF produces descriptors which are binary strings.

## Stitching Multiple Images

We begin by considering the middle image, the reasons to choose is as follows

1. When we apply warping to images from left to right or right to left, the entire final image gets more distorted near the end. ORB is sensitive to distortion and will not be able to match the keypoints detected at these ends, and therefore it is not optimal to so.
2. It is computationally less expensive, consider images 1,2,3,4,5 if we proceed from left to right then we match keypoints and get the result in the following order 1 -> 2 = RESULT1, RESULT1 -> 3 = RESULT2, RESULT3 -> 4 = RESULT4, RESULT4 -> 5 = RESULT6(Once for 1 and 2 to get RESULT1 (1+1) , then once for RESULT1 and 3 to get RESULT2 (1+1),then once for RESULT2 and 4 to get RESULT3(1+1) and finally once for RESULT3 and 5 (1+1)). Here the total number of times we need to detect the keypoints and descriptos for each of the images is 8. Now consider stitching from the middle 2 -> 3 <- 4 = RESULT1, 1 -> RESULT1 <- 4, here the total number of times we need to detect the keypoints is 6 (Once for 2, 3 and 4 and warp them to get RESULT1(1 + 1 + 1), Once for RESULT1, 1 and 5 to get RESULT2(1 + 1 + 2)) this is lesser than stitching from left to right or right to left.
