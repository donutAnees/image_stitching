# Running the Program
## Prerequisite
- **OPENCV**, Our project uses the open source library OPENCV, for implementing some of the functionalities our code. The minimum version we have is OPENCV 3.0, hence it is requested to have a version higher than this to run this program without any issues.
- **CMAKE**, We have used CMAKE for building our project, it must be installed to generate the project files for the required platform. 

## Generate the necessary files
From the **/IMAGE_STITCHING** directory, run the following commands
`cd opencv` and 
`cmake .`
`make`
This will search for the CMakeLists.txt file in the opencv folder and follows the specification mentioned there to generate the project files and builds the target.

## Input to the program
The program expects the following arguments as input
    - Number of images (ex: 4)
    - Name of the images with extension (ex: img1.jpg img2.jpg img3.jpg img4.jpg)

```
./Stitcher 2 img1.jpg img2.jpg

```

If resizing is enabled, the program will be executed with high speed with a hit at accuracy, if accuracy is preferred then resizing can be disabled this will make the program run a bit slower but the results will be accurate.

Once the program is done executing the result is written to the result.jpg file