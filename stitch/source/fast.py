import cv2 as cv
import numpy as np

image = cv.imread("../img/img1.ppm", cv.IMREAD_GRAYSCALE)

fastDetector = cv.FastFeatureDetector_create(threshold = 20)

fastDetector.setNonmaxSuppression(False)

keypoints = fastDetector.detect(image, None)
imgwithKpts = cv.drawKeypoints(image.copy(), keypoints, None, (128,0,128), cv.DRAW_MATCHES_FLAGS_DEFAULT)

cv.namedWindow("FAST without NMS", cv.WINDOW_NORMAL)
cv.imshow("FAST without NMS", imgwithKpts)

fastDetector.setNonmaxSuppression(True)

keypoints_NMS = fastDetector.detect(image, None)
imgwithKpts_NMS = cv.drawKeypoints(image.copy(), keypoints_NMS, None, (128,128,0), cv.DRAW_MATCHES_FLAGS_DEFAULT)

cv.namedWindow("FAST with NMS", cv.WINDOW_NORMAL)
cv.imshow("FAST with NMS", imgwithKpts_NMS)

cv.waitKey(0)
cv.destroyAllWindows()